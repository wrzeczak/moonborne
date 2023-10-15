#include <raylib.h>

#define WIDTH 1600
#define HEIGHT 900

enum {
	START_MENU,
	GAME_WORLD,
	SETTINGS_MENU
};

void render_start_menu() {
	ClearBackground(BLACK);

	int font_size = (0.1f) * HEIGHT;

	char * title = "MOONBORNE";

	int title_width = MeasureText(title, font_size);

	DrawText(title, (WIDTH - title_width) / 2, (int) (HEIGHT * 0.3f), font_size, RAYWHITE);
}

int main(void) {
	InitWindow(WIDTH, HEIGHT, "MOONBORNE");

	int screen_state = START_MENU;

	while(!WindowShouldClose()) {
		BeginDrawing();

			switch (screen_state) {
				default: render_start_menu();
			}

			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

