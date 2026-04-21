/*
 * Filename: deadlock.c
 * Purpose: Implements Banker's Algorithm deadlock avoidance and basic deadlock detection utilities.
 * Author: [Author Name]
 * Date: 2026-04-21
 */
#include <stdio.h> // Provides printf, scanf, and snprintf for user prompts and resource-table output.
#include <string.h> // Provides memset-style semantics support if future matrix-reset helpers are expanded.
#include "../include/deadlock.h" // Provides matrix declarations and function contracts for deadlock management.

int allocation[MAX_PROCESSES][NUM_RESOURCES] = {{0}}; // Defines allocation matrix storing resources currently held by each process.
int maximum[MAX_PROCESSES][NUM_RESOURCES] = {{0}}; // Defines maximum-claim matrix storing peak resource demands per process.
int available[NUM_RESOURCES] = {0, 0, 0, 0}; // Defines currently available units for radio channels, ambulances, fire trucks, and police cars.
int need[MAX_PROCESSES][NUM_RESOURCES] = {{0}}; // Defines remaining-need matrix storing how many more units each process may still request.

static int configured_processes = 0; // Tracks how many processes were configured so loops avoid uninitialized matrix rows.

static const char *resource_names[NUM_RESOURCES] = { // Maps resource indices to readable names for prompts and diagnostics.
	"radio_channels", // Labels index 0 as radio communication channels resource units.
	"ambulances", // Labels index 1 as ambulance vehicle resource units.
	"fire_trucks", // Labels index 2 as fire truck resource units.
	"police_cars" // Labels index 3 as police car resource units.
}; // Closes resource-name mapping array after all four resource classes are labeled.

static void clear_input_buffer(void) { // Clears leftover console input so repeated scanf calls remain reliable.
	int ch = 0; // Stores one consumed character at a time while draining stdin.
	while ((ch = getchar()) != '\n' && ch != EOF) { // Continues draining until line end or EOF to prevent prompt-skipping issues.
	} // Closes drain loop body because character consumption itself is the required side effect.
} // Ends buffer-clear helper after stdin line cleanup is complete.

static int read_bounded_int(const char *prompt, int minimum, int maximum_value) { // Reads an integer safely and enforces numeric bounds for resource input.
	int value = 0; // Stores parsed numeric input before validation checks are applied.
	int scan_status = 0; // Tracks scanf conversion success so invalid text can be handled cleanly.
	while (1) { // Repeats until valid in-range integer input is provided by the user.
		printf("%s", prompt); // Prints caller-provided prompt so user knows which field to enter.
		scan_status = scanf("%d", &value); // Attempts to parse an integer from standard input.
		if (scan_status != 1) { // Detects non-numeric input so matrix values are never corrupted by bad parsing.
			printf("Invalid input. Please enter a numeric value.\n"); // Explains parse failure so user can correct the next attempt.
			clear_input_buffer(); // Removes invalid characters so next scanf starts with clean input.
			continue; // Restarts prompt loop because no valid integer was captured.
		} // Closes parse-failure branch after cleanup and retry logic.
		clear_input_buffer(); // Clears trailing newline so subsequent prompts are not skipped.
		if (value < minimum || value > maximum_value) { // Enforces allowed bounds so resource data remains realistic and consistent.
			printf("Please enter a value between %d and %d.\n", minimum, maximum_value); // Reports valid range to guide corrected input.
			continue; // Retries because out-of-range values violate matrix constraints.
		} // Closes bounds-check branch after either retry or acceptance.
		return value; // Returns validated integer so caller can safely store it in resource matrices.
	} // Closes validation loop after guaranteed return on successful input.
} // Ends bounded integer reader helper after delivering safe numeric input.

