//------------------------------------------------------------------------------//
// party -- functions and structures describing parties and their interactions  //
// in political realms. parties are the fundamental unit of politics outside of //
// the player character, and will act as more-or-less unified blocs.            //
//------------------------------------------------------------------------------//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // the joys of C++

#include <vector>
#include <string>

#include "person.h"
#include "party_data.h"

//------------------------------------------------------------------------------

typedef struct {
	Person leader;

	std::vector<std::string> electable_representatives; // NOTE: leader.full = electable_representatives[0]
	int voter_base;

	std::string name;
	std::string full_name;
	std::string abbreviation;

	int general_affiliation;

	politics_t pol;
} Party;

//------------------------------------------------------------------------------

