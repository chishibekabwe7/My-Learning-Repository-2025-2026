/*
 * Filename: scheduler.c
 * Purpose: Implements FCFS and SJF scheduling simulations and returns aggregate scheduling metrics.
 * Author: [Author Name]
 * Date: 2026-04-21
 */
#include <stdio.h> // We include stdio so the scheduler can print Gantt charts and status messages.
#include "../include/scheduler.h" // We include scheduler declarations so definitions match declared return types.
#include "../include/process.h" // We include process definitions so the scheduler can access PCB fields and states.

static int collect_schedulable_processes(PCB *selected[], int limit) { // We gather only eligible processes so algorithms ignore blocked or finished tasks.
	int index = 0; // We use an index counter to scan every slot in the fixed-size process table.
	int selected_count = 0; // We track how many NEW or READY processes are available for scheduling.
	for (index = 0; index < MAX_PROCESSES; index++) { // We iterate across all table slots because entries are stored globally.
		if (process_table[index].pid > 0 && (process_table[index].state == NEW || process_table[index].state == READY)) { // We accept only initialized processes that are eligible to run now.
			if (selected_count < limit) { // We guard writes so we never overflow the temporary pointer array.
				selected[selected_count] = &process_table[index]; // We store a pointer so sorting does not destroy original table storage order.
				selected_count = selected_count + 1; // We increment the count so the next eligible process uses the next pointer slot.
			} // We close the capacity guard after conditionally inserting the pointer.
		} // We close eligibility filtering after handling this table entry.
	} // We finish scanning once every process-table slot has been checked.
	return selected_count; // We return the number of schedulable processes so callers can drive sorting and metrics safely.
} // We end collection helper after building the filtered scheduling list.

static void sort_selected_fcfs(PCB *selected[], int count) { // We isolate FCFS sorting so arrival-time ordering stays reusable and testable.
	int pass = 0; // We track bubble-sort passes because each pass moves one largest item toward the end.
	int pos = 0; // We track pair position so adjacent elements can be compared and swapped.
	for (pass = 0; pass < count - 1; pass++) { // We run enough passes to guarantee total ordering for all selected processes.
		for (pos = 0; pos < count - pass - 1; pos++) { // We compare adjacent pairs inside the unsorted suffix for this pass.
			if (selected[pos]->arrival_time > selected[pos + 1]->arrival_time || (selected[pos]->arrival_time == selected[pos + 1]->arrival_time && selected[pos]->pid > selected[pos + 1]->pid)) { // We order earlier arrivals first and break ties by PID for deterministic output.
				PCB *temp = selected[pos]; // We hold the left pointer temporarily so we can perform a safe swap.
				selected[pos] = selected[pos + 1]; // We move the right pointer left because it should execute earlier.
				selected[pos + 1] = temp; // We place the original left pointer to the right to complete the swap.
			} // We close comparison branch after optional swap.
		} // We close inner loop after all adjacent pairs in this pass are processed.
	} // We close outer loop when FCFS ordering is fully established.
} // We end FCFS sort helper so run_fcfs can focus on simulation steps.

static void sort_selected_sjf(PCB *selected[], int count) { // We isolate SJF sorting so shortest-burst ordering can be applied consistently.
	int pass = 0; // We count passes for bubble sort so repeated adjacency checks eventually fully sort the list.
	int pos = 0; // We track current adjacent-pair index for comparisons during each pass.
	for (pass = 0; pass < count - 1; pass++) { // We perform count-1 passes because that is sufficient for bubble-sort completion.
		for (pos = 0; pos < count - pass - 1; pos++) { // We compare neighboring entries that are still in the unsorted region.
			if (selected[pos]->burst_time > selected[pos + 1]->burst_time || (selected[pos]->burst_time == selected[pos + 1]->burst_time && selected[pos]->arrival_time > selected[pos + 1]->arrival_time) || (selected[pos]->burst_time == selected[pos + 1]->burst_time && selected[pos]->arrival_time == selected[pos + 1]->arrival_time && selected[pos]->pid > selected[pos + 1]->pid)) { // We prioritize shorter jobs, then earlier arrivals, then lower PID for stable deterministic order.
				PCB *temp = selected[pos]; // We store one pointer temporarily so swapping can occur without losing data.
				selected[pos] = selected[pos + 1]; // We move the preferred right-side entry into the earlier execution slot.
				selected[pos + 1] = temp; // We place the previous left entry behind it to complete ordering.
			} // We close SJF comparison branch after an optional swap.
		} // We finish this pass after checking all adjacent unsorted pairs.
	} // We finish all passes when SJF ordering is complete.
} // We end SJF sort helper to keep run_sjf logic concise.