void init_resources(void) { // Initializes available resources and captures allocation/maximum claims for Banker's Algorithm.
	int process_index = 0; // Iterates through configured processes while collecting matrix values.
	int resource_index = 0; // Iterates through resource columns for each process row.
	char prompt[160] = {0}; // Stores formatted prompt text so user input questions include process/resource context.
	available[0] = 6; // Sets sample available radio channels for emergency communication operations.
	available[1] = 4; // Sets sample available ambulances for medical dispatch operations.
	available[2] = 3; // Sets sample available fire trucks for fire-response operations.
	available[3] = 5; // Sets sample available police cars for law-enforcement response operations.
	for (process_index = 0; process_index < MAX_PROCESSES; process_index++) { // Clears all matrix rows so reinitialization starts from clean state.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Clears each resource column for current process row.
			allocation[process_index][resource_index] = 0; // Resets current allocation so stale ownership data is removed.
			maximum[process_index][resource_index] = 0; // Resets maximum claims so old demand limits do not persist.
			need[process_index][resource_index] = 0; // Resets remaining need so it can be recomputed from fresh input.
		} // Closes inner reset loop after current process row is cleared.
	} // Closes outer reset loop after all process rows are reset.
	printf("\nSample Available Resources:\n"); // Prints heading so user sees baseline availability before entering claims.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Iterates resources to display each available count clearly.
		printf("  %s = %d\n", resource_names[resource_index], available[resource_index]); // Prints each resource name and available unit count.
	} // Closes availability display loop after all resources are shown.
	configured_processes = read_bounded_int("Enter number of processes to configure for Banker (1-20): ", 1, MAX_PROCESSES); // Captures active process count so algorithms only process initialized rows.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Collects allocation and maximum claims for each configured process.
		printf("\nConfigure process P%d:\n", process_index); // Prints process header so user input remains organized by process.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Gathers current allocation values for each resource type.
			snprintf(prompt, sizeof(prompt), "  Allocation for P%d (%s): ", process_index, resource_names[resource_index]); // Builds contextual prompt string for allocation entry.
			allocation[process_index][resource_index] = read_bounded_int(prompt, 0, 9999); // Stores non-negative allocation because held resources cannot be negative.
		} // Closes allocation-input loop after all four resources are captured for this process.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Gathers maximum claims for each resource type.
			int minimum_maximum = allocation[process_index][resource_index]; // Sets lower bound so maximum claim cannot be less than currently allocated units.
			snprintf(prompt, sizeof(prompt), "  Maximum for P%d (%s) [>= %d]: ", process_index, resource_names[resource_index], minimum_maximum); // Builds prompt explaining minimum legal maximum claim.
			maximum[process_index][resource_index] = read_bounded_int(prompt, minimum_maximum, 9999); // Stores validated maximum so Banker inequalities remain valid.
		} // Closes maximum-input loop after this process claim vector is complete.
	} // Closes process-configuration loop after all active process rows are entered.
	calculate_need(); // Computes need matrix immediately so safety and request checks use current values.
	if (is_safe_state() == 1) { // Evaluates initial state safety so user receives immediate deadlock-risk feedback.
		printf("Initial configuration is SAFE under Banker's Algorithm.\n"); // Confirms configuration allows a complete safe execution sequence.
	} else { // Handles unsafe initial state to warn user before processing further requests.
		printf("Initial configuration is UNSAFE under Banker's Algorithm.\n"); // Warns that current claims/availability cannot guarantee completion.
	} // Closes initial safety feedback branch.
} // Ends initialization after matrix input, need computation, and safety check are complete.

void calculate_need(void) { // Recomputes remaining need matrix from maximum and current allocation values.
	int process_index = 0; // Iterates through configured process rows for matrix computation.
	int resource_index = 0; // Iterates through resource columns within each process row.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Processes only configured rows so uninitialized rows remain ignored.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Computes one need value per process/resource cell.
			need[process_index][resource_index] = maximum[process_index][resource_index] - allocation[process_index][resource_index]; // Applies Banker formula need = maximum - allocation.
			if (need[process_index][resource_index] < 0) { // Guards against unexpected negative values if user data is inconsistent.
				need[process_index][resource_index] = 0; // Clamps to zero so algorithms never treat negative remaining demand as valid.
			} // Closes negative-need guard after potential correction.
		} // Closes inner loop after all resource needs for this process are computed.
	} // Closes outer loop after all configured process needs are updated.
} // Ends need-calculation routine after matrix refresh is complete.

