//----------------------------------------------------------------------------//
// text-interface -- functions for dealing with the text interface and disp-  //
// laying data to the player.                                                 //
//----------------------------------------------------------------------------//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h> // for screen_width()
#include <unistd.h> // for screen_width(), sleep()

#include <vector>
#include <string>

#include "person.h"

//------------------------------------------------------------------------------

#define VERSION 0.01
#define VERSION_STR "0.01"

#define QUERY_LEN_MAX 64

#define BOLD_ON "\e[1m"
#define BOLD_OFF "\e[m"

//------------------------------------------------------------------------------

typedef struct {
	int length;
	char * input;
} input_t;

//------------------------------------------------------------------------------

void clear(); // clears the screen, wrapped in a function so it can be portable
void prompt(); // generic input prompt, might be configurable
void prompt(const char * query); // ask a question of the player, then prompt
input_t input(int max_length); // read text input and return the string
int screen_width(); // get the width of the terminal

void start_screen(); // display the starting screen

void print_daily_heading();

void infobox_padded_print(const char * input, int infobox_width, char padding_char); // used for the person infobox, prints a formatted line
void print_person(Person p, int align); // prints personal data in a nice format

Person interactive_create_person(); // this really could be in person.h, but i think that since this will be putting out a lot of text it's better to keep it here and just be okay with it returning something

//------------------------------------------------------------------------------

// TODO: make it portable!
void clear() {
	system("clear");
}

void prompt() {
	printf("?> ");
}

void prompt(const char * query) {
	if(strlen(query) > QUERY_LEN_MAX) printf("WARNING: Query %s too long (length: %d)!\n", query, strlen(query));
	printf("%s ?> ", query);
}

input_t input(int max_length = 256) {
	char input_buffer[max_length];
	memset(input_buffer, 0, max_length);
	fflush(stdin);

	char * fgets_error = fgets(input_buffer, max_length, stdin);

	if(fgets_error == NULL) {
		printf("ERROR: fgets() failed! [%d]\n", ferror(stdin));
		exit(1); // TODO: handle this error in a better way
	}

	int raw_command_length = strlen(input_buffer);

	char command[raw_command_length];

	strncpy(command, input_buffer, raw_command_length - 1);
	command[raw_command_length - 1] = '\0';

	input_t output = (input_t) { // isn't this naming scheme just perfectly ironic
		raw_command_length,
		command
	};

	return output;
}

int screen_width() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_col;
}

//------------------------------------------------------------------------------

void print_daily_heading(int day, Person player) {
	std::string crossbar(screen_width(), '=');

	char info[screen_width()];

	snprintf(info, screen_width(), "= DAY %d -- %s ", day, player.name);
	int info_width = strlen(info);

	std::string info_crossbar(info);
	std::string info_crossbar_padding(screen_width() - info_width, '=');

	info_crossbar += info_crossbar_padding;

	printf("%s\n%s\n%s\n", crossbar.c_str(), info_crossbar.c_str(), crossbar.c_str());
}

//------------------------------------------------------------------------------

void start_screen(int load_time) {
	clear();

	int width = screen_width();

	char title[width + 1]; // if we're printing anything longer, something has gone horribly wrong

	snprintf(title, width, "Welcome to Moonborne Polsim -- Version %s ", VERSION_STR);

	std::string title_padding((width - 8 - strlen(title)), '=');

	printf("=#= " BOLD_ON "%s" BOLD_OFF " =#=%s\n", title, title_padding.c_str());

	sleep(load_time);
}

//------------------------------------------------------------------------------

enum {
	LEFT,
	CENTER,
	RIGHT
};

//------------------------------------------------------------------------------


// NOTE: `input` should be already formatted text eg. "AGE: 52" -- this does not do printf() stuff for you!
void infobox_padded_print(const char * input, int infobox_width, int align, char padding_char = ' ') {
	int input_width = strlen(input); // TODO: verify that strlen() is safe
	if(input_width > (infobox_width - 4)) {
		printf("ERROR: Bad `infobox_padded_print()` call -- input too wide!");
		exit(1);
	}

	std::string external_padding;

	if(align == RIGHT) {
		external_padding = std::string((screen_width() - infobox_width), ' ');
	} else if(align == CENTER) {
		external_padding = std::string((screen_width() / 2 - infobox_width / 2), ' ');
	}

	std::string internal_padding((infobox_width - 5 - input_width), padding_char);
	printf("%s# %s %s #\n", (align == LEFT) ? "" : external_padding.c_str(), input, internal_padding.c_str());
}

void print_person(Person p, int align = LEFT) {
	int infobox_width = 40;

	std::string external_padding;

	if(align == RIGHT) {
		external_padding = std::string((screen_width() - infobox_width), ' ');
	} else if(align == CENTER) {
		external_padding = std::string((screen_width() / 2 - infobox_width / 2), ' ');
	}

	// the magical number six comes from '#==' (3) + 2 spaces around the name + 1 final '#' -- this formats the box to be `infobox_width` characters wide
	std::string infobox_header_padding((infobox_width - 6 - strlen(p.name)), '=');

	printf("%s#== %s %s#\n", (align == LEFT) ? "" : external_padding.c_str(), p.name, infobox_header_padding.c_str());

	char age_str[infobox_width]; // if we're trying to put anything bigger than infobox_width in, there's a problem anyways. really should be ibw - 4...
	snprintf(age_str, infobox_width, "AGE: %d", p.age);

	infobox_padded_print(age_str, infobox_width, align);

	std::string box_bottom_padding((infobox_width - 2), '=');

	printf("%s#%s#\n", (align == LEFT) ? "" : external_padding.c_str(), box_bottom_padding.c_str());


}

//------------------------------------------------------------------------------

Person interactive_create_person() {
	int name_length_max = 64;
	char name[name_length_max];
	memset(name, 0, name_length_max);

	fflush(stdin);

	prompt("Your character's name? [64]");

	input_t name_input = input();

	printf("INFO: Name recieved: %s, length: %d\n", name_input.input, name_input.length);

	sleep(3);

	Person output;
	memset(output.name, 0, name_length_max);

	strncpy(output.name, name_input.input, name_input.length);

	output.age = 36;

	return output;
}

//------------------------------------------------------------------------------

