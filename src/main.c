#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define i_key int
#define i_type ivec
#include "stc/cvec.h"

//------------------------------------------------------------------------------

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

// tiles are scaled 4x
#define TILE_SCALE 4
#define TILE_SIZE 8

// currently only using the top 9 groups from the tileset, will change in the future
#define TILESET_SIZE 9

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
void render_start_menu(int framecount) {
	ClearBackground(BLACK);

	int font_size = (0.1f) * HEIGHT;

	char * title = "MOONBORNE";

	int title_width = MeasureText(title, font_size);

	DrawText(title, (WIDTH - title_width) / 2, (int) (HEIGHT * 0.3f), font_size, RAYWHITE);

	DrawRectangle(10, HEIGHT - 50, 40, 40, frametime_fade(RED, framecount));

	render_input_option("SPACE", framecount);
}

//------------------------------------------------------------------------------

void render_game_world(int framecount, ivec map, int width, int height, Texture2D * tileset) {
	ClearBackground(BLACK);
	render_map(map, width, height, tileset);
}

void render_tile(int x, int y, int tile_type, Texture2D * tileset) {
	// DrawRectangle(x * 8 * TILE_SCALE, y * 8 * TILE_SCALE, 8 * TILE_SCALE, 8 * TILE_SCALE, BLUE);
	DrawTextureEx(tileset[tile_type], (Vector2) { x * TILE_SCALE * TILE_SIZE, y * TILE_SCALE * TILE_SIZE }, 0.0f, TILE_SCALE, WHITE);
	DrawText(TextFormat("%d", tile_type), x * 8 * TILE_SCALE + 10, y * 8 * TILE_SCALE + 10, 15, WHITE);
}

//! NOTE: the map size must be exactly width * height!
void render_map(ivec map, int width, int height, Texture2D * tileset) {
	int size = width * height;

	for(int i = 0; i < size; i++) {
		int x = i % width;
		int y = (i - x) / height;

		int t = *(ivec_at(&map, i));

		render_tile(x, y, t, tileset);
	}
}


//------------------------------------------------------------------------------

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	bool render_debug_info = false;

	SetTargetFPS(TARGET_FPS);

	int framecount = 0;

	Image tileset_img = LoadImage("./tiles.png");

	ivec map = { 0 };

	for(int i = 0; i < 9; i++) {
		ivec_push(&map, i);
	}

	Texture2D tileset[TILESET_SIZE];

	for(int i = 0; i < TILESET_SIZE; i++) {
		/* magic numbers, change these when changing tilesets */
		int w = 3;
		int h = 3;

		int x = i % w;
		int y = (i - x) / h;

		Image chunk = ImageFromImage(tileset_img, (Rectangle) { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE });

		Texture2D tile = LoadTextureFromImage(chunk);

		tileset[i] = tile;

		UnloadImage(chunk);
	}

	while(!WindowShouldClose()) {

		// keep framecount between TARGET_FPS and 0, framecount is used for stuff like frametime_fade
		if(framecount <= TARGET_FPS) framecount++;
		else framecount = 0;

		// toggle debug info
		if(IsKeyPressed(KEY_F9)) render_debug_info = !render_debug_info;

		if(IsKeyPressed(KEY_SPACE) && screen_state == START_MENU) screen_state = GAME_WORLD;

		BeginDrawing();
			ClearBackground(BLACK);

			// neat little way to do this i think, probably too small of a use case to be practical
			switch (screen_state) {
				case GAME_WORLD: render_game_world(framecount, map, 3, 3, tileset); break;
				default: render_start_menu(framecount);
			}

			// render debug info -- should probably separate into its own function but i want to be concurrent with other screen states
			if(render_debug_info) {
				DrawFPS(10, 10);
				DrawFPS(10, 130);
				DrawText(TextFormat("SCREENSTATE %d", screen_state), 10, 100, 20, RAYWHITE);
				DrawText(TextFormat("FRAMECOUNT %d", framecount), 10, 40, 20, RAYWHITE);
				DrawText(TextFormat("deltaTIME %.4f", GetFrameTime()), 10, 70, 20, RAYWHITE);
			}

		EndDrawing();
	}

	// i think raylib can do this auto-magically?? but just to be sure
	UnloadImage(tileset_img);

	CloseWindow();

	return 0;
}