int is_safe_state(void) { // Runs the Banker's safety algorithm to test whether all processes can eventually finish.
	int work[NUM_RESOURCES] = {0}; // Stores a mutable copy of available resources used to simulate future allocations/releases.
	int finish[MAX_PROCESSES] = {0}; // Tracks whether each process can finish in the simulated safe-sequence search.
	int process_index = 0; // Iterates process rows while evaluating runnable candidates.
	int resource_index = 0; // Iterates resource columns during need and release checks.
	int progress_made = 1; // Tracks whether at least one process finished in a pass to control algorithm termination.
	int completed_count = 0; // Counts simulated completions so final safety verdict can be produced.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Copies available vector into work before simulation begins.
		work[resource_index] = available[resource_index]; // Initializes work with currently free units because those are immediately usable.
	} // Closes work-initialization loop after all resource classes are copied.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Initializes finish flags for all configured processes.
		finish[process_index] = 0; // Marks each process unfinished so algorithm must prove it can complete.
	} // Closes finish initialization loop after active rows are prepared.
	while (progress_made == 1) { // Repeats passes while at least one new process can complete in current simulated state.
		progress_made = 0; // Resets pass progress flag so this iteration must discover fresh completion to continue.
		for (process_index = 0; process_index < configured_processes; process_index++) { // Scans each unfinished process to see if its need fits work.
			int can_finish = 1; // Assumes process can finish until any resource requirement disproves it.
			if (finish[process_index] == 1) { // Skips processes already proven completable in earlier passes.
				continue; // Continues to next process because this row is already resolved.
			} // Closes already-finished guard branch.
			for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Compares each needed resource against currently simulated availability.
				if (need[process_index][resource_index] > work[resource_index]) { // Detects unmet demand meaning this process cannot finish now.
					can_finish = 0; // Marks process as currently blocked so it is not selected this pass.
					break; // Stops checking remaining resources because one shortage is sufficient to block completion.
				} // Closes shortage-detection branch for this resource comparison.
			} // Closes need-vs-work loop after all resources are tested or shortage found.
			if (can_finish == 1) { // Runs completion simulation when process need is fully satisfiable by work vector.
				for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Simulates process finishing and releasing its held resources.
					work[resource_index] = work[resource_index] + allocation[process_index][resource_index]; // Adds released allocation back into work for subsequent processes.
				} // Closes resource-release loop after simulated completion release is applied.
				finish[process_index] = 1; // Marks process as finishable in safe sequence to avoid reevaluating it.
				progress_made = 1; // Records progress so algorithm performs another pass for newly enabled processes.
				completed_count = completed_count + 1; // Increments completion count toward full-system safety verification.
			} // Closes completion-simulation branch after safe-sequence advancement.
		} // Closes process scan for current pass.
	} // Closes iterative safety loop when no additional process can be completed.
	if (completed_count == configured_processes) { // Declares state safe only if every configured process can finish.
		return 1; // Returns safe-state indicator expected by caller logic.
	} // Closes safe-result branch.
	return 0; // Returns unsafe-state indicator when at least one process cannot complete.
} // Ends safety algorithm after producing safe/unsafe result.

int request_resources(int pid, int request[]) { // Attempts to grant one process request while preserving Banker's safety guarantees.
	int resource_index = 0; // Iterates through requested resource vector entries.
	if (pid < 0 || pid >= configured_processes) { // Validates target process index so matrix access stays in configured range.
		printf("Request denied: invalid PID/index %d.\n", pid); // Explains denial because process row does not exist.
		return 0; // Returns denial status because request cannot be evaluated safely.
	} // Closes PID validation branch.
	if (request == NULL) { // Validates request pointer so dereferencing does not trigger undefined behavior.
		printf("Request denied: request vector is NULL.\n"); // Explains denial because no resource quantities were supplied.
		return 0; // Returns denial status because request content is unavailable.
	} // Closes null-request guard branch.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Validates each requested quantity for basic correctness constraints.
		if (request[resource_index] < 0) { // Rejects negative request values because resource demand cannot be negative.
			printf("Request denied: negative request for %s is invalid.\n", resource_names[resource_index]); // Explains which resource violated request rules.
			return 0; // Returns denial because invalid vector values invalidate the entire request.
		} // Closes negative-request guard for this resource.
		if (request[resource_index] > need[pid][resource_index]) { // Enforces Banker rule that process cannot request beyond its declared remaining need.
			printf("Request denied: requested %d %s but need is only %d.\n", request[resource_index], resource_names[resource_index], need[pid][resource_index]); // Explains denial as over-claim beyond maximum contract.
			return 0; // Returns denial because granting over-claim breaks Banker model assumptions.
		} // Closes over-need guard for this resource.
		if (request[resource_index] > available[resource_index]) { // Checks immediate availability so impossible grants are denied upfront.
			printf("Request denied: only %d %s available right now.\n", available[resource_index], resource_names[resource_index]); // Explains shortage preventing immediate grant.
			return 0; // Returns denial because system currently lacks sufficient free units.
		} // Closes immediate-availability guard for this resource.
	} // Closes per-resource validation loop after all constraints pass.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Applies tentative allocation so safety can be tested on resulting state.
		available[resource_index] = available[resource_index] - request[resource_index]; // Removes granted units from available vector during trial grant.
		allocation[pid][resource_index] = allocation[pid][resource_index] + request[resource_index]; // Adds granted units to process allocation in tentative state.
		need[pid][resource_index] = need[pid][resource_index] - request[resource_index]; // Reduces remaining need to reflect newly allocated units.
	} // Closes tentative-grant loop after all resource classes are updated.
	if (is_safe_state() == 1) { // Tests whether tentative grant preserves at least one complete safe sequence.
		printf("Request granted: system remains in a SAFE state.\n"); // Confirms grant because Banker's safety condition is satisfied.
		return 1; // Returns success status so caller knows allocation is committed.
	} // Closes safe-grant branch after successful safety evaluation.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Rolls back tentative allocation when safety check fails.
		available[resource_index] = available[resource_index] + request[resource_index]; // Restores available units because unsafe grant must be undone.
		allocation[pid][resource_index] = allocation[pid][resource_index] - request[resource_index]; // Restores original allocation because request is denied.
		need[pid][resource_index] = need[pid][resource_index] + request[resource_index]; // Restores original need because no actual grant occurred.
	} // Closes rollback loop after all vectors are reverted to pre-request state.
	printf("Request denied: granting it would move the system to an UNSAFE state.\n"); // Explains denial reason as Banker safety violation.
	return 0; // Returns denial status after unsafe tentative grant rollback.
} // Ends request handler after grant or rollback path completes.

