#include <raylib.h>
#include <toml.h>

#include <stdlib.h>
#include <math.h>

#define i_implement // NOTE: "ivec.h" must be included before any other headers using ivec to prevent multiple inclusions, errors, etc.
#include "ivec.h"

#include "load.h"
#include "rndr.h"

//------------------------------------------------------------------------------

#ifndef CONSTS
#define CONSTS

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

#define TILE_SCALE 8
#define TILE_SIZE 8

// screen-state enumeration
enum {
	START_MENU,
	GAME_WORLD,
	SETTINGS_MENU
};

#endif

/*------------------------------------------------------------------------------

void render_tile(int x, int y, int tile_type, Texture2D * tileset, bool render_debug_info) {
	// DrawRectangle(x * 8 * TILE_SCALE, y * 8 * TILE_SCALE, 8 * TILE_SCALE, 8 * TILE_SCALE, BLUE);
	DrawTextureEx(tileset[tile_type], (Vector2) { x * TILE_SCALE * TILE_SIZE + (tile_type * 2), y * TILE_SCALE * TILE_SIZE + (tile_type * 2) }, (float) tile_type, TILE_SCALE, WHITE);

	if(render_debug_info) DrawText(TextFormat("%d", tile_type), (x * TILE_SCALE * TILE_SIZE) + (TILE_SCALE * TILE_SIZE * 0.25), (y * TILE_SCALE * TILE_SIZE) + (TILE_SCALE * TILE_SIZE * 0.25), TILE_SCALE * TILE_SIZE * 0.5, RAYWHITE);
}

//! NOTE: the map size must be exactly width * height!
void render_map(ivec map, int width, int height, Texture2D * tileset, bool render_debug_info) {
	int size = width * height;

	for(int i = 0; i < size; i++) {
		int x = i % width;
		int y = (i - x) / height;

		int t = *(ivec_at(&map, i));

		render_tile(x + 10, y + 5, t, tileset, render_debug_info);
	}
}

void render_game_world(int framecount, ivec map, loadmap_return_t lmt, loadtileset_return_t lts, bool render_debug_info) {
	ClearBackground(BLACK);
	render_map(map, width, height, tileset, render_debug_info);

	if(render_debug_info) ff_debug_box(BLUE, framecount);
}

//------------------------------------------------------------------------------

*/

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	bool render_debug_info = false;

	SetTargetFPS(TARGET_FPS);

	int framecount = 0;

	loadmap_return_t lmt = load_map("./data/debug-map.toml");
	printf("INFO: MAP: Map loaded succesfully! WIDTH: %d, HEIGHT: %d, MAP SIZE: %d, REQ SIZE: %d\n", lmt.width, lmt.height, (int) ivec_size(&(lmt.map)), (int) ivec_size(&(lmt.req)));

	loadtile_return_t ltt = load_tile("./data/debug-tileset.toml");
	printf("INFO: TEXTURE: Tileset loaded succesfully!\n");

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
				default: render_start_menu(framecount, render_debug_info);
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

