#include "raylib.h"
#include "player.h"
#include "config.h"
#include "map.h"

/* Player sprite sheet: 64 x 96 = 4 columns x 6 rows of 16 px cells.
 * Rows map 1:1 to the Direction enum: DOWN, UP, LEFT, RIGHT. */
static Texture2D player_tex;

PlayerIntent read_keyboard_intent(void) {
    PlayerIntent in = {0};

    if (IsKeyDown(KEY_LEFT))  in.move_x -= 1;
    if (IsKeyDown(KEY_RIGHT)) in.move_x += 1;
    if (IsKeyDown(KEY_UP))    in.move_y -= 1;
    if (IsKeyDown(KEY_DOWN))  in.move_y += 1;

    in.place_bomb = IsKeyPressed(KEY_SPACE);
    return in;
}

/* True if the player's hitbox overlaps any solid map tile. */
static bool collides(const Player *p) {
    int r1 = (int)(p->y / TILE_SIZE);
    int r2 = (int)((p->y + p->height - 1) / TILE_SIZE);
    int c1 = (int)(p->x / TILE_SIZE);
    int c2 = (int)((p->x + p->width - 1) / TILE_SIZE);

    for (int r = r1; r <= r2; r++) {
        for (int c = c1; c <= c2; c++) {
            Tile t = map_get(r, c);
            if (t == TILE_WALL || t == TILE_BLOCK) return true;
        }
    }
    return false;
}

void player_init(Player *p, int id, float x, float y) {
	p->id          = id;
    p->x           = x;
    p->y           = y;
    p->width       = PLAYER_SIZE * SCALE;
    p->height      = PLAYER_FRAME_H * SCALE;
    p->facing      = DIR_DOWN;
    p->speed       = 120.0f;
    p->max_bombs   = 1;
    p->blast_range = 2 * 1;
    p->bombs_active = 0;
    p->alive       = true;
    p->anim_frame  = 0;
    p->anim_time   = 0;

    player_tex = LoadTexture("assets/PlayerWhiteWalk.png");
    SetTextureFilter(player_tex, TEXTURE_FILTER_POINT);
}

void player_update(Player *p, PlayerIntent in, float dt) {
    if (!p->alive) return;

    // X axis
    float old_x = p->x;
    p->x += in.move_x * p->speed * dt;
    if (collides(p))  p->x = old_x;

    // Y axis
    float old_y = p->y;
    p->y += in.move_y * p->speed * dt;
    if (collides(p))  p->y = old_y;

    // facing
    if (in.move_x < 0) p->facing = DIR_LEFT;
    if (in.move_x > 0) p->facing = DIR_RIGHT;
    if (in.move_y < 0) p->facing = DIR_UP;
    if (in.move_y > 0) p->facing = DIR_DOWN;

    // animation: cycle frames while moving, reset to frame 0 when idle
    bool moving = (in.move_x != 0 || in.move_y != 0);
    if (moving) {
        p->anim_time += dt;
        if (p->anim_time > 0.15f) {
            p->anim_time = 0;
            p->anim_frame = (p->anim_frame + 1) % 4;
        }
    } else {
        p->anim_frame = 0;
        p->anim_time = 0;
    }

    // bomb
    if (in.place_bomb && p->bombs_active < p->max_bombs) {
        p->bombs_active++;
    }
}

void player_draw(const Player *p) {
    if (!p->alive) return;

    Rectangle src = {
        (float)(p->anim_frame * PLAYER_SIZE),
        (float)(p->facing * PLAYER_FRAME_H),
        (float)PLAYER_SIZE,
        (float)PLAYER_FRAME_H,
    };
    Rectangle dst = {
        p->x,
        p->y,
        p->width,
        p->height,
    };
    DrawTexturePro(player_tex, src, dst, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

void player_bounds(const Player *p, float *x, float *y, float *w, float *h) {
    *x = p->x;
    *y = p->y;
    *w = p->width;
    *h = p->height;
}

void player_kill(Player *p) {
    p->alive = false;
}