void display_resource_table(void) { // Prints Banker matrices so operators can inspect allocation/need/availability at a glance.
	int process_index = 0; // Iterates process rows while printing matrix table output.
	int resource_index = 0; // Iterates resource columns for each printed matrix segment.
	if (configured_processes <= 0) { // Handles uninitialized state so table output remains meaningful before setup.
		printf("Resource table is empty. Run init_resources() first.\n"); // Instructs user to initialize matrices before viewing table.
		return; // Exits early because there are no configured process rows to print.
	} // Closes empty-state guard branch.
	printf("\n================ Banker Resource Table ================\n"); // Prints table title to distinguish this report from other module output.
	printf("Resources: [radio_channels, ambulances, fire_trucks, police_cars]\n"); // Prints resource ordering so vector columns are interpreted correctly.
	printf("Available: "); // Prints availability label before vector values.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Prints each current availability value in defined resource order.
		printf("%s=%d ", resource_names[resource_index], available[resource_index]); // Prints name/value pair for one resource class.
	} // Closes availability print loop after all resource values are shown.
	printf("\n-------------------------------------------------------\n"); // Prints separator so matrix header and rows are visually distinct.
	printf("PID | Allocation                | Maximum                   | Need\n"); // Prints column labels for allocation, maximum, and need vectors.
	printf("-------------------------------------------------------\n"); // Prints second separator to frame process rows.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Prints one row per configured process.
		printf("P%-2d | [", process_index); // Starts row with process label and opening allocation bracket.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Prints allocation vector values for this process.
			printf("%d%s", allocation[process_index][resource_index], (resource_index == NUM_RESOURCES - 1) ? "" : ", "); // Prints allocation value with comma separator except after last element.
		} // Closes allocation-vector print loop after four resource values are emitted.
		printf("] | ["); // Prints separator between allocation and maximum vectors.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Prints maximum vector values for this process.
			printf("%d%s", maximum[process_index][resource_index], (resource_index == NUM_RESOURCES - 1) ? "" : ", "); // Prints maximum value with consistent comma formatting.
		} // Closes maximum-vector print loop after all claims are displayed.
		printf("] | ["); // Prints separator between maximum and need vectors.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Prints need vector values for this process.
			printf("%d%s", need[process_index][resource_index], (resource_index == NUM_RESOURCES - 1) ? "" : ", "); // Prints need value with consistent comma formatting.
		} // Closes need-vector print loop after all remaining demands are displayed.
		printf("]\n"); // Ends current process row after all three vectors are printed.
	} // Closes process-row loop after full matrix table is printed.
	printf("=======================================================\n"); // Prints closing border to complete formatted table output.
} // Ends resource-table display function.

