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

void vcp_print_attr(WINDOW * info, int height, const char * label, std::string value); // during vcp, print an attirbute to the info window
std::string vcp_get_attr(WINDOW * interact, int height, const char * query_msg); // during vcp, get an attribute from the interact window's input
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

	std::string input_field_padding(interact_width() - 2, ' ');
	std::string input_field = '#' + input_field_padding + '#';

	mvwprintw(interact, height - 2, 0, crossbar.c_str());
	mvwprintw(interact, height - 1, 0, input_field.c_str()); // clear the input field
	mvwprintw(interact, height - 1, 0, "?>");
	mvwprintw(interact, height, 0, crossbar.c_str());

	wmove(interact, height - 1, 3);

	wrefresh(interact);
}

//------------------------------------------------------------------------------

// during vcp, print an attirbute to the info window
void vcp_print_attr(WINDOW * info, int height, const char * label, std::string value) {
	wattron(info, A_BOLD);
	mvwprintw(info, height, 2, "%s: ", label);
	wattroff(info, A_BOLD);
	mvwprintw(info, height, 4 + strlen(label), "%s", value.c_str());
	wrefresh(info);
}

// during vcp, get an attribute from the interact window's input
std::string vcp_get_attr(WINDOW * interact, int height, const char * query_msg) {
	char input_buffer[1024];

	mvwprintw(interact, height, 0, "- %s", query_msg);
	wrefresh(interact);

	prompt(interact);

	echo();
	mvwgetstr(interact, LINES - 7, 3, input_buffer);
	noecho();

	int input_len = strlen(input_buffer);
	input_buffer[input_len] = '\0';
	std::string input = input_buffer;

	return input;
}

// create a person using the TUI
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

	std::string name = vcp_get_attr(interact, height_iterator, "Enter your character's name.");
	vcp_print_attr(info, height_iterator, "NAME", name);

	height_iterator++;

	__vcp_get_age:
	std::string age_str = vcp_get_attr(interact, height_iterator, "How old is your character? [35-80]");

	int age;
	try {
		age = std::stoi(age_str);
	} catch(const std::exception& e) {
		goto __vcp_get_age;
	}

	if(age < 35 || age > 80) {
		mvwprintw(interact, LINES - 7, 3, "INCORRECT INPUT: Age was not within defined range, or was not a valid number!");
		goto __vcp_get_age;
	}

	vcp_print_attr(info, height_iterator, "AGE", age_str);

	height_iterator++;

	getch();

	output.name = name;
	output.age = age;

	return output;
}

