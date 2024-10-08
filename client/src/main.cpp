#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "stuff.h"

#define screenWidth 1200
#define screenHeight 900

char password[256] = {0};
bool passwordCorrect = false;
Texture2D image;


std::vector<std::string> wishes;

void loginScreen()
{
    DrawText("Hallu Mausi <3", 100, 200, 40, GRAY);
    DrawText("Enter Password", 100, 300, 40, GRAY);
    GuiTextBox({100, 400, 300, 50}, password, 20, true);
    DrawTexture(image, (screenWidth - (image.width + 50)), screenHeight / 2 - image.height / 2, WHITE);

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

void drawListScreen()
{
    DrawTexture(image, 50, 50, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    DrawText("Add a Wish", 100, 550, 40, GRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    GuiTextBox({100, 650, 300, 50}, wish, 20, true);

    if (GuiButton({100, 750, 300, 50}, "Add Wish"))
    {
		std::string w = wish;
        addWish(w);
        loadWishes(wishes);
        memset(wish, 0, sizeof(wish));
    }

    bool needsUpdate = false;

    for(int i = 0; i < wishes.size(); i++)
    {
        auto state = GuiGetState();

        if(GuiButton({600, float(100 + i * 50), 30, 30}, "X"))
        {
            removeWish(i);
            needsUpdate = true;
        }

        if(i == 0)
            GuiSetState(STATE_DISABLED);

        if (GuiButton({650, float(100 + i * 50), 30, 30}, "+"))
        {
            moveWish(i, UP);
            needsUpdate = true;
        }
        GuiSetState(state);

        if (i + 1 == wishes.size())
            GuiSetState(STATE_DISABLED);

        if(GuiButton({700, float(100 + i * 50), 30, 30}, "-"))
        {
            moveWish(i, DOWN);
            needsUpdate = true;
        }

        GuiSetState(state);

        DrawText(wishes[i].c_str(), 750, 100 + i * 50, 30, GRAY);
    }
    if(needsUpdate)
        loadWishes(wishes);
}

int main() {
    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Raylib Window Example");

    // httplib::Client cli("http://localhost:8080");

    // auto res = cli.Post("/check_password");

    // std::cout << res->status << std::endl;
    // std::cout << res->body << std::endl;


    image = LoadTexture("bg.png");

    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        // Start drawing
        BeginDrawing();

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