int detect_deadlock(void) { // Performs simple deadlock detection by simulating completion using current availability and allocations.
	int work[NUM_RESOURCES] = {0}; // Stores mutable available-resource copy used during detection simulation.
	int finish[MAX_PROCESSES] = {0}; // Tracks whether each process can eventually complete in detection pass.
	int process_index = 0; // Iterates process rows while evaluating blocked/completable states.
	int resource_index = 0; // Iterates resource columns for need and release checks.
	int progress_made = 1; // Tracks whether any additional process can complete in current pass.
	int deadlock_found = 0; // Tracks whether at least one process remains permanently blocked.
	if (configured_processes <= 0) { // Handles uninitialized state so detection is not run on empty configuration.
		printf("Deadlock detection skipped: no configured processes.\n"); // Reports why no detection result is produced.
		return 0; // Returns no-deadlock result for empty system state.
	} // Closes empty-configuration guard branch.
	for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Initializes work with currently available resources before simulation.
		work[resource_index] = available[resource_index]; // Copies availability because detection should not mutate real availability vector.
	} // Closes work initialization loop after all resource classes are copied.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Initializes finish flags using simple "holds resources" heuristic.
		int holds_resources = 0; // Tracks whether this process currently holds any resources that could later be released.
		for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Scans allocation row to determine whether process owns resources.
			if (allocation[process_index][resource_index] > 0) { // Detects positive allocation indicating process owns at least one unit.
				holds_resources = 1; // Marks process as resource holder for detection initialization logic.
			} // Closes ownership-detection branch for this resource cell.
		} // Closes ownership scan loop after all resources are checked for this process.
		if (holds_resources == 0) { // Marks processes with no allocation as already finishable in this simplified detector.
			finish[process_index] = 1; // Treats zero-allocation process as not contributing to deadlock cycle.
		} else { // Handles processes that hold resources and may participate in wait cycles.
			finish[process_index] = 0; // Marks process unfinished so algorithm must prove it can complete.
		} // Closes finish-initialization branch for this process.
	} // Closes finish initialization loop after all process rows are prepared.
	while (progress_made == 1) { // Repeats while additional processes can be resolved in simulation.
		progress_made = 0; // Resets pass progress so this iteration must discover fresh completion to continue.
		for (process_index = 0; process_index < configured_processes; process_index++) { // Scans unresolved processes to see if current work can satisfy their need.
			int can_complete = 1; // Assumes process can complete until any unmet need disproves it.
			if (finish[process_index] == 1) { // Skips processes already marked complete in prior passes.
				continue; // Moves to next process because this one no longer affects detection.
			} // Closes already-finished branch.
			for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Compares each outstanding need with currently simulated work.
				if (need[process_index][resource_index] > work[resource_index]) { // Detects shortage meaning process cannot complete with current work.
					can_complete = 0; // Marks process blocked for this pass.
					break; // Stops resource comparison early because one shortage is enough to block completion.
				} // Closes shortage check for this resource.
			} // Closes need-check loop for current process.
			if (can_complete == 1) { // Simulates process completion when all needs can be met from work.
				for (resource_index = 0; resource_index < NUM_RESOURCES; resource_index++) { // Releases process allocation back to work after simulated completion.
					work[resource_index] = work[resource_index] + allocation[process_index][resource_index]; // Adds held resources to work so other waiting processes may proceed.
				} // Closes release loop after all resources are returned to work.
				finish[process_index] = 1; // Marks process as completable so detector does not revisit it.
				progress_made = 1; // Records progress so detector runs another pass for newly enabled processes.
			} // Closes completion-simulation branch.
		} // Closes process scan loop for this detection pass.
	} // Closes iterative detection loop when no new process can be resolved.
	for (process_index = 0; process_index < configured_processes; process_index++) { // Inspects unresolved processes to determine whether deadlock exists.
		if (finish[process_index] == 0) { // Identifies process that remains blocked after all simulation passes.
			if (deadlock_found == 0) { // Prints heading once before listing blocked processes.
				printf("Potential deadlock detected. Blocked processes: "); // Announces detection before blocked process identifiers are listed.
			} // Closes heading-print guard branch.
			deadlock_found = 1; // Flags that at least one process is stuck waiting indefinitely.
			printf("P%d ", process_index); // Prints blocked process identifier for operator diagnosis.
		} // Closes blocked-process check for this process row.
	} // Closes unresolved-process inspection loop.
	if (deadlock_found == 1) { // Handles output for deadlock-present case.
		printf("\n"); // Ends blocked-process list line after all IDs are printed.
	} else { // Handles output for no-deadlock case.
		printf("No deadlock detected: every process can eventually proceed or release resources.\n"); // Reports clean state when all processes were resolvable.
	} // Closes final-status output branch.
	return deadlock_found; // Returns detection result where 1 means blocked processes remain and 0 means no deadlock.
} // Ends deadlock detection routine after reporting and returning status.