static ScheduleResult simulate_non_preemptive(PCB *selected[], int count, const char *label) { // We centralize non-preemptive execution so FCFS and SJF share one tested simulation path.
	ScheduleResult result = {0.0f, 0.0f, 0.0f}; // We initialize metrics to zero so empty schedules return safe default values.
	int current_time = 0; // We track simulated clock time to compute starts, finishes, and turnaround values.
	int first_start_time = -1; // We remember first execution time so utilization reflects actual scheduling window.
	int total_burst_time = 0; // We accumulate busy CPU time so utilization can be computed later.
	int total_waiting_time = 0; // We accumulate waiting times so average waiting can be calculated.
	int total_turnaround_time = 0; // We accumulate turnaround times so average turnaround can be calculated.
	int index = 0; // We use an index variable to iterate through sorted process pointers.
	if (count == 0) { // We guard against empty schedules so no divide-by-zero happens in metric formulas.
		printf("No NEW/READY processes available for %s scheduling.\n", label); // We notify the operator why no schedule was produced.
		return result; // We return zeroed metrics because no process execution occurred.
	} // We close empty-schedule guard after returning defaults.
	printf("\nGantt Chart (%s):\n", label); // We print a chart heading so users know which algorithm generated the timeline.
	printf("[ProcessName | start-end]\n"); // We print the chart format legend so segment text is easy to interpret.
	for (index = 0; index < count; index++) { // We execute each sorted process sequentially because FCFS and SJF are non-preemptive here.
		int start_time = 0; // We hold start time per process so waiting and chart output are computed clearly.
		int finish_time = 0; // We hold finish time per process so turnaround can be derived accurately.
		if (current_time < selected[index]->arrival_time) { // We account for CPU idle gaps when the next process has not arrived yet.
			current_time = selected[index]->arrival_time; // We fast-forward time to arrival because execution cannot start before process existence.
		} // We close idle-gap handling before recording actual start time.
		if (first_start_time < 0) { // We capture the first execution instant once so utilization window is anchored correctly.
			first_start_time = current_time; // We store initial start to represent when CPU first became active in this run.
		} // We close first-start capture after possible assignment.
		start_time = current_time; // We set process start equal to current clock since CPU begins executing it now.
		selected[index]->state = RUNNING; // We set RUNNING so process state reflects active CPU ownership during simulation.
		finish_time = start_time + selected[index]->burst_time; // We compute finish as start plus required CPU burst for non-preemptive execution.
		// Formula: turnaround_time = finish_time - arrival_time, because turnaround measures total elapsed time from process arrival to completion.
		selected[index]->turnaround_time = finish_time - selected[index]->arrival_time; // We store turnaround to capture complete lifecycle duration in the system.
		// Formula: waiting_time = turnaround_time - burst_time, because waiting is the non-executing portion of total turnaround.
		selected[index]->waiting_time = selected[index]->turnaround_time - selected[index]->burst_time; // We store waiting to quantify queue delay before CPU service.
		selected[index]->remaining_time = 0; // We mark remaining work as zero because non-preemptive execution completed the full burst.
		selected[index]->state = TERMINATED; // We set TERMINATED so downstream modules know this process finished execution.
		total_waiting_time = total_waiting_time + selected[index]->waiting_time; // We accumulate waiting to support average-waiting-time computation.
		total_turnaround_time = total_turnaround_time + selected[index]->turnaround_time; // We accumulate turnaround to support average-turnaround computation.
		total_burst_time = total_burst_time + selected[index]->burst_time; // We add burst to track total CPU busy time across scheduled processes.
		current_time = finish_time; // We advance simulation clock because CPU is now free only after this finish time.
		printf("[%s | %d-%d] ", selected[index]->name, start_time, finish_time); // We print one Gantt segment so execution order and time range are visible.
	} // We close process-execution loop after every selected process has been simulated.
	printf("\n"); // We end the Gantt output line so following logs appear on a fresh line.
	result.avg_waiting_time = (float)total_waiting_time / (float)count; // We compute mean waiting time so algorithm queue efficiency can be compared.
	result.avg_turnaround_time = (float)total_turnaround_time / (float)count; // We compute mean turnaround time so end-to-end responsiveness is measurable.
	if (current_time > first_start_time) { // We ensure positive elapsed window so utilization division is mathematically valid.
		// Formula: cpu_utilization = (total_burst_time / (last_finish_time - first_start_time)) * 100, because utilization is busy-time share of total schedule window.
		result.cpu_utilization = ((float)total_burst_time / (float)(current_time - first_start_time)) * 100.0f; // We convert busy-time ratio into a percentage for standard CPU utilization reporting.
	} else { // We handle degenerate timing windows to avoid division by zero.
		result.cpu_utilization = 0.0f; // We report zero utilization because no measurable execution interval exists.
	} // We close utilization computation branch after assigning a safe metric value.
	return result; // We return populated metrics so callers can display or log scheduler performance.
} // We end shared simulator helper after all non-preemptive metrics are computed.

ScheduleResult run_fcfs(void) { // We implement FCFS so tasks are scheduled by earliest arrival time.
	PCB *selected[MAX_PROCESSES] = {0}; // We allocate pointer slots to hold filtered schedulable processes.
	int count = 0; // We track how many eligible entries were collected for this run.
	count = collect_schedulable_processes(selected, MAX_PROCESSES); // We collect NEW/READY processes because only those states are allowed to be scheduled.
	sort_selected_fcfs(selected, count); // We sort by arrival time to satisfy FCFS ordering rules before simulation.
	return simulate_non_preemptive(selected, count, "FCFS"); // We simulate execution and return aggregated FCFS performance metrics.
} // We end FCFS entry point after returning the computed ScheduleResult.

ScheduleResult run_sjf(void) { // We implement SJF so tasks are scheduled by shortest required CPU burst.
	PCB *selected[MAX_PROCESSES] = {0}; // We allocate pointer slots to hold schedulable processes for burst-based sorting.
	int count = 0; // We track the number of NEW/READY processes selected for this SJF run.
	count = collect_schedulable_processes(selected, MAX_PROCESSES); // We filter to schedulable states because blocked/terminated tasks must not run.
	sort_selected_sjf(selected, count); // We sort by burst time to satisfy SJF behavior before executing processes.
	return simulate_non_preemptive(selected, count, "SJF"); // We run the common simulator and return SJF metrics.
} // We end SJF entry point after returning computed scheduling metrics.
