#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"
#define MAX_INPUT_CHARS 9

typedef struct {
    Vector2 position;
    Vector2 size;
    int points;
    char name[MAX_INPUT_CHARS + 1];
} Paddle;

#endif