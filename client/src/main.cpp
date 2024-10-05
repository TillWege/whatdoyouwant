#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "httplib.h"

#define screenWidth 1200
#define screenHeight 900

char password[256] = {0};
bool passwordCorrect = false;
Texture2D image;

httplib::Client cli("http://localhost:8080");

httplib::Headers headers = {
    { "Content-Type", "application/json" }
};

std::vector<std::string> wishes;

void loadWishes()
{
    auto res = cli.Get("/wishes");

    if(res->status == 200)
    {
        std::cout << res->body << std::endl;
        wishes.clear();


        // Remove the surrounding brackets and quotes
        std::string cleanedStr = res->body.substr(1, res->body.length() - 2); // Remove [ and ]
        cleanedStr.erase(std::remove(cleanedStr.begin(), cleanedStr.end(), '\"'), cleanedStr.end());

        // Split the cleaned string by comma
        std::stringstream ss(cleanedStr);
        std::string token;
        while (std::getline(ss, token, ','))
        {
            wishes.push_back(token);
        }
    }
}

void loginScreen()
{
    DrawText("Hallu Mausi <3", 100, 200, 40, GRAY);
    DrawText("Enter Password", 100, 300, 40, GRAY);
    GuiTextBox({100, 400, 300, 50}, password, 20, true);
    DrawTexture(image, (screenWidth - (image.width + 50)), screenHeight / 2 - image.height / 2, WHITE);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    if (GuiButton({100, 500, 300, 50}, "Login"))
    {
        std::string json_payload = R"({"password": ")" + std::string(password) + R"("})";

        // Send POST request with JSON payload
        auto res = cli.Post("/check_password", headers, json_payload, "application/json");

        if(res->status == 200)
        {
            std::cout << "Password correct" << std::endl;
            passwordCorrect = true;
            loadWishes();
        }
    }
}

char wish[256] = {0};

void addWish()
{
    std::string payload = R"({"wish": ")" + std::string(wish) + R"(", "password": ")" + std::string(password) + R"("})";
    auto res = cli.Post("/wishes", headers, payload, "application/json");
    std::cout << res->status << std::endl;
}

void removeWish(int index)
{
    //std::string payload = R"({"wish": ")" + wishes[index] + R"(", "password": ")" + std::string(password) + R"("})";
    //auto res = cli.Delete("/wishes", headers, payload, "application/json");
    //std::cout << res->status << std::endl;
}

enum dir {UP, DOWN};

void moveWish(int index, dir direction)
{
    if(direction == UP)
    {
        std::string payload = R"({"id1": )" + std::to_string(index - 1) + R"(, "id2": )" + std::to_string(index) + R"(,"password": ")" + std::string(password) + R"("})";
        std::cout << payload << std::endl;
        auto res = cli.Post("/swap", headers, payload, "application/json");
        std::cout << res->status << std::endl;
        std::cout << res->body << std::endl;
    } else {
        std::string payload = R"({"id1": )" + std::to_string(index) + R"(, "id2": )" + std::to_string(index + 1) + R"(,"password": ")" + std::string(password) + R"("})";
        std::cout << payload << std::endl;
        auto res = cli.Post("/swap", headers, payload, "application/json");
        std::cout << res->status << std::endl;
        std::cout << res->body << std::endl;
    }
}

void drawListScreen()
{
    DrawTexture(image, 50, 50, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    DrawText("Add a Wish", 100, 550, 40, GRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    GuiTextBox({100, 650, 300, 50}, wish, 20, true);

    if (GuiButton({100, 750, 300, 50}, "Add Wish"))
    {
        addWish();
        loadWishes();
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
        loadWishes();
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
