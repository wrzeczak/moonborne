#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

#define TILE_SCALE 4
#define TILESET_SIZE 9

enum {
	START_MENU,
	GAME_WORLD,
	SETTINGS_MENU
};

Color frametime_fade(Color c, int framecount) {
	int shifted_framecount = abs(framecount - (TARGET_FPS / 2));

	return Fade(c, (float) shifted_framecount / ((float) TARGET_FPS * 2) + (0.6f));
}

// FORMAT: { x, y, data }; size is data + 2, sizes are always negative
int map[18] = { 4, 4, 0, 1, 1, 2, 4, 5, 5, 6, 4, 5, 5, 6, 7, 8, 8, 9 };

void render_input_option(char * keyname, int framecount) {
	int font_size = HEIGHT * 0.03f;

	char * left = "Press ";

	int length = MeasureText(left, font_size) + MeasureText(keyname, font_size);

	int offset = (WIDTH - length) / 2;

	DrawText(left, offset, HEIGHT * 0.9f, font_size, frametime_fade(RAYWHITE, framecount));
	DrawText(keyname, offset + MeasureText(left, font_size), HEIGHT * 0.9f, font_size, frametime_fade(YELLOW, framecount));
}

void render_start_menu(int framecount) {
	ClearBackground(BLACK);

	int font_size = (0.1f) * HEIGHT;

	char * title = "MOONBORNE";

	int title_width = MeasureText(title, font_size);

	DrawText(title, (WIDTH - title_width) / 2, (int) (HEIGHT * 0.3f), font_size, RAYWHITE);

	DrawRectangle(10, HEIGHT - 50, 40, 40, frametime_fade(RED, framecount));

	render_input_option("SPACE", framecount);
}

// NOTE: map_size includes the two size members, ie. a map of size 16 should be 18 to include x and y
void render_map(int * map, int map_size, Texture2D * tileset) {
	int mx = abs(map[0]);
	int my = abs(map[1]);

	for(int i = 2; i < map_size; i++) {
		int x = i % 4;
		int y = (i - x) / 4;

		DrawTextureEx(tileset[map[i]], (Vector2) { x * 8 * TILE_SCALE, y * 8 * TILE_SCALE }, 0.0f, TILE_SCALE, WHITE);
	}
}

void render_game_world(int framecount, Texture2D * tileset) {
	ClearBackground(BLACK);
	render_map(map, 18, tileset);
}

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	bool render_debug_info = false;

	SetTargetFPS(TARGET_FPS);

	int framecount = 0;

	Image tileset_img = LoadImage("./tiles.png");

	Texture2D tileset[TILESET_SIZE] = { 0 };


	for(int i = 0; i < TILESET_SIZE; i++) {
		int x = i % 3;
		int y = (i - x) / 3;

		Image sector = ImageFromImage(tileset_img, (Rectangle) { 8, 8, x * 8, y * 8 });
		Texture2D tile = LoadTextureFromImage(sector);
		UnloadImage(sector);

		tileset[y * 3 + x] = tile;
	}

	while(!WindowShouldClose()) {

		if(framecount <= TARGET_FPS) framecount++;
		else framecount = 0;

		if(IsKeyPressed(KEY_F9)) render_debug_info = !render_debug_info;

		if(IsKeyPressed(KEY_SPACE) && screen_state == START_MENU) screen_state = GAME_WORLD;

		BeginDrawing();
			ClearBackground(BLACK);

			switch (screen_state) {
				case GAME_WORLD: render_game_world(framecount, tileset); break;
				default: render_start_menu(framecount);
			}

			if(render_debug_info) {
				DrawFPS(10, 10);
				DrawFPS(10, 130);
				DrawText(TextFormat("SCREENSTATE %d", screen_state), 10, 100, 20, RAYWHITE);
				DrawText(TextFormat("FRAMECOUNT %d", framecount), 10, 40, 20, RAYWHITE);
				DrawText(TextFormat("deltaTIME %.4f", GetFrameTime()), 10, 70, 20, RAYWHITE);
			}

		EndDrawing();
	}

	UnloadImage(tileset_img);

	CloseWindow();

	return 0;
}

