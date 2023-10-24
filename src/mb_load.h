//------------------------------------------------------------------------------
// moonborne loading functions
//------------------------------------------------------------------------------

#pragma once

#include <toml.h>
#include <stdlib.h>
#include <stdio.h>

#define i_implement
#include "mb_ivec.h"

//------------------------------------------------------------------------------
// LOADMAP FUNCTIONS
//------------------------------------------------------------------------------

typedef struct {
	int width, height;
	ivec map; // a 1-d array that when arranged w*h displays a grid of tiles
	ivec req; // an unordered list of all tile ids used in this map
} loadmap_return_t;

void loadmap_error(const char * msg, const char * errbuf);
loadmap_return_t load_map(char * path);

