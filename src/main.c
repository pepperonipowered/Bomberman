#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>

#define BOARD_W 17
#define BOARD_H 13
#define TILE_PX 40

#define HALF      15.0f   // half-size of the player hitbox (30px box in a 40px tile)
#define ASSIST    28.0f   // max misalignment (px) that still gets nudged into a lane

static bool solid[BOARD_H][BOARD_W];

static bool is_solid(int x, int y)
{
    if (x < 0 || y < 0 || x >= BOARD_W || y >= BOARD_H) return true;
    return solid[y][x];
}

// Move along X by dx, snapping flush against any solid tile. Returns true if blocked.
static bool move_x(Vector2 *p, float dx)
{
    p->x += dx;
    int y0 = (int)floorf((p->y - HALF) / TILE_PX);
    int y1 = (int)floorf((p->y + HALF - 0.01f) / TILE_PX);

    if (dx > 0) {
        int x = (int)floorf((p->x + HALF - 0.01f) / TILE_PX);
        for (int y = y0; y <= y1; y++)
            if (is_solid(x, y)) { p->x = x * TILE_PX - HALF; return true; }
    } else if (dx < 0) {
        int x = (int)floorf((p->x - HALF) / TILE_PX);
        for (int y = y0; y <= y1; y++)
            if (is_solid(x, y)) { p->x = (x + 1) * TILE_PX + HALF; return true; }
    }
    return false;
}

// Same thing along Y.
static bool move_y(Vector2 *p, float dy)
{
    p->y += dy;
    int x0 = (int)floorf((p->x - HALF) / TILE_PX);
    int x1 = (int)floorf((p->x + HALF - 0.01f) / TILE_PX);

    if (dy > 0) {
        int y = (int)floorf((p->y + HALF - 0.01f) / TILE_PX);
        for (int x = x0; x <= x1; x++)
            if (is_solid(x, y)) { p->y = y * TILE_PX - HALF; return true; }
    } else if (dy < 0) {
        int y = (int)floorf((p->y - HALF) / TILE_PX);
        for (int x = x0; x <= x1; x++)
            if (is_solid(x, y)) { p->y = (y + 1) * TILE_PX + HALF; return true; }
    }
    return false;
}

// If blocked while moving along X, slide toward the center of the lane (row) ahead.
static void assist_x(Vector2 *p, float dx, float step)
{
    int ahead = (int)floorf((p->x + (dx > 0 ? HALF + 1 : -HALF - 1)) / TILE_PX);
    int base  = (int)floorf(p->y / TILE_PX);

    float best = 0.0f;
    bool found = false;
    for (int row = base - 1; row <= base + 1; row++) {
        if (is_solid(ahead, row)) continue;               // lane blocked
        float off = (row + 0.5f) * TILE_PX - p->y;
        if (fabsf(off) > ASSIST) continue;                // too far away
        if (!found || fabsf(off) < fabsf(best)) { best = off; found = true; }
    }
    if (found) move_y(p, Clamp(best, -step, step));
}

static void assist_y(Vector2 *p, float dy, float step)
{
    int ahead = (int)floorf((p->y + (dy > 0 ? HALF + 1 : -HALF - 1)) / TILE_PX);
    int base  = (int)floorf(p->x / TILE_PX);

    float best = 0.0f;
    bool found = false;
    for (int col = base - 1; col <= base + 1; col++) {
        if (is_solid(col, ahead)) continue;
        float off = (col + 0.5f) * TILE_PX - p->x;
        if (fabsf(off) > ASSIST) continue;
        if (!found || fabsf(off) < fabsf(best)) { best = off; found = true; }
    }
    if (found) move_x(p, Clamp(best, -step, step));
}

int main(void)
{
    InitWindow(BOARD_W * TILE_PX, BOARD_H * TILE_PX, "Bomberman");
    SetTargetFPS(60);

    Color dark_gray = { 18, 18, 18, 255 };

    // border + classic pillars + a manual tile
    for (int y = 0; y < BOARD_H; y++)
        for (int x = 0; x < BOARD_W; x++)
            solid[y][x] = (x == 0 || y == 0 || x == BOARD_W - 1 || y == BOARD_H - 1
                           || (x % 2 == 0 && y % 2 == 0));
    solid[3][5] = true;

    Vector2 player = { 1.5f * TILE_PX, 1.5f * TILE_PX };
    float speed = 160.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;

        Vector2 dir = { 0, 0 };
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dir.x += 1;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) dir.x -= 1;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) dir.y += 1;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) dir.y -= 1;

       if (dir.x != 0 || dir.y != 0) {
			Vector2 n = Vector2Normalize(dir);
			float step = speed * dt;
			float dx = n.x * step;
			float dy = n.y * step;

			bool bx = dx != 0 && move_x(&player, dx);
			bool by = dy != 0 && move_y(&player, dy);

			// blocked on one axis: top up the other axis to full speed (no wall drag)
			if (bx && dy != 0) move_y(&player, (dy > 0 ? 1 : -1) * (step - fabsf(dy)));
			if (by && dx != 0) move_x(&player, (dx > 0 ? 1 : -1) * (step - fabsf(dx)));

			// corner assist only when pressing a single direction
			if (bx && dy == 0) assist_x(&player, dx, step);
			if (by && dx == 0) assist_y(&player, dy, step);
		}

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < BOARD_H; y++) {
            for (int x = 0; x < BOARD_W; x++) {
                DrawRectangle(x * TILE_PX, y * TILE_PX, TILE_PX, TILE_PX,
                              solid[y][x] ? GRAY : dark_gray);
                DrawRectangleLines(x * TILE_PX, y * TILE_PX, TILE_PX, TILE_PX, GREEN);
            }
        }

        // actual hitbox (what collision uses)
		DrawRectangleRec((Rectangle){ player.x - HALF, player.y - HALF, HALF * 2, HALF * 2 },
						Fade(PURPLE, 0.5f));
		DrawRectangleLinesEx((Rectangle){ player.x - HALF, player.y - HALF, HALF * 2, HALF * 2 },
							2, PURPLE);

		// the circle on top, for comparison
		DrawCircleV(player, HALF, Fade(WHITE, 0.4f));

		// center point
		DrawCircleV(player, 2, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}