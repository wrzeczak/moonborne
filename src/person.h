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

//------------------------------------------------------------------------------

typedef struct {
	std::string name;
	int age;
} Person;

//------------------------------------------------------------------------------

Person create_person(std::string name, int age);
Person create_random_person();

//------------------------------------------------------------------------------

Person create_person(std::string name, int age) {
	Person output;

	output.name = name;

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

