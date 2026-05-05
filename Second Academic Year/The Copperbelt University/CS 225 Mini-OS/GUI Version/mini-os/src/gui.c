#include <stdio.h> // Provides file I/O helpers used for loading recent log lines into the dashboard.
#include <string.h> // Provides string formatting helpers used when copying log text into buffers.
#include "../include/gui.h" // Provides the dashboard constants, colors, and function declarations.
#include "../include/process.h" // Provides the global process table used by the process panel.
#include "../include/scheduler.h" // Provides scheduler-related types used by the dashboard controls.
#include "../include/memory.h" // Provides the global memory map used by the memory panel.

#define GUI_LOG_FILE "serc_log.txt" // Defines the dashboard log filename locally so the GUI does not depend on the logger enum names.

static Font dashboard_font = {0}; // Stores the monospace font used for terminal-style dashboard text.
static DashboardState dashboard_state = {0}; // Stores the current GUI state for the active dashboard session.

static void load_recent_logs(DashboardState *state) { // Loads the most recent log lines for the log panel.
	FILE *log_fp = NULL; // Holds the log file handle while reading recent entries.
	char recent_lines[20][256] = {{0}}; // Stores the newest 20 lines before they are copied into the GUI state.
	char line_buffer[256] = {0}; // Holds one log line at a time during file scanning.
	int line_count = 0; // Tracks how many lines were seen so the newest 20 can be preserved.
	int index = 0; // Tracks the output position while copying the buffered log lines.
	int start_index = 0; // Marks where the last 20 log lines begin inside the circular buffer.
	if (state == NULL) { // Prevents null state access when the caller passes an invalid pointer.
		return; // Exits early because there is no dashboard state to populate.
	} // Ends the null-state guard.
	log_fp = fopen(GUI_LOG_FILE, "r"); // Opens the persistent log file in read mode.
	if (log_fp == NULL) { // Handles missing or unreadable log files safely.
		state->log_count = 1; // Records one fallback line so the panel still shows useful text.
		snprintf(state->log_buffer[0], sizeof(state->log_buffer[0]), "Log file unavailable: %s", GUI_LOG_FILE); // Explains why the log panel cannot show live history.
		return; // Exits because there are no file lines to copy into the dashboard.
	} // Ends the file-open guard.
	while (fgets(line_buffer, sizeof(line_buffer), log_fp) != NULL) { // Reads the log file one line at a time.
		snprintf(recent_lines[line_count % 20], sizeof(recent_lines[0]), "%s", line_buffer); // Stores the current line in the circular history buffer.
		line_count++; // Increments the total line counter so the tail can be reconstructed later.
	} // Ends the file-reading loop at end-of-file.
	fclose(log_fp); // Closes the log file after capturing its content.
	state->log_count = 0; // Resets the visible log count before repopulating the GUI buffer.
	start_index = line_count > 20 ? line_count - 20 : 0; // Computes the first visible line for the log panel.
	for (index = 0; index < 20 && index < line_count; index++) { // Copies at most 20 of the most recent lines into the state.
		int source_index = (start_index + index) % 20; // Resolves the circular-buffer slot that holds the selected line.
		snprintf(state->log_buffer[index], sizeof(state->log_buffer[index]), "%s", recent_lines[source_index]); // Copies the selected line into the dashboard buffer.
		state->log_count++; // Tracks how many visible lines are now available for rendering.
	} // Ends the copy loop after the tail of the log file is loaded.
	if (state->log_count == 0) { // Handles the case where the log file was empty.
		snprintf(state->log_buffer[0], sizeof(state->log_buffer[0]), "Log file is currently empty."); // Supplies a friendly empty-state message.
		state->log_count = 1; // Ensures the panel still has one line to display.
	} // Ends the empty-log fallback branch.
} // Ends the log-loading helper.

static void draw_panel_frame(Rectangle bounds, Color tint) { // Draws a reusable panel container for dashboard sections.
	DrawRectangleRec(bounds, SERC_PANEL); // Fills the panel background with the requested dark theme color.
	DrawRectangleLinesEx(bounds, 2.0f, tint); // Draws a colored border so the active panel is visually clear.
} // Ends the panel-frame helper.

