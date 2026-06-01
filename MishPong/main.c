#include "raylib.h"
#include "ball.h"
#include "paddle.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gameScreen.h"

void UpdateBallPosition(Ball *ball, float deltaTime)
{
    ball->position.x += ball->velocity.x * ball->speed * deltaTime;
    ball->position.y += ball->velocity.y * ball->speed * deltaTime;
}
void DetectWallCollision(
    Ball *ball,
    int screenHeight)
{
    if ((ball->position.y > screenHeight - ball->radius) || (ball->position.y < ball->radius))
    {
        ball->velocity.y *= -1;
    }
}

void HandlePaddleMovement(Paddle *paddle, float deltaTime, int keyUp, int keyDown)
{
    if (IsKeyDown(keyUp) && (paddle->position.y > 0))
    {
        paddle->position.y -= 200.0f * deltaTime;
    }
    if (IsKeyDown(keyDown) && (paddle->position.y < GetScreenHeight() - paddle->size.y))
    {
        paddle->position.y += 200.0f * deltaTime;
    }
}

void HandlePaddleCollision(Ball *ball, Rectangle paddle)
{
    bool collision = CheckCollisionCircleRec(ball->position, ball->radius, paddle);

    if (collision)
    {

        if (collision)
        {
            // Displace ball outside paddle bounds before reversing so the collision
            // isn't re-triggered on the next frame while overlap persists.
            if (ball->velocity.x > 0)
            {
                ball->position.x = paddle.x - ball->radius;
            }
            else
            {
                ball->position.x = paddle.x + paddle.width + ball->radius;
            }
            ball->velocity.x *= -1;
        }
    }
}

Vector2 GetRandomVelocity()
{
    float x = (rand() % 2 == 0) ? -1.0f : 1.0f;
    float y = (rand() % 2 == 0) ? -1.0f : 1.0f;
    return (Vector2){x, y};
}

void ResetPositions(Ball *ball, Paddle *paddle, Paddle *paddle2)
{
    ball->position.x = GetScreenWidth() / 2.0f;
    ball->position.y = GetScreenHeight() / 2.0f;
    ball->velocity = GetRandomVelocity();

    paddle->position.x = 10.0f;
    paddle->position.y = 200.0f;

    paddle2->position.x = GetScreenWidth() - 80.0f;
    paddle2->position.y = 200.0f;
}

void ResetGame(Ball *ball, Paddle *paddle, Paddle *paddle2)
{
    ResetPositions(ball, paddle, paddle2);
    paddle->points = 0;
    paddle2->points = 0;
}

void DrawPlayingScreen(Ball ball, Paddle paddle, Paddle paddle2)
{
    DrawCircle(ball.position.x, ball.position.y, ball.radius, MAROON);
    DrawRectangle(paddle.position.x, paddle.position.y, paddle.size.x, paddle.size.y, Fade(SKYBLUE, 0.5f));
    DrawRectangle(paddle2.position.x, paddle2.position.y, paddle2.size.x, paddle2.size.y, Fade(SKYBLUE, 0.5f));
    DrawText(TextFormat("%s: %d", paddle.name, paddle.points), GetScreenWidth() / 4 - MeasureText(TextFormat("%s: %d", paddle.name, paddle.points), 40) / 2, 20, 40, GRAY);
    DrawText(TextFormat("%s: %d", paddle2.name, paddle2.points), GetScreenWidth() * 3 / 4 - MeasureText(TextFormat("%s: %d", paddle2.name, paddle2.points), 40) / 2, 20, 40, GRAY);
}

void DrawGameOverScreen(Paddle winner)
{
    const char *gameOverText = TextFormat("%s Wins! Press 'R' to Restart", winner.name);
    DrawText(gameOverText, GetScreenWidth() / 2 - MeasureText(gameOverText, 40) / 2, GetScreenHeight() / 2 - 20, 40, RED);
}
void HandlePlayerNameInput(char *playerName, int *letterCount, int *frameCounter, bool *mouseOnText, Rectangle textBox)
{
    if (CheckCollisionPointRec(GetMousePosition(), textBox))
    {
        *mouseOnText = true;
    }
    else
    {
        *mouseOnText = false;
    }

    if (*mouseOnText)
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();

        // Drain the whole per-frame key queue; a single call would drop
        // characters when the user types faster than one key per frame.
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (*letterCount < MAX_INPUT_CHARS))
            {
                playerName[*letterCount] = (char)key;
                playerName[*letterCount + 1] = '\0';
                (*letterCount)++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            (*letterCount)--;
            if (*letterCount < 0)
            {
                *letterCount = 0;
            }

            playerName[*letterCount] = '\0';
        }
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (*mouseOnText)
    {
        (*frameCounter)++;
    }
    else
    {
        *frameCounter = 0;
    }
}
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126))
        keyPressed = true;

    return keyPressed;
}

