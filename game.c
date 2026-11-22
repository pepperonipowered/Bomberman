#include "game.h"

Vector2 user_input(void) {
    Vector2 move = { 0, 0 };
    if (IsKeyDown(KEY_RIGHT)) move.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))  move.x -= 2.0f;
    if (IsKeyDown(KEY_UP))    move.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))  move.y += 2.0f;
    return move;
}