static void draw_text_line(Font font, const char *text, Vector2 position, float size, Color color) { // Draws a single line of dashboard text.
	if (text == NULL) { // Prevents invalid text pointers from reaching Raylib.
		return; // Exits because there is no text to render.
	} // Ends the null-text guard.
	DrawTextEx(font, text, position, size, 1.0f, color); // Renders the string using the loaded monospace font.
} // Ends the text helper.

void init_gui(void) { // Creates the Raylib window and prepares the dashboard state.
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SERC Mini-OS Dashboard"); // Opens the dashboard window with the requested title.
	SetTargetFPS(60); // Limits the render loop to 60 FPS for smooth but efficient updates.
	dashboard_font = LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 20, NULL, 0); // Loads a monospace font for terminal-style text.
	if (dashboard_font.texture.id == 0) { // Checks whether the first font load failed.
		dashboard_font = LoadFontEx("/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf", 20, NULL, 0); // Falls back to another common monospace font.
	} // Ends the primary font fallback branch.
	if (dashboard_font.texture.id == 0) { // Checks whether both external fonts failed to load.
		dashboard_font = GetFontDefault(); // Falls back to Raylib's built-in default font so text still appears.
	} // Ends the final font fallback branch.
	dashboard_state.selected_algorithm = 0; // Initializes the scheduler selection to FCFS.
	dashboard_state.selected_memory_fit = 0; // Initializes the memory-fit selection to First Fit.
	dashboard_state.quantum = 3; // Initializes the Round Robin quantum to the requested default value.
	dashboard_state.active_panel = 0; // Initializes panel focus to the default top-level state.
	dashboard_state.log_count = 0; // Clears the visible log count before loading data.
	dashboard_state.animation_timer = 0.0f; // Resets the animation timer for smooth motion updates.
	load_recent_logs(&dashboard_state); // Loads recent log lines so the dashboard starts with useful context.
} // Ends GUI initialization.

void close_gui(void) { // Releases GUI resources and closes the Raylib window.
	if (dashboard_font.texture.id != 0 && dashboard_font.texture.id != GetFontDefault().texture.id) { // Checks that the loaded font is not the built-in default font.
		UnloadFont(dashboard_font); // Frees the external monospace font resource.
	} // Ends the font-unload guard.
	CloseWindow(); // Closes the Raylib window and tears down the graphics context.
} // Ends GUI cleanup.

void draw_header(DashboardState *state) { // Draws the dashboard title banner and summary text.
	Rectangle header_bounds = {16.0f, 16.0f, SCREEN_WIDTH - 32.0f, 84.0f}; // Defines the full-width header area.
	if (state == NULL) { // Prevents null state access during header rendering.
		return; // Exits because the header depends on dashboard state.
	} // Ends the null-state guard.
	draw_panel_frame(header_bounds, SERC_BLUE); // Draws the header container with the primary accent color.
	draw_text_line(dashboard_font, "SERC Mini-OS Dashboard", (Vector2){30.0f, 28.0f}, 28.0f, SERC_TEXT); // Draws the main dashboard title.
	draw_text_line(dashboard_font, "Algorithms: FCFS / SJF / Priority / Round Robin | Memory: First Fit / Best Fit / Worst Fit", (Vector2){30.0f, 58.0f}, 18.0f, SERC_TEXT); // Shows the available control categories.
	DrawTextEx(dashboard_font, TextFormat("Quantum: %d", state->quantum), (Vector2){SCREEN_WIDTH - 180.0f, 32.0f}, 20.0f, 1.0f, SERC_YELLOW); // Shows the current Round Robin quantum.
} // Ends the header renderer.