void DrawPlayerNameInput(char *playerName, int playerNumber, int letterCount, int frameCounter, Rectangle textBox, bool mouseOnText, int posX, int posY)
{
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawText(playerName, textBox.x, textBox.y + 5, 40, MAROON);
    DrawText(TextFormat("PLAYER %d NAME: %i/%i", playerNumber, letterCount, MAX_INPUT_CHARS), posX, posY, 20, DARKGRAY);

    if (mouseOnText)
    {
        if (letterCount < MAX_INPUT_CHARS)
        {
            // /20 sets the blink half-period: at 60fps this yields ~3 blinks/second.
            if (((frameCounter / 20) % 2) == 0)
            {
                DrawText("_", textBox.x + 5 + MeasureText(playerName, 40), textBox.y + 5, 40, MAROON);
            }
        }
    }
}

void DrawMenuScreen(char *playerName,
                    char *playerName2,
                    int letterCount,
                    int letterCount2,
                    int frameCounter,
                    Rectangle textBox,
                    Rectangle textBox2,
                    bool mouseOnText)
{
    DrawText("Welcome to MishPong!", GetScreenWidth() / 2 - MeasureText("Welcome to MishPong!", 40) / 2, GetScreenHeight() / 2 - 100, 40, DARKGRAY);
    DrawText("Press ENTER to Start", GetScreenWidth() / 2 - MeasureText("Press ENTER to Start", 20) / 2, GetScreenHeight() / 2 - 10, 20, GRAY);

    DrawPlayerNameInput(playerName, 1, letterCount, frameCounter, textBox, mouseOnText, textBox.x - 10, textBox.y + 50);
    DrawPlayerNameInput(playerName2, 2, letterCount2, frameCounter, textBox2, mouseOnText, textBox2.x - 10, textBox2.y + 50);
}
int main(void)
{

    InitWindow(800, 600, "MishPong");

    char playerName[MAX_INPUT_CHARS + 1] = "Player 1";
    int letterCount = 8;

    char playerName2[MAX_INPUT_CHARS + 1] = "Player 2";
    int letterCount2 = 8;

    int frameCounter = 0;

    srand(time(NULL));
    SetTargetFPS(60);

    Ball ball = {
        .position = {800 / 2.0f, 600 / 2.0f},
        .velocity = GetRandomVelocity(),
        .radius = 30.0f,
        .speed = 200.0f};

    Paddle paddle = {
        .position = {10.0f, 200.0f},
        .size = {70.0f, 200.0f},
        .points = 0,
        .name = "Player 1"};

    Paddle paddle2 = {
        .position = {GetScreenWidth() - 80.0f, 200.0f},
        .size = {70.0f, 200.0f},
        .points = 0,
        .name = "Player 2"};

    Rectangle textBox = {GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 + 70, 200, 40};
    Rectangle textBox2 = {GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 + 70, 200, 40};
    bool mouseOnText = false;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    GameScreen currentScreen = GAME_MENU;

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case GAME_MENU:
            HandlePlayerNameInput(playerName, &letterCount, &frameCounter, &mouseOnText, textBox);
            HandlePlayerNameInput(playerName2, &letterCount2, &frameCounter, &mouseOnText, textBox2);

            if (IsKeyPressed(KEY_ENTER))
            {
                strncpy(paddle.name, playerName, MAX_INPUT_CHARS);
                paddle.name[MAX_INPUT_CHARS] = '\0'; // strncpy won't null-terminate when source fills the buffer

                strncpy(paddle2.name, playerName2, MAX_INPUT_CHARS);
                paddle2.name[MAX_INPUT_CHARS] = '\0';

                currentScreen = GAME_PLAYING;
            }
            break;
        case GAME_PLAYING:
        {
            float deltaTime = GetFrameTime();
            // 1. Update
            UpdateBallPosition(&ball, deltaTime);
            DetectWallCollision(&ball, screenHeight);

            HandlePaddleMovement(&paddle, deltaTime, KEY_W, KEY_S);
            HandlePaddleMovement(&paddle2, deltaTime, KEY_UP, KEY_DOWN);

            Rectangle paddleRect = {paddle.position.x, paddle.position.y, paddle.size.x, paddle.size.y};
            Rectangle paddle2Rect = {paddle2.position.x, paddle2.position.y, paddle2.size.x, paddle2.size.y};
            HandlePaddleCollision(&ball, paddleRect);
            HandlePaddleCollision(&ball, paddle2Rect);

            if (ball.position.x - ball.radius < 0)
            {
                paddle2.points++;
                ResetPositions(&ball, &paddle, &paddle2);
            }
            else if (ball.position.x + ball.radius > screenWidth)
            {
                paddle.points++;
                ResetPositions(&ball, &paddle, &paddle2);
            }

            if (paddle.points >= 5 || paddle2.points >= 5)
            {
                currentScreen = GAME_OVER;
            }
            break;
        }
        case GAME_OVER:
            if (IsKeyPressed(KEY_R))
            {
                ResetGame(&ball, &paddle, &paddle2);
                currentScreen = GAME_MENU;
            }
            break;
        }
        // 2. Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentScreen)
        {
        case GAME_MENU:
            DrawMenuScreen(playerName,
                           playerName2,
                           letterCount,
                           letterCount2,
                           frameCounter,
                           textBox,
                           textBox2,
                           mouseOnText);
            break;
        case GAME_PLAYING:
            DrawPlayingScreen(ball, paddle, paddle2);
            break;
        case GAME_OVER:
            DrawGameOverScreen(paddle.points >= 5 ? paddle : paddle2);
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}