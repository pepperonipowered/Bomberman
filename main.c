#include "raylib.h"
#include "player.h"

#define SCALE 2.5f

const int screenWidth = 800;
const int screenHeight = 450;

Player player = {
    .player_position = { 48, 72 },
    .player_rectangle = { 16, 16 }
};

// The gray obstacle, defined once and used for both drawing and collision
Rectangle wall = { 400, 225, 16 * SCALE, 16 * SCALE };

bool colliding = false;

// Player position is the center, so convert to a top-left Rectangle
Rectangle get_player_rect(const Player *p) {
    float w = p->player_rectangle.x * SCALE;
    float h = p->player_rectangle.y * SCALE;
    return (Rectangle){
        p->player_position.x - w / 2,
        p->player_position.y - h / 2,
        w,
        h
    };
}

Vector2 user_input(void) {
    Vector2 move = { 0, 0 };
    if (IsKeyDown(KEY_RIGHT)) move.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))  move.x -= 2.0f;
    if (IsKeyDown(KEY_UP))    move.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))  move.y += 2.0f;
    return move;
}

void update() {
    Vector2 move = user_input();
    float halfW = player.player_rectangle.x * SCALE / 2;
    float halfH = player.player_rectangle.y * SCALE / 2;

    // Move and resolve each axis separately so you can slide along walls
    player.player_position.x += move.x;
    if (CheckCollisionRecs(get_player_rect(&player), wall)) {
        if (move.x > 0) player.player_position.x = wall.x - halfW;
        else if (move.x < 0) player.player_position.x = wall.x + wall.width + halfW;
    }

    player.player_position.y += move.y;
    if (CheckCollisionRecs(get_player_rect(&player), wall)) {
        if (move.y > 0) player.player_position.y = wall.y - halfH;
        else if (move.y < 0) player.player_position.y = wall.y + wall.height + halfH;
    }

    // For debug display: true when touching (edges count as touching)
    Rectangle r = get_player_rect(&player);
    colliding = CheckCollisionRecs(
        (Rectangle){ r.x - 1, r.y - 1, r.width + 2, r.height + 2 }, wall);
}

void draw() {
    Rectangle r = get_player_rect(&player);

    BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(wall, GRAY);
        DrawRectangleRec(r, PINK);
        DrawRectangleLinesEx(r, 1, colliding ? RED : GREEN); // debug box
        DrawCircleV(player.player_position, 2.0f, colliding ? RED : GREEN); //debug center

        DrawFPS(10, 10);
    EndDrawing();
}

void init() {
    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);
}

int main() {
    init();
    while (!WindowShouldClose()) {
        update();
        draw();
    }
    CloseWindow();
    return 0;
}