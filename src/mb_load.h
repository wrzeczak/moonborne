//------------------------------------------------------------------------------
// moonborne loading functions
//------------------------------------------------------------------------------

#pragma once

#include <toml.h>
#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------------------------------
// LOADMAP FUNCTIONS
//------------------------------------------------------------------------------

typedef struct {
	int width, height;
	ivec map; // a 1-d array that when arranged w*h displays a grid of tiles
	ivec req; // an unordered list of all tile ids used in this map
} loadmap_return_t;

void loadmap_error(const char * msg, const char * errbuf) {
	fprintf(stderr, "INFO: MAP ERROR: %s%s\n", msg, errbuf ? errbuf : "");
	exit(1);
}

// takes a path to a toml file
loadmap_return_t load_map(char * path) {
	/*
	[info]
	width, height (int)

	[data]
	map (array of ints)
	*/

	//--- load the toml file ---
	FILE * fp = fopen(path, "r");

	char errbuf[200];
	if(!fp) loadmap_error("Couldn't open map file - ", errbuf);

	toml_table_t * root = toml_parse_file(fp, errbuf, sizeof(errbuf));

	fclose(fp);

	if(!root) loadmap_error("Cannot parse map file - ", errbuf);

	//--- info table ---
	toml_table_t * info_table = toml_table_in(root, "info");
	if(!info_table) loadmap_error("Missing [info] in map file - ", "");

	toml_datum_t width_raw = toml_int_in(info_table, "width");
	if(!width_raw.ok) loadmap_error("Cannot read info.width.", "");

	toml_datum_t height_raw = toml_int_in(info_table, "height");
	if(!height_raw.ok) loadmap_error("Cannot read info.height.", "");

	int width = (int) width_raw.u.i;
	int height = (int) height_raw.u.i;

	printf("INFO: MAP: Loaded map info data WIDTH: %d, HEIGHT %d\n", width, height);

	//--- data table ---
	toml_table_t * data = toml_table_in(root, "data");
	if(!data) loadmap_error("Missing [data] in map file!", "");

	ivec map = { 0 };
	ivec req = { 0 };

	toml_array_t * map_raw = toml_array_in(data, "map");
	toml_array_t * req_raw = toml_array_in(data, "req");

	for(int i = 0; i < width * height; i++) {
		toml_datum_t map_bit = toml_int_at(map_raw, i);
		toml_datum_t req_bit = toml_int_at(req_raw, i);
		if(!map_bit.ok) loadmap_error("Map data conversion (tmldt -> ivec) failed!", "");

		ivec_push(&map, map_bit.u.i);

		if(req_bit.ok) ivec_push(&req, req_bit.u.i);
	}

	if(width * height != (int) ivec_size(&map)) loadmap_error("Given width, height do not match data.map size!", "");

	printf("INFO: MAP: Loaded `map` from `map_raw`! LENGTH: %d\n", (int) ivec_size(&map));
	printf("INFO: MAP: Loaded `req` from `req_raw`! LENGTH: %d\n", (int) ivec_size(&req));

	// done with toml
	toml_free(root);

	// do output
	loadmap_return_t output;

	output.width = width;
	output.height = height;

	ivec_copy(&output.map, &map);
	ivec_copy(&output.req, &req);

	return output;
}

//------------------------------------------------------------------------------

