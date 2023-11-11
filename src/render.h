//----------------------------------------------------------------------------//
// render.h -- functions for rendering boxes, text, and debug data onto the   //
// screen, as well as handling visual input.                                  //
//----------------------------------------------------------------------------//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#include <sys/ioctl.h>
#include <unistd.h>

#include <vector>
#include <string>

#include "person.h"

//------------------------------------------------------------------------------

// constant definitions

//------------------------------------------------------------------------------ '

// type definitions

//------------------------------------------------------------------------------

// function declarations

void visual_init(); // initialize ncurses ie. all visual functionality
void window_deinit(WINDOW * win); // generically delete a window

int interact_width(); // return the width of the interact window
int info_width(); // return the width of the info window

void prompt(WINDOW * interact); // display a little prompt message at the bottom of the interact window
void clear_prompt(WINDOW * interact); // clear the prompt box from the screen

void interact_hline(WINDOW * interact, int height); // draw a horizontal line in the interact window

void header_message(WINDOW * header, const char * title, const char * info); // display a message in the header
//TODO: make aliases with std::string as well

//------------------------------------------------------------------------------

// initialize ncurses ie. all visual functionality
void visual_init() {
	initscr();
	raw(); // TODO: do we actually need this?
	noecho(); // NOTE: all input functions likely need to toggle on and then toggle off echoing
	keypad(stdscr, true);

	refresh();
}

// generically delete a window
void window_deinit(WINDOW * win) {
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
}

//------------------------------------------------------------------------------

int interact_width() {
	return (int) ((float) COLS * 0.7f);
}

int info_width() {
	return COLS - interact_width();
}

//------------------------------------------------------------------------------

// display a little prompt message the bottom of the interact window
void prompt(WINDOW * interact) {
	int height = LINES - 6;

	std::string crossbar_padding(interact_width() - 2, '=');
	std::string crossbar = '#' + crossbar_padding + '#';

	std::string input_field_padding(interact_width() - 2, ' ');
	std::string input_field = '#' + input_field_padding + '#';

	mvwprintw(interact, height - 2, 0, crossbar.c_str());
	mvwprintw(interact, height - 1, 0, input_field.c_str()); // clear the input field
	mvwprintw(interact, height - 1, 0, "?>");
	mvwprintw(interact, height, 0, crossbar.c_str());

	wmove(interact, height - 1, 3);

	wrefresh(interact);
}

void clear_prompt(WINDOW * interact) {
	int height = LINES - 6;

	std::string its_only_emptiness_on_the_inside(interact_width(), ' ');

	mvwprintw(interact, height - 2, 0, its_only_emptiness_on_the_inside.c_str());
	mvwprintw(interact, height - 1, 0, its_only_emptiness_on_the_inside.c_str());
	mvwprintw(interact, height, 0, its_only_emptiness_on_the_inside.c_str());

	wrefresh(interact);
}

//------------------------------------------------------------------------------

// draw a horizontal line in the interact window
void interact_hline(WINDOW * interact, int height) {
	std::string line_padding(interact_width() - 4, '-');
	std::string line = "  " + line_padding + "  ";

	mvwprintw(interact, height, 0, "%s", line.c_str());
	wrefresh(interact);
}

//------------------------------------------------------------------------------

// display a message in the header
void header_message(WINDOW * header, const char * title, const char * info) {
	std::string header_padding(COLS - 2, ' ');
	std::string no_man_can_cure_my_illness_now_for_the_LORD_hath_forsaken_me = '#' + header_padding + '#';

	mvwprintw(header, 2, 0, "%s", no_man_can_cure_my_illness_now_for_the_LORD_hath_forsaken_me.c_str());
	mvwprintw(header, 2, 2, "%s -- %s", title, info);
	wrefresh(header);
}

