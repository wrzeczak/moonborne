#pragma once

#include "render.h"
#include "person.h"

//------------------------------------------------------------------------------

void vcp_print_attr(WINDOW * info, int height, const char * label, std::string value); // during vcp, print an attirbute to the info window
std::string vcp_get_attr(WINDOW * interact, int height, const char * query_msg); // during vcp, get an attribute from the interact window's input
Person visual_create_person(WINDOW * header, WINDOW * info, WINDOW * interact); // create a person using the TUI

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

	name_t default_output_name = NAME_ERROR;
	int default_output_age = 127;

	output = (Person) { default_output_name, default_output_age };

	//------------------------------------------------------------------------------

	mvwprintw(header, 2, 2, "Character Creation -- Following the prompts in the interact window, add information to create your character!");

	wrefresh(header);

	//------------------------------------------------------------------------------

	int height_iterator = 2;

	__vcp_get_name:
	std::string name_str = vcp_get_attr(interact, height_iterator, "Enter your character's name.");
	name_t name = process_name(name_str);

	if(name.first == NAME_ERROR.first) goto __vcp_get_name;

	vcp_print_attr(info, height_iterator, "NAME", name.full);

	height_iterator++;

	__vcp_get_age:
	std::string age_str = vcp_get_attr(interact, height_iterator, "How old is your character? [35-80]");

	int age;
	try {
		age = std::stoi(age_str);
	} catch(const std::exception& e) {
		goto __vcp_get_age;
	}

	if(age < 35 || age > 80) goto __vcp_get_age;

	vcp_print_attr(info, height_iterator, "AGE", age_str);

	height_iterator++;

	clear_prompt(interact);

	header_message(header, "PROCEED", "Press any key!");
	mvwprintw(interact, height_iterator, 0, "# Not exactly a spry young buck anymore, are we %s...", name.first.c_str());

	height_iterator++;
	interact_hline(interact, height_iterator);
	height_iterator++;

	getch();

	vcp_print_attr(info, height_iterator, "FIRST", name.first);
	height_iterator++;
	if(name.has_middle) {
		vcp_print_attr(info, height_iterator, "MIDDLE", name.middle);
		height_iterator++;
	}
	vcp_print_attr(info, height_iterator, "LAST", name.last);
	height_iterator += 2;

	vcp_print_attr(info, height_iterator, "FIRST INITIAL", std::string(1, name.fi));
	height_iterator++;
	if(name.has_middle) {
		vcp_print_attr(info, height_iterator, "MIDDLE INITIAL", std::string(1, name.mi));
		height_iterator++;
	}
	vcp_print_attr(info, height_iterator, "LAST INITIAL", std::string(1, name.li));

	getch();

	output.name = name;
	output.age = age;

	return output;
}