void draw_process_panel(DashboardState *state) { // Draws the process overview panel using the live process table.
	Rectangle panel_bounds = {16.0f, 112.0f, 400.0f, 272.0f}; // Defines the left-side process area.
	int row = 0; // Tracks the visible process row during iteration.
	if (state == NULL) { // Prevents null state access during process rendering.
		return; // Exits because the process panel depends on dashboard state.
	} // Ends the null-state guard.
	draw_panel_frame(panel_bounds, state->active_panel == 1 ? SERC_GREEN : SERC_PANEL); // Highlights the panel when it is focused.
	draw_text_line(dashboard_font, "Process Overview", (Vector2){28.0f, 124.0f}, 22.0f, SERC_TEXT); // Draws the panel title.
	for (row = 0; row < MAX_PROCESSES && row < 6; row++) { // Limits the display to a few rows for readability.
		if (process_table[row].pid <= 0) { // Skips empty slots in the process table.
			continue; // Moves on to the next entry.
		} // Ends the empty-slot guard.
		DrawTextEx(dashboard_font, TextFormat("PID %d | %-22s | P:%d | R:%d", process_table[row].pid, process_table[row].name, process_table[row].priority, process_table[row].remaining_time), (Vector2){28.0f, 160.0f + (float)(row * 28)}, 16.0f, 1.0f, SERC_TEXT); // Renders one compact process summary line.
	} // Ends the process-table loop.
} // Ends the process panel renderer.

void draw_memory_panel(DashboardState *state) { // Draws the memory management panel and fit mode.
	Rectangle panel_bounds = {16.0f, 400.0f, 400.0f, 288.0f}; // Defines the lower-left memory area.
	const char *fit_label = "First Fit"; // Sets the default fit label.
	int index = 0; // Tracks the memory block row while iterating.
	if (state == NULL) { // Prevents null state access during memory rendering.
		return; // Exits because the memory panel depends on dashboard state.
	} // Ends the null-state guard.
	if (state->selected_memory_fit == 1) { // Checks whether Best Fit is selected.
		fit_label = "Best Fit"; // Updates the fit label.
	} else if (state->selected_memory_fit == 2) { // Checks whether Worst Fit is selected.
		fit_label = "Worst Fit"; // Updates the fit label.
	} // Ends the fit selection branch.
	draw_panel_frame(panel_bounds, state->active_panel == 2 ? SERC_BLUE : SERC_PANEL); // Highlights the panel when it is focused.
	draw_text_line(dashboard_font, "Memory Management", (Vector2){28.0f, 412.0f}, 22.0f, SERC_TEXT); // Draws the panel title.
	DrawTextEx(dashboard_font, TextFormat("Selected Fit: %s", fit_label), (Vector2){28.0f, 446.0f}, 18.0f, 1.0f, SERC_YELLOW); // Shows the current allocation strategy.
	for (index = 0; index < MAX_BLOCKS && index < 5; index++) { // Limits the memory summary to a few visible blocks.
		if (memory_map[index].size <= 0) { // Skips unused memory descriptors.
			continue; // Moves to the next memory block.
		} // Ends the inactive-block guard.
		DrawTextEx(dashboard_font, TextFormat("Block %02d | %3dMB | %s", memory_map[index].block_id, memory_map[index].size, memory_map[index].is_free ? "FREE" : "USED"), (Vector2){28.0f, 482.0f + (float)(index * 28)}, 16.0f, 1.0f, memory_map[index].is_free ? SERC_GREEN : SERC_RED); // Renders a simple free/used block summary line.
	} // Ends the memory-map loop.
} // Ends the memory panel renderer.

void draw_scheduler_panel(DashboardState *state) { // Draws the scheduler control panel and current algorithm selection.
	Rectangle panel_bounds = {432.0f, 112.0f, 392.0f, 272.0f}; // Defines the upper-middle scheduler area.
	const char *algorithm_label = "FCFS"; // Sets the default scheduler label.
	if (state == NULL) { // Prevents null state access during scheduler rendering.
		return; // Exits because the scheduler panel depends on dashboard state.
	} // Ends the null-state guard.
	if (state->selected_algorithm == 1) { // Checks whether SJF is selected.
		algorithm_label = "SJF"; // Updates the algorithm label.
	} else if (state->selected_algorithm == 2) { // Checks whether Priority Scheduling is selected.
		algorithm_label = "Priority"; // Updates the algorithm label.
	} else if (state->selected_algorithm == 3) { // Checks whether Round Robin is selected.
		algorithm_label = "Round Robin"; // Updates the algorithm label.
	} // Ends the algorithm-selection branch.
	draw_panel_frame(panel_bounds, state->active_panel == 3 ? SERC_YELLOW : SERC_PANEL); // Highlights the scheduler panel when it is focused.
	draw_text_line(dashboard_font, "CPU Scheduling", (Vector2){444.0f, 124.0f}, 22.0f, SERC_TEXT); // Draws the panel title.
	DrawTextEx(dashboard_font, TextFormat("Selected Algorithm: %s", algorithm_label), (Vector2){444.0f, 160.0f}, 18.0f, 1.0f, SERC_TEXT); // Shows the selected algorithm.
	DrawTextEx(dashboard_font, TextFormat("Round Robin Quantum: %d", state->quantum), (Vector2){444.0f, 194.0f}, 18.0f, 1.0f, SERC_TEXT); // Shows the current quantum.
	draw_text_line(dashboard_font, "Keys: 1=FCFS  2=SJF  3=Priority  4=RR  +/- Quantum", (Vector2){444.0f, 232.0f}, 16.0f, SERC_BLUE); // Describes the keyboard shortcuts.
} // Ends the scheduler panel renderer.

