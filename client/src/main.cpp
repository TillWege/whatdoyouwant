#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "stuff.h"

#define INITIAL_SCREEN_WIDTH 1200
#define INITIAL_SCREEN_HEIGHT 900

char password[256] = {0};
bool passwordCorrect = false;
Texture2D image;

std::vector<std::string> wishes;

void loginScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();

	DrawText("Hallu Mausi <3", 100, 200, 40, GRAY);
	DrawText("Enter Password", 100, 300, 40, GRAY);
	GuiTextBox({100, 400, 300, 50}, password, 20, true);
	DrawTexture(image, width - (image.width + 50), height / 2 - image.height / 2, WHITE);

	GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
	if (GuiButton({100, 500, 300, 50}, "Login"))
	{
		auto pw = std::string(password);
		if (login(pw))
		{
			passwordCorrect = true;
			loadWishes(wishes);
		}
	}
}

char wish[256] = {0};
Vector2 scroll = {0, 0};

void drawListScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();

	DrawTexture(image, 50, 50, WHITE);
	GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
	DrawText("Add a Wish", 100, height - 350, 40, GRAY);

	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	GuiTextBox({100, float(height - 250), 300, 50}, wish, 20, true);

	if (GuiButton({100, float(height - 150), 300, 50}, "Add Wish"))
	{
		std::string w = wish;
		addWish(w);
		loadWishes(wishes);
		memset(wish, 0, sizeof(wish));
	}

	int itemCount = wishes.size();

	Rectangle bounds = {600, 0, float(width - 600), float(height)};
	Rectangle content = {0, 0, float(width - 700), float(itemCount * 50) + 150};

	Rectangle view = {0};

	GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xFFFFFFFF);
	GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);

	GuiScrollPanel(bounds, nullptr, content, &scroll, &view);

	BeginScissorMode(bounds.x, bounds.y, bounds.width, bounds.height);
	for (int i = 0; i < itemCount; i++)
	{
		float yPos = 100 + (i * 50) + scroll.y;
		if (yPos < 100 || yPos >= height - 50) continue;

		auto state = GuiGetState();

		if (GuiButton({600, yPos, 30, 30}, "X"))
		{
			removeWish(i);
			loadWishes(wishes);
			itemCount--;
			continue;
		}

		if (i == 0)
			GuiSetState(STATE_DISABLED);

		if (GuiButton({650, yPos, 30, 30}, "+"))
		{
			moveWish(i, UP);
			loadWishes(wishes);
		}
		GuiSetState(state);

		if (i + 1 == wishes.size())
			GuiSetState(STATE_DISABLED);

		if (GuiButton({700, yPos, 30, 30}, "-"))
		{
			moveWish(i, DOWN);
			loadWishes(wishes);
		}

		GuiSetState(state);
		DrawText(wishes[i].c_str(), 750, yPos, 30, GRAY);
	}
	EndScissorMode();
}

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Raylib Window Example");

	image = LoadTexture("bg.png");

	// Main game loop
	while (!WindowShouldClose()) {   // Detect window close button or ESC key
		// Start drawing
		BeginDrawing();
		std::cout << GetFPS() << std::endl;

		ClearBackground(WHITE);

		if(!passwordCorrect)
			loginScreen();
		else
			drawListScreen();

		EndDrawing();
	}

	UnloadTexture(image);
	CloseWindow();

	return 0;
}
