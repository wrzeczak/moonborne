#include "mb_load.h"

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

	loadmap_return_t output;

	//--- info table ---
	toml_table_t * info = toml_table_in(root, "info");
	if(!info) loadmap_error("Missing [info] in map file - ", "");

	toml_datum_t width_raw = toml_int_in(info, "width");
	if(!width_raw.ok) loadmap_error("Cannot read info.width.", "");

	toml_datum_t height_raw = toml_int_in(info, "height");
	if(!height_raw.ok) loadmap_error("Cannot read info.height.", "");

	int width = (int) width_raw.u.i;
	int height = (int) height_raw.u.i;

	output.width = width;
	output.height = height;

	//--- data table ---
	toml_table_t * data = toml_table_in(root, "data");
	if(!data) loadmap_error("Missing [data] in map file!", "");

	ivec map = { 0 };

	toml_array_t * map_raw = toml_array_in(data, "map");
	for(int i = 0; i < width * height; i++) {
		toml_datum_t bit_raw = toml_int_in(map_raw, i);
		if(!bit_raw.ok) loadmap_error("Map data conversion (tmldt -> ivec) failed!", "");

		ivec_push(&map, bit_raw.u.i);
	}

	if(width * height != (int) ivec_size(&map)) loadmap_error("Given width, height do not match data.map size!", "");
	output.map = map;

	ivec req = { 0 };

	for(int i = 0; i < width * height; i++) {
		bool unique = true;
		int bit = (int) *(ivec_at(&map, i));

		for(int j = 0; j < (int) ivec_size(&req); j++) {
			int check = (int) *(ivec_at(&req, j));
			if(check == bit) unique = false;
		}

		if(unique) ivec_push(&req, bit);
	}

	output.req = req;

	toml_free(root);
	return output;
}

