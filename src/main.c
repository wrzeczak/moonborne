#include <raylib.h>

int main(void) {
	InitWindow(800, 600, "MOONBORNE");

	while(!WindowShouldClose()) {
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

