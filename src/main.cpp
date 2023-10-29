#include <raylib.h>
#include <toml.h>

#include <stdlib.h>
#include <math.h>

#include <vector>

typedef std::vector<int> ivec;

#include "load.h"
#include "rndr.h"

//------------------------------------------------------------------------------

#ifndef CONSTS
#define CONSTS

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

#define TILE_SCALE 8

// screen-state enumeration
enum {
	START_MENU,
	GAME_WORLD,
	SETTINGS_MENU
};

#endif

//------------------------------------------------------------------------------

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	bool render_debug_info = false;

	SetTargetFPS(TARGET_FPS);

	int framecount = 0;

	loadmap_return_t lmt = load_map("./data/debug-map.toml");
	printf("INFO: MAP: Map loaded succesfully! [ w, h, ms, rs ] [ %d, %d, %d, %d ]\n", lmt.width, lmt.height, lmt.map.size(), lmt.req.size());

	loadtile_return_t ltt = load_tile("./data/debug-tileset.toml");
	printf("INFO: TEXTURE: Tileset loaded succesfully! [ w, h, s ] [ %d, %d, %d ]\n", ltt.width, ltt.height, ltt.tile_size);

	while(!WindowShouldClose()) {

		// keep framecount between TARGET_FPS and 0, framecount is used for stuff like frametime_fade
		if(framecount <= TARGET_FPS) framecount++;
		else framecount = 0;

		// toggle debug info
		if(IsKeyPressed(KEY_F9)) render_debug_info = !render_debug_info;

		if(IsKeyPressed(KEY_SPACE) && screen_state == START_MENU) screen_state = GAME_WORLD;
		if(IsKeyPressed(KEY_F1)) screen_state = START_MENU;

		BeginDrawing();
			ClearBackground(BLACK);

			// neat little way to do this i think, probably too small of a use case to be practical
			switch (screen_state) {
				case GAME_WORLD: render_game_world(framecount, lmt, ltt, render_debug_info); break;
				default: render_start_menu(framecount, render_debug_info); break;
			}

			// render debug info -- should probably separate into its own function but i want to be concurrent with other screen states
			if(render_debug_info) {
				render_general_debug_info(framecount, screen_state);
			}

			DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
