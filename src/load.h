//------------------------------------------------------------------------------
// moonborne loading function -- now in 3-D!
//------------------------------------------------------------------------------

#pragma once

#include <toml.hpp>
#include <stdlib.h>
#include <stdio.h>

#include <raylib.h>

#include <vector>
#include <string>
#include <algorithm> // for std::find

typedef std::vector<int> ivec;     // integer vector
typedef std::vector<Vector2> cvec; // coordinate vector

//------------------------------------------------------------------------------

void load_error(const char * msg, const char * errbuf) {
	fprintf(stderr, "INFO: ERROR: %s%s\n", msg, errbuf);
	exit(1);
}

// now that we're in C++ we can overload functions :jeb:
void load_error(const char * msg) {
	fprintf(stderr, "INFO: ERROR: %s\n", msg);
	exit(1);
}

//------------------------------------------------------------------------------
// LOADMAP and related functions
//------------------------------------------------------------------------------

typedef struct {
	int width, height;
	int map_size, req_size;
	ivec map; // 1-d array of all tiles expressed in the form of tile ids
	ivec req; // ordered list of all tile ids requested by the map in question
} loadmap_return_t;

// takes a path to a toml file and outputs the map stored in that file
loadmap_return_t load_map(const char * path) {
	/*
	[info]
	width, height (int)

	[data]
	map (array of ints)
	*/

	// TODO: when porting to windows: https://github.com/ToruNiina/toml11#decoding-a-toml-file, open file in binary mode to prevent fuckshit errors
	FILE * fp = fopen(path, "r");

	char errbuf[200];

	if(!fp) load_error("Couldn't open map file - ", errbuf);

	std::string path_sstr(path);
	const toml::value root = toml::parse(path_sstr);

	fclose(fp);

	// TODO: error checking like we had in tomlc99

	const int width = toml::find<int>(root, "info", "width");
	const int height = toml::find<int>(root, "info", "height");

	const ivec map = toml::find<ivec>(root, "data", "map");

	if(map.size() == 0) load_error("Conversion from toml array to std::vector failed!");
	if((width * height) != map.size()) load_error("Loaded values info.width * info.height != map.size() - Data mismatch!");

	int map_size = map.size();

	ivec req = map;

	// as much as I dislike C++-isms, this seems to be vastly more efficient...
	std::sort(req.begin(), req.end());
	req.erase(std::unique(req.begin(), req.end()), req.end());

	int req_size = req.size();

	if(req_size > 32) load_error("Too many unique tiles requested! LIMIT: 32, REQ_SIZE: ", std::to_string(req_size).c_str());

	loadmap_return_t output = (loadmap_return_t) {
		width, height,
		map_size, req_size,
		map,
		req
	};

	return output;
}

void print_lmt(loadmap_return_t lmt) {
	printf("INFO: LMT: {\n");
	printf("\twidth: %d, height: %d,\n", lmt.width, lmt.height);
	printf("\tmap_size: %d, req_size: %d,\n", lmt.map_size, lmt.req_size);

	printf("\tmap: {\n");

	for(int y = 0; y < lmt.height; y++) {
		printf("\t\t");
		for(int x = 0; x < lmt.width; x++) {
			int t = lmt.map[y * lmt.width + x];

			if(t < 10) printf("%d  ", t);
			else printf("%d ", t);
		}
		printf("\n");
	}

	printf("\t},\n");

	printf("\treq: { ");
	for(int i = 0; i < lmt.req.size(); i++) {
		int t = lmt.req[i];
		printf("%d ", t);
	}

	printf("}\n");
	printf("}\n");
}

//------------------------------------------------------------------------------
// LOADTILE and related functions
//------------------------------------------------------------------------------

typedef struct {
	int tile_size, width, height; // width and height are the tile counts on each side
	cvec coords; // each index of the map maps to a (u, v) coordinate pair in this list
	Texture source;
} loadtile_return_t;

// takes a path to a toml file and outputs the map stored in that file
loadtile_return_t load_tile(const char * path, loadmap_return_t lmt) {
	/*
	[info]
	tile_size, width, height (int)
	source (string) // points to source image file
	*/

	// TODO: when porting to windows: https://github.com/ToruNiina/toml11#decoding-a-toml-file, open file in binary mode to prevent fuckshit errors
	FILE * fp = fopen(path, "r");

	char errbuf[200];

	if(!fp) load_error("Couldn't open map file - ", errbuf);

	std::string path_sstr(path);
	const toml::value root = toml::parse(path_sstr);

	fclose(fp);

	const int tile_size = toml::find<int>(root, "info", "tile_size");
	const int width = toml::find<int>(root, "info", "width");
	const int height = toml::find<int>(root, "info", "height");

	std::string source_path_sstr_header("./data/");
	std::string source_path_sstr_value = toml::find<std::string>(root, "info", "source");

	std::string source_path_sstr = source_path_sstr_header + source_path_sstr_value;

	const char * source_path = source_path_sstr.c_str();

	//------------------------------------------------------------------------------

	Image source_image = LoadImage(source_path);

	Texture source = LoadTextureFromImage(source_image);

	UnloadImage(source_image);

	//------------------------------------------------------------------------------

	cvec coords(width * height);

	for(int i = 0; i < lmt.req.size(); i++) {
		int t = lmt.req[i];

		int x = (t % width) * tile_size;
		int y = ((t - x) / height) * tile_size;

		coords[t] = (Vector2) { x, y };
	}

	loadtile_return_t output = (loadtile_return_t) {
		tile_size, width, height,
		coords,
		source
	};

	return output;
}

void print_ltt(loadtile_return_t ltt) {
	printf("INFO: LTT: {\n");
	printf("\twidth: %d, height: %d,\n", ltt.width, ltt.height);
	printf("\ttile_size: %d,\n", ltt.tile_size);

	printf("\tcoords: {\n");

	for(int y = 0; y < ltt.height; y++) {
		printf("\t\t");
		for(int x = 0; x < ltt.width; x++) {
			Vector2 uv = ltt.coords[y * ltt.width + x];

			printf("{ %d, %d }  ", uv.x, uv.y);
		}
		printf("\n");
	}

	printf("\t}\n");
	printf("}\n");
}

