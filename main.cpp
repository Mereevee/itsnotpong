#include "raylib.h"
#include <iostream>
#include <vector>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

using namespace std;

int frameCounter = 0;
int screenWidth  = 1280;
int screenHeight = 720;
int playerVelocity = 420;
int playerPoints, enemyPoints;
Rectangle ball = { screenWidth / 2 - ball.width / 2, screenHeight / 2 - ball.height / 2, 20.0f , 20.0f };
bool ballStatic = true;
Vector2 ballSpeed = { 360.0f, 360.0f };
char tip[] = "Pressione a barra de espaço";
Sound hit = LoadSound("../resources/sound/hit.wav");
Rectangle player = { 90.0f, 90.0f, 20.0f, 100.0f };
Rectangle enemy = { screenWidth - 90.0f - 20, 90.0f, 20.0f, 100.0f };

void resetBall() {
    ballSpeed = { 360, 360 };
    if (GetRandomValue(0, 1) == 1) ballSpeed.x *= -1;
    if (GetRandomValue(0, 1) == 1) ballSpeed.y *= -1; 
    ball.x = screenWidth / 2 - ball.width;
    ball.y = screenHeight / 2 - ball.height;
}

void Update(void);

int main(void)
{
    // Initialization

    InitWindow(screenWidth, screenHeight, "cory na casa branca");
    InitAudioDevice();

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(Update, 0, 1);
    #else
        SetTargetFPS(9999999);
        while (!WindowShouldClose()) {
            Update();
        }
    #endif

    // De-Initialization
    CloseWindow();
    return 0;
}

void Update(void) {
    // UPDATE
    frameCounter++;
    float deltaTime = GetFrameTime();

    if (IsKeyDown(KEY_UP)   || IsKeyDown(KEY_W)) player.y -= playerVelocity * deltaTime;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.y += playerVelocity * deltaTime;
    if (IsKeyPressed(KEY_SPACE)) {
        if (ballStatic) {
            ballStatic = false;
        }
    }

    enemy.y = GetMouseY() - enemy.height / 2;

    if (enemy.y < 0) enemy.y = 0;
    if (enemy.y > GetScreenHeight() - enemy.height) enemy.y = GetScreenHeight() - enemy.height;

    if (player.y < 0) player.y = 0;
    if (player.y > GetScreenHeight() - player.height) player.y = GetScreenHeight() - player.height;

    if (ball.y < 0) {
        if (ballSpeed.y < 0) {
            ballSpeed.y *= -1;
        }
    }
    if (ball.y > GetScreenHeight() - ball.height) {
        if (ballSpeed.y > 0) {
            ballSpeed.y *= -1;
        }
    }
    if (ball.x < 0) {
        enemyPoints++;
        resetBall();
        ballStatic = true;
    }
    if (ball.x > GetScreenWidth() - ball.width) {
        playerPoints++;
        resetBall();
        ballStatic = true;
    }

    if (CheckCollisionRecs(ball, player)) {
        ballSpeed.x = ((ball.x - player.x + (player.width / 2))  / 4) * 60;
        ballSpeed.y = ((ball.y - player.y - (player.height / 2)) / 4) * 60;
        PlaySound(hit);
    }

    if (CheckCollisionRecs(ball, enemy)) {
        ballSpeed.x = ((ball.x - enemy.x - (enemy.width / 2))  / 4) * 60;
        ballSpeed.y = ((ball.y - enemy.y - (enemy.height / 2)) / 4) * 60;
        PlaySound(hit);
    }

    if (ballSpeed.x >  600) ballSpeed.x =  600;
    if (ballSpeed.y >  600) ballSpeed.y =  600;
    if (ballSpeed.x < -600) ballSpeed.x = -600;
    if (ballSpeed.y < -600) ballSpeed.y = -600;

    if (!ballStatic) {
        ball.x += ballSpeed.x * deltaTime;
        ball.y += ballSpeed.y * deltaTime;
    }
    
    // Draw
    BeginDrawing();

        ClearBackground(BLACK);
        DrawRectangleRec(player, WHITE);
        DrawRectangleRec(enemy, WHITE);
        DrawRectangleRec(ball, WHITE);
        DrawText(TextFormat("%i", playerPoints), 0, 500, 50, WHITE);
        DrawText(TextFormat("%i", enemyPoints), 1000, 500, 50, WHITE);
        DrawFPS(0, 0);


        DrawText("©mereevee", 0, screenHeight - 20, 20, WHITE);

        if (ballStatic) DrawText(tip, (screenWidth / 2) - (MeasureText(tip, 30) / 2), 400, 30, WHITE);

    EndDrawing();
}