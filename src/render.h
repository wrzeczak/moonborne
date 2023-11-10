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

Person visual_create_person(WINDOW * header, WINDOW * info, WINDOW * interact); // create a person using the TUI

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

	mvwprintw(interact, height - 2, 0, crossbar.c_str());
	mvwprintw(interact, height - 1, 0, "?>");
	mvwprintw(interact, height, 0, crossbar.c_str());

	wmove(interact, height - 1, 3);

	wrefresh(interact);
}

//------------------------------------------------------------------------------

Person visual_create_person(WINDOW * header, WINDOW * info, WINDOW * interact) {
	Person output;

	std::string default_output_name = "MISSINGNO";
	int default_output_age = 127;

	output = (Person) { default_output_name, default_output_age };

	//------------------------------------------------------------------------------

	mvwprintw(header, 2, 2, "Character Creation -- Following the prompts in the interact window, add information to create your character!");

	wrefresh(header);

	//------------------------------------------------------------------------------

	int height_iterator = 2;

	char name_buffer[256];

	mvwprintw(interact, height_iterator, 0, "- Enter character name.");
	wrefresh(interact);

	prompt(interact);

	echo();
	mvwgetstr(interact, LINES - 7, 3, name_buffer);
	noecho();

	int name_len = strlen(name_buffer);
	name_buffer[name_len] = '\0';
	std::string name = name_buffer;
	output.name = name;

	wattron(info, A_BOLD);
	mvwprintw(info, height_iterator, 2, "NAME:");
	wattroff(info, A_BOLD);
	mvwprintw(info, height_iterator, 8, "%s", name.c_str());
	wrefresh(info);

	getch();

	return output;
}