void draw_log_panel(DashboardState *state) { // Draws the recent log panel from the buffered log lines.
	Rectangle panel_bounds = {432.0f, 400.0f, 392.0f, 288.0f}; // Defines the lower-middle log area.
	int row = 0; // Tracks the visible log row during rendering.
	if (state == NULL) { // Prevents null state access during log rendering.
		return; // Exits because the log panel depends on dashboard state.
	} // Ends the null-state guard.
	draw_panel_frame(panel_bounds, state->active_panel == 4 ? SERC_RED : SERC_PANEL); // Highlights the log panel when it is focused.
	draw_text_line(dashboard_font, "Recent Logs", (Vector2){444.0f, 412.0f}, 22.0f, SERC_TEXT); // Draws the panel title.
	for (row = 0; row < state->log_count && row < 8; row++) { // Shows up to eight recent log lines.
		draw_text_line(dashboard_font, state->log_buffer[row], (Vector2){444.0f, 446.0f + (float)(row * 26)}, 16.0f, SERC_TEXT); // Renders one buffered log line.
	} // Ends the log-buffer loop.
} // Ends the log panel renderer.

void draw_resource_panel(DashboardState *state) { // Draws the right-side resource and navigation panel.
	Rectangle panel_bounds = {840.0f, 112.0f, 424.0f, 576.0f}; // Defines the tall right-side panel area.
	if (state == NULL) { // Prevents null state access during resource rendering.
		return; // Exits because the resource panel depends on dashboard state.
	} // Ends the null-state guard.
	draw_panel_frame(panel_bounds, state->active_panel == 5 ? SERC_BLUE : SERC_PANEL); // Highlights the resource panel when it is focused.
	draw_text_line(dashboard_font, "Resource & System Status", (Vector2){852.0f, 124.0f}, 22.0f, SERC_TEXT); // Draws the panel title.
	DrawTextEx(dashboard_font, TextFormat("Animation Timer: %.2f", state->animation_timer), (Vector2){852.0f, 160.0f}, 18.0f, 1.0f, SERC_TEXT); // Shows the timer used for smooth updates.
	draw_text_line(dashboard_font, "Keys: TAB cycle panels | ESC exit | Enter refresh logs", (Vector2){852.0f, 194.0f}, 16.0f, SERC_YELLOW); // Documents the main dashboard controls.
	draw_text_line(dashboard_font, "Dashboard panels summarize the current SERC simulation state.", (Vector2){852.0f, 232.0f}, 16.0f, SERC_TEXT); // Explains the dashboard purpose.
	draw_text_line(dashboard_font, "The console menu remains available after you close this window.", (Vector2){852.0f, 260.0f}, 16.0f, SERC_TEXT); // Reminds the user that CLI mode still exists.
	draw_text_line(dashboard_font, "Press 1-4 to change scheduler focus values.", (Vector2){852.0f, 306.0f}, 16.0f, SERC_GREEN); // Reinforces the scheduler keyboard controls.
	draw_text_line(dashboard_font, "Press 5-8 to switch focus between process, memory, log, and resource areas.", (Vector2){852.0f, 334.0f}, 16.0f, SERC_GREEN); // Reinforces the panel navigation controls.
	draw_text_line(dashboard_font, "This dashboard uses the live process table and memory map from the Mini-OS project.", (Vector2){852.0f, 380.0f}, 16.0f, SERC_TEXT); // States that the UI is tied to the underlying system model.
} // Ends the resource panel renderer.

