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

	cvec coords(width * height); // NOTE: some of these values are empty because this only loads from req!

	for(int i = 0; i < lmt.req.size(); i++) {
		int t = lmt.req[i];

		int u = t % width;
		int v = (t - u) / height;

		coords[t] = (Vector2) { u, v };
	}

	loadtile_return_t output = (loadtile_return_t) {
		tile_size, width, height,
		coords,
		source
	};

	return output;
}

