#include <raylib.h>
#include <toml.h>

#include <stdlib.h>
#include <math.h>

#define i_implement // NOTE: "ivec.h" must be included before any other headers using ivec to prevent multiple inclusions, errors, etc.
#include "ivec.h"

#include "load.h"

//------------------------------------------------------------------------------

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

// tiles are scaled 4x
#define TILE_SCALE 8
#define TILE_SIZE 8

//------------------------------------------------------------------------------

// screen-state enumeration
enum {
	START_MENU,
	GAME_WORLD,
	SETTINGS_MENU
};

//------------------------------------------------------------------------------

// utility function for making things lightly blink to attract the eye
Color frametime_fade(Color c, int framecount) {
	int shifted_framecount = abs(framecount - (TARGET_FPS / 2));

	return Fade(c, (float) shifted_framecount / ((float) TARGET_FPS * 2) + (0.6f));
}

void ff_debug_box(Color c, int framecount) {
	DrawRectangle(10, HEIGHT - 50, 40, 40, frametime_fade(c, framecount));

	DrawText("<---- ff_debug_box", 10 + 40 + 10, HEIGHT - 40, 20, RAYWHITE);
}

// doesn't process any actual input, just says "you can press this key"
void render_input_option(char * keyname, int framecount) {
	int font_size = HEIGHT * 0.03f;

	char * left = "Press ";

	int length = MeasureText(left, font_size) + MeasureText(keyname, font_size);

	int offset = (WIDTH - length) / 2;

	DrawText(left, offset, HEIGHT * 0.9f, font_size, frametime_fade(RAYWHITE, framecount));
	DrawText(keyname, offset + MeasureText(left, font_size), HEIGHT * 0.9f, font_size, frametime_fade(YELLOW, framecount));
}

//------------------------------------------------------------------------------

// render the title screen
void render_start_menu(int framecount, bool render_debug_info) {
	ClearBackground(BLACK);

	int font_size = (0.1f) * HEIGHT;

	char * title = "MOONBORNE";

	int title_width = MeasureText(title, font_size);

	DrawText(title, (WIDTH - title_width) / 2, (int) (HEIGHT * 0.3f), font_size, RAYWHITE);

	if(render_debug_info) ff_debug_box(RED, framecount);

	render_input_option("SPACE", framecount);
}

//------------------------------------------------------------------------------

void render_general_debug_info(int framecount, int screen_state) {
	DrawText(TextFormat("SCREENSTATE %d", screen_state), 10, 100, 20, RAYWHITE);
	DrawText(TextFormat("FRAMECOUNT %d", framecount), 10, 40, 20, RAYWHITE);
	DrawText(TextFormat("deltaTIME %.4f", GetFrameTime()), 10, 70, 20, RAYWHITE);
	DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, RAYWHITE);
	DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, RAYWHITE);
	DrawLine(WIDTH / 4, 0, WIDTH / 4, HEIGHT, YELLOW);
	DrawLine(WIDTH * 0.75f, 0, WIDTH * 0.75f, HEIGHT, YELLOW);
	DrawLine(0, HEIGHT / 4, WIDTH, HEIGHT / 4, YELLOW);
	DrawLine(0, HEIGHT * 0.75f, WIDTH, HEIGHT * 0.75f, YELLOW);
}

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
				// case GAME_WORLD: render_game_world(framecount, lmt, lts, render_debug_info); break;
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

