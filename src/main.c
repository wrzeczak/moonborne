#include <raylib.h>
#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------------------

#define WIDTH 1600
#define HEIGHT 900

#define TARGET_FPS 75

// tiles are scaled 4x
#define TILE_SCALE 4

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

//------------------------------------------------------------------------------

typedef struct {
	int tile_type;
	Vector2 position;
} Tile;

// output[0] == a Tile of tile_type map size (mx * my + 1) which contains the map size in its position vector
Tile * init_map(int mapx, int mapy, int * data) {
	Tile map[mapx * mapy + 1];

	map[0] = (Tile) { (mapx * mapy) + 1, (Vector2) { mapx, mapy }};

	for(int y = 0; y < mapy; y++) {
		for(int x = 0; x < mapx; x++) {
			int type = data[y * mapx + x];

			map[y * mapx + x + 1] = (Tile) { type, (Vector2) { x, y } };
		}
	}

	return map;
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

// this and everything relating to the map will be massively revamped, i'm just trying to get something working at this point
// NOTE: map_size includes the two size members, ie. a map of size 16 should be 18 to include x and y
void render_map(Tile * map, Texture2D * tileset) {
	// the first two items of this list are the dimensions of the map
	int mx = map[0].position.x;
	int my = map[0].position.y;

	for(int i = 1; i < (mx * my + 1); i++) {
		Tile t = map[i];

		DrawTextureEx(tileset[t.tile_type], (Vector2) { t.position.x * TILE_SCALE, t.position.y * TILE_SCALE }, 0.0f, TILE_SCALE, WHITE);
	}
}

void render_game_world(int framecount, Tile * map, Texture2D * tileset) {
	ClearBackground(BLACK);
	render_map(map, tileset);
}

//------------------------------------------------------------------------------

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	bool render_debug_info = false;

	SetTargetFPS(TARGET_FPS);

	int framecount = 0;

	Image tileset_img = LoadImage("./tiles.png");

	Texture2D tileset[TILESET_SIZE] = { 0 };

	// load in each tile as its own texture and put it in this array for later access
	for(int i = 0; i < TILESET_SIZE; i++) {
		int x = i % 3;
		int y = (i - x) / 3;

		Image sector = ImageFromImage(tileset_img, (Rectangle) { 8, 8, x * 8, y * 8 });
		Texture2D tile = LoadTextureFromImage(sector);
		UnloadImage(sector);

		tileset[y * 3 + x] = tile;
	}

	Tile * map = init_map(3, 3, (int) { 1, 2, 3, 4, 5, 6, 7, 8, 9 });

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
				case GAME_WORLD: render_game_world(framecount, map, tileset); break;
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

