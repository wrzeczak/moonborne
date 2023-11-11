#pragma once

#include "render.h"
#include "person.h"

//------------------------------------------------------------------------------

void vcp_print_attr(WINDOW * info, int height, const char * label, std::string value); // during vcp, print an attirbute to the info window
std::string vcp_get_attr(WINDOW * interact, int height, const char * query_msg); // during vcp, get an attribute from the interact window's input
float vcp_political_slider(WINDOW * interact, int height, const char * label); // display a slider which can be moved around by the player to get a float value between 0.0f and 1.0f

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

// slider label -- economic, value label -- equality
float vcp_political_slider(WINDOW * interact, WINDOW * header, int height, const char * slider_label, const char * value_label) {
	mvwprintw(interact, height - 1, 0, "- SLIDER: %s -- Input your %s from your 8values test.", slider_label, value_label);
	header_message(header, "SLIDER", "Use the arrow keys or the h and l keys to move the slider from side to side, then hit ENTER to submit.");

	wrefresh(interact);

	bool slider_should_return = false;
	int slider_value = 10; // slider operates on a scale of 20 characters, 10 gradations, 2 characters per gradation

	std::string slider_padding(22, '-');
	std::string slider_cover = '*' + slider_padding + '*';

	mvwprintw(interact, height, 2, "%s", slider_cover.c_str());
	mvwprintw(interact, height + 1, 2, "| ##########---------- |");
	mvwprintw(interact, height + 2, 2, "%s", slider_cover.c_str());

	wrefresh(interact);

	while(!slider_should_return) {
		int ch = getch();

		if((ch == KEY_LEFT || ch == (int) 'h') && slider_value > 0) slider_value -= 2;
		if((ch == KEY_RIGHT || ch == (int) 'l') && slider_value < 20) slider_value += 2;
		if((ch == (int) '\n')) {
			slider_should_return = true;
			continue;
		}

		std::string value, space;
		if(slider_value > 0) value = std::string(slider_value, '#');
		else value = "";

		if(slider_value < 20) space = std::string(20 - slider_value, '-');
		else space = "";

		mvwprintw(interact, height + 1, 2, "| %s%s |", value.c_str(), space.c_str());

		wrefresh(interact);
	}

	float value = (float) slider_value / 20.0f;

	mvwprintw(interact, height + 3, 0, "- %s slider value saved.", slider_label);
	wrefresh(interact);
	return value;
}

// create a person using the TUI
Person visual_create_person(WINDOW * header, WINDOW * info, WINDOW * interact) {
	Person output;

	name_t default_output_name = NAME_ERROR;
	int default_output_age = 127;

	output = (Person) { default_output_name, default_output_age };

	//------------------------------------------------------------------------------

	mvwprintw(header, 2, 2, "Character Creation -- Following the prompts in the interact window, add information to create your character, press ENTER to proceed.");

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

	height_iterator = 8;

	float economic = vcp_political_slider(interact, header, height_iterator, "ECONOMIC", "EQUALITY");
	height_iterator += 5;
	float diplomatic = vcp_political_slider(interact, header, height_iterator, "DIPLOMATIC", "NATION");
	height_iterator += 5;
	float civil = vcp_political_slider(interact, header, height_iterator, "CIVIL", "LIBERTY");
	height_iterator += 5;
	float society = vcp_political_slider(interact, header, height_iterator, "SOCIETAL", "TRADITION");
	height_iterator += 5;

	getch();

	height_iterator -= 15;

	vcp_print_attr(info, height_iterator + 0, "ECONOMIC", std::to_string(economic));
	vcp_print_attr(info, height_iterator + 2, "DIPLOMATIC", std::to_string(diplomatic));
	vcp_print_attr(info, height_iterator + 4, "CIVIL", std::to_string(civil));
	vcp_print_attr(info, height_iterator + 6, "SOCIETAL", std::to_string(society));

	getch();

	output.name = name;
	output.age = age;

	output.pol = (politics_t) {
		economic,
		diplomatic,
		civil,
		society
	};

	return output;
}

