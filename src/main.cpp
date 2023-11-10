#include "person.h"
#include "render.h"

//------------------------------------------------------------------------------

// init() functions

bool init() {
	visual_init();
	return true;
}

//------------------------------------------------------------------------------

int main(void) {
	//------------------------------------------------------------------------------

	// initialization

	init();

	/* SCREEN LAYOUT
	#---------------------------#	Header will display constant, global information
	|========== header =========|	such as date, basic player stats, and some cont-
	|---------------------------|	ext sensitive information depending on state.
	|=================|=========|
	|=================|=========|	Info will display info either by request or auto.
	|==== interact ===|== info =|
	|=================|=========|	Interact will be where the player inputs infor-
	|=================|=========|	mation and recieves direct responses, ie. dia-
	#---------------------------#	logue, commands, etc.
	*/

	WINDOW * header;
	WINDOW * info;
	WINDOW * interact;

	header = newwin(5, COLS, 0, 0);
	wborder(header, '#', '#', '=', '=', '#', '#', '#', '#');
	wrefresh(header);

	info = newwin(LINES - 5, info_width(), 5, interact_width());
	wborder(info, '|', ' ', ' ', ' ', '|', ' ', '|', ' ');
	wrefresh(info);

	interact = newwin(LINES - 5, interact_width(), 5, 0);
	wborder(interact, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(interact);

	//------------------------------------------------------------------------------

	// variables

	int day_counter = 0;

	bool game_should_close = false;
	bool day_should_proceed = false;

	Person player;

	//------------------------------------------------------------------------------

	// game loop

	refresh();

	/*
	while(!game_should_close) {
		day_counter++;

		day_should_proceed = false;

		while(!day_should_proceed) {
			// ...
			getch();
			refresh();

		}
	}
	*/

	wrefresh(interact);
	wrefresh(info);
	wrefresh(header);
	refresh();

	prompt(interact);

	player = visual_create_person(header, info, interact);

	getch();

	//------------------------------------------------------------------------------

	// de-init

	//------------------------------------------------------------------------------

	window_deinit(header);
	window_deinit(info);
	window_deinit(interact);

	endwin();
	return 0;
}