void run_gui(void) { // Runs the main dashboard event loop until the window closes.
	init_gui(); // Ensures the Raylib window, font, and dashboard state are ready before drawing.
	while (WindowShouldClose() == 0) { // Keeps the dashboard alive until the user closes it or presses ESC.
		dashboard_state.animation_timer += GetFrameTime(); // Advances the animation timer using frame delta time.
		if (IsKeyPressed(KEY_TAB) != 0) { // Detects when the user wants to move focus to the next panel.
			dashboard_state.active_panel = (dashboard_state.active_panel + 1) % 6; // Cycles through the available focus targets.
		} // Ends the focus-rotation branch.
		if (IsKeyPressed(KEY_ONE) != 0) { // Detects the FCFS shortcut.
			dashboard_state.selected_algorithm = 0; // Selects FCFS as the active scheduler.
		} // Ends the FCFS branch.
		if (IsKeyPressed(KEY_TWO) != 0) { // Detects the SJF shortcut.
			dashboard_state.selected_algorithm = 1; // Selects SJF as the active scheduler.
		} // Ends the SJF branch.
		if (IsKeyPressed(KEY_THREE) != 0) { // Detects the Priority shortcut.
			dashboard_state.selected_algorithm = 2; // Selects Priority Scheduling as the active scheduler.
		} // Ends the priority branch.
		if (IsKeyPressed(KEY_FOUR) != 0) { // Detects the Round Robin shortcut.
			dashboard_state.selected_algorithm = 3; // Selects Round Robin as the active scheduler.
		} // Ends the round-robin branch.
		if (IsKeyPressed(KEY_EQUAL) != 0 || IsKeyPressed(KEY_KP_ADD) != 0) { // Detects keys that increase the quantum.
			dashboard_state.quantum++; // Increases the Round Robin time slice.
		} // Ends the quantum-increase branch.
		if ((IsKeyPressed(KEY_MINUS) != 0 || IsKeyPressed(KEY_KP_SUBTRACT) != 0) && dashboard_state.quantum > 1) { // Detects keys that decrease the quantum while keeping it positive.
			dashboard_state.quantum--; // Decreases the Round Robin time slice.
		} // Ends the quantum-decrease branch.
		if (IsKeyPressed(KEY_FIVE) != 0) { // Detects the process-panel shortcut.
			dashboard_state.active_panel = 1; // Focuses the process panel.
		} // Ends the process-focus branch.
		if (IsKeyPressed(KEY_SIX) != 0) { // Detects the memory-panel shortcut.
			dashboard_state.active_panel = 2; // Focuses the memory panel.
		} // Ends the memory-focus branch.
		if (IsKeyPressed(KEY_SEVEN) != 0) { // Detects the log-panel shortcut.
			dashboard_state.active_panel = 4; // Focuses the log panel.
		} // Ends the log-focus branch.
		if (IsKeyPressed(KEY_EIGHT) != 0) { // Detects the resource-panel shortcut.
			dashboard_state.active_panel = 5; // Focuses the resource panel.
		} // Ends the resource-focus branch.
		if (IsKeyPressed(KEY_ENTER) != 0) { // Detects the log-refresh shortcut.
			load_recent_logs(&dashboard_state); // Reloads the latest log lines so the dashboard stays current.
		} // Ends the log-refresh branch.
		BeginDrawing(); // Starts the Raylib drawing pass for the current frame.
		ClearBackground(SERC_DARK); // Paints the background with the requested SERC theme color.
		draw_header(&dashboard_state); // Draws the shared banner at the top of the window.
		draw_process_panel(&dashboard_state); // Draws the process overview panel.
		draw_scheduler_panel(&dashboard_state); // Draws the scheduler control panel.
		draw_memory_panel(&dashboard_state); // Draws the memory panel.
		draw_log_panel(&dashboard_state); // Draws the log panel.
		draw_resource_panel(&dashboard_state); // Draws the resource and navigation panel.
		EndDrawing(); // Ends the Raylib drawing pass so the frame can be presented.
	} // Ends the GUI loop after the user closes the window.
	close_gui(); // Releases GUI resources after the window loop terminates.
} // Ends the dashboard loop.
