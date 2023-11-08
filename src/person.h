//----------------------------------------------------------------------------//
// person -- functions and structures that deal with people and data manage-  //
// ment for people.                                                           //
//----------------------------------------------------------------------------//

#pragma once
#include <stdio.h> // used for debugging, not for player input/output
#include <stdlib.h>
#include <vector>
#include <string>

//------------------------------------------------------------------------------

typedef struct {
	char * name;
	int age;
} Person;

//------------------------------------------------------------------------------

Person create_person(char * name, int age);
Person create_random_person();

//------------------------------------------------------------------------------

Person create_person(char * name, int age) {
	Person output;

	output.name = name;
	output.age = age;

	return output;
}

//------------------------------------------------------------------------------

Person create_random_person() {
	char * name = (char *) "John Audie Smith";
	int age = (rand() % 50) + 35;

	return create_person(name, age);
}

//------------------------------------------------------------------------------

