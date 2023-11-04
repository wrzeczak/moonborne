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

typedef struct {
	int width, height;
	int girth, length;
	int tile_size;
	ivec map, req;
	cvec coords;
	Texture source;
} loadmap_return_t;

//------------------------------------------------------------------------------

void load_error(const char * msg, const char * errbuf);
void load_error(const char * msg);

loadmap_return_t load_map(char * path);

void print_lmt(loadmap_return_t lmt);

//------------------------------------------------------------------------------
// MAP LOADING
//------------------------------------------------------------------------------


loadmap_return_t load_map(const char * path) {
	// TODO: when porting to windows: https://github.com/ToruNiina/toml11#decoding-a-toml-file, open file in binary mode to prevent fuckshit errors
	FILE * fp = fopen(path, "r");

	char errbuf[200];

	if(!fp) load_error("Couldn't open map file - ", errbuf);

	std::string path_sstr(path);
	const toml::value root = toml::parse(path_sstr);

	fclose(fp);

	//------------------------------------------------------------------------------

	const int width = toml::find<int>(root, "info", "width");
	const int height = toml::find<int>(root, "info", "height");
	const int tile_size = toml::find<int>(root, "info", "tile_size");
	const int girth = toml::find<int>(root, "info", "girth"); // how many tiles wide the tileset is
	const int length = toml::find<int>(root, "info", "length"); // how many tiles tall the tileset is

	std::string source_path_sstr_header("./data/");
	std::string source_path_sstr_value = toml::find<std::string>(root, "info", "source");

	std::string source_path_sstr = source_path_sstr_header + source_path_sstr_value;

	const char * source_path = source_path_sstr.c_str();

	//------------------------------------------------------------------------------

	Image source_image = LoadImage(source_path);

	Texture source = LoadTextureFromImage(source_image);

	UnloadImage(source_image);

	//------------------------------------------------------------------------------

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

	//------------------------------------------------------------------------------

	cvec coords(girth * length);

	for(int y = 0; y < length; y++) {
		for(int x = 0; x < girth; x++) {
			Vector2 uv = { (float) ((x % girth) * tile_size), (float) ((y % length) * tile_size) };

			coords[y * girth + x] = uv;
		}
	}

	//------------------------------------------------------------------------------

	printf("INFO: LOADMAP: coords: { ");
	for(int i = 0; i < coords.size(); i++) {
		Vector2 uv = coords[i];
		printf("{ %d, %d } ", uv.x, uv.y);
	}
	printf("}\n");

	//------------------------------------------------------------------------------

	loadmap_return_t output = (loadmap_return_t) {
		width, height,
		girth, length,
		tile_size,
		map, req,
		coords,
		source
	};

	return output;
}

//------------------------------------------------------------------------------
// DEBUG FUNCTIONS
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

void print_lmt(loadmap_return_t lmt) {
	printf("INFO: LMT: {\n");
	printf("\twidth: %d, height: %d,\n", lmt.width, lmt.height);
	printf("\tgirth: %d, length: %d,\n", lmt.girth, lmt.length);
	printf("\ttile_size: %d,\n", lmt.tile_size);
	printf("\tmap_size: %d, req_size: %d,\n", lmt.map.size(), lmt.req.size());

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

	printf("},\n");

	printf("\tcoords: { ");
	for(int i = 0; i < lmt.coords.size(); i++) {
		Vector2 uv = lmt.coords[i];
		printf("{ %d, %d } ", uv.x, uv.y);
	}

	printf("}\n");
	printf("\t}\n");
	printf("}\n");
}

