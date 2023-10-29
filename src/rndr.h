//------------------------------------------------------------------------------
// rndr.h - rendering functions
//------------------------------------------------------------------------------

#pragma once


#include <vector>

typedef std::vector<int> ivec;
typedef std::vector<Texture> tvec;

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

#define SUPPORT_FILEFORMAT_PNG

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

// note: t is the requested tile id
void render_tile(int x, int y, int t, loadtile_return_t ltt, bool render_debug_info) {
	// DrawRectangle(x * 8 * TILE_SCALE, y * 8 * TILE_SCALE, 8 * TILE_SCALE, 8 * TILE_SCALE, BLUE);
	DrawTextureEx(ltt.tileset[t], (Vector2) { x * TILE_SCALE * ltt.tile_size, y * TILE_SCALE * ltt.tile_size }, 0.0f, TILE_SCALE, WHITE);

	if(render_debug_info) DrawText(TextFormat("%d", t), (x * TILE_SCALE * ltt.tile_size) + (TILE_SCALE * ltt.tile_size * 0.25), (y * TILE_SCALE * ltt.tile_size) + (TILE_SCALE * ltt.tile_size * 0.25), TILE_SCALE * ltt.tile_size * 0.5, RAYWHITE);
}

//! NOTE: the map size must be exactly width * height!
void render_map(loadmap_return_t lmt, loadtile_return_t ltt, bool render_debug_info) {
	int size = lmt.width * lmt.height;

	for(int i = 0; i < size; i++) {
		int x = i % lmt.width;
		int y = (i - x) / lmt.height;

		int t = lmt.map[i];

		render_tile(x, y, t, ltt, render_debug_info);
	}
}

void render_game_world(int framecount, loadmap_return_t lmt, loadtile_return_t ltt, bool render_debug_info) {
	ClearBackground(BLACK);
	render_map(lmt, ltt, render_debug_info);

	if(render_debug_info) ff_debug_box(BLUE, framecount);
}

//------------------------------------------------------------------------------

