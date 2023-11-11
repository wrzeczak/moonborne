//----------------------------------------------------------------------------//
// person -- functions and structures that deal with people and data manage-  //
// ment for people.                                                           //
//----------------------------------------------------------------------------//

#pragma once
#include <stdio.h> // used for debugging, not for player input/output
#include <stdlib.h>
#include <string.h> // the joys of C++

#include <vector>
#include <string>
#include <sstream>

//------------------------------------------------------------------------------

typedef struct {
	std::string full; 		// full name ie. "John Albert Ridley Jackson"
	std::string first;		// "John"
	bool has_middle;		// if false, no middle name was provided
	std::string middle;		// "Albert Ridley," "MISSINGNO" if not provided
	std::string last;		// "Jackson"
	// first name initial, middle name initial, etc.
	char fi;				// 'J'
	char mi; 				// 'A,' or '%' if not provided
	char li;				// 'J'
} name_t;

name_t NAME_ERROR = (name_t) {
	std::string("MISSINGNO"),
	std::string("MISSINGNO"),
	false,
	std::string("MISSINGNO"),
	std::string("MISSINGNO"),
	'%',
	'%',
	'%'
};

typedef struct {
	name_t name;
	int age;
} Person;

//------------------------------------------------------------------------------

name_t process_name(std::string name); // turn a string ie. "John Jack Blount" into a `name_t`

Person create_person(std::string name, int age);

Person create_random_person();

//------------------------------------------------------------------------------

// turn a string ie. "John Jack Blount" into a `name_t`
name_t process_name(std::string name) { //TODO: function error checking, currently assumes a name with at least one space, make function better, yadda yadda
	std::stringstream name_ss(name);

	std::string segment;
	std::vector<std::string> segments;

	while(std::getline(name_ss, segment, ' ')) segments.push_back(segment);

	name_t output = NAME_ERROR;

	if(segments.size() < 2) return NAME_ERROR;

	if(segments.size() < 3) output.has_middle = false;
	else output.has_middle = true;

	std::string middle;

	if(output.has_middle) {
		if(segments.size() > 3) {
			for(int i = 1; i < (segments.size() - 1); i++) {
				middle += segments[i] + ' ';
			}

			int len = middle.length();
			middle.resize(len - 1); // remove the trailing space character
		} else {
			middle = segments[1];
		}
	}

	output.full = name;
	output.first = segments[0];
	if(output.has_middle) output.middle = middle; // otherwise, default from NAME_ERROR
	output.last = segments[segments.size() - 1];

	output.fi = segments[0][0];
	if(output.has_middle) output.mi = middle[0]; // otherwise, default from NAME_ERROR
	output.li = segments[segments.size() - 1][0];

	return output;
}

//------------------------------------------------------------------------------

Person create_person(std::string name, int age) {
	Person output;

	output.name = process_name(name);

	output.age = age;

	return output;
}

//------------------------------------------------------------------------------

Person create_random_person() {
	std::string name = "John Audie Smith";
	int age = (rand() % 50) + 35;

	return create_person(name, age);
}

//------------------------------------------------------------------------------

