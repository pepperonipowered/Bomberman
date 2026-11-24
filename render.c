#include "render.h"
#include "config.h"
#include "map.h"
#include "player.h"
#include "raylib.h"

/* Order matches the columns in the tileset image. */
typedef enum {
    SPR_PILLAR,
    SPR_BLOCK,
    SPR_BORDER,
    SPR_FLOOR,
    SPR_SHADOW
} Sprite;

static Texture2D tileset;
static const Player *render_player;

/* ---- internal helpers -------------------------------------------------- */

static Sprite sprite_for(int row, int col) {
    switch (map_get(row, col)) {
        case TILE_WALL:
            return (col == 0 || col == MAP_COLS - 1) ? SPR_BORDER : SPR_PILLAR;
        case TILE_BLOCK:
            return SPR_BLOCK;
        default:
            /* Floor is shaded if the tile above casts a shadow. */
            return tile_casts_shadow(map_get(row - 1, col)) ? SPR_SHADOW
                                                            : SPR_FLOOR;
    }
}

static void draw_tile(int row, int col) {
    Sprite s = sprite_for(row, col);

    Rectangle src = { (float)(s * SRC_TILE_PX), 0.0f,
                      SRC_TILE_PX, SRC_TILE_PX };
    Rectangle dst = { col * TILE_SIZE, row * TILE_SIZE,
                      TILE_SIZE, TILE_SIZE };

    DrawTexturePro(tileset, src, dst, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

static void draw_map(void) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            draw_tile(r, c);
        }
    }
}

/* ---- public API -------------------------------------------------------- */

void render_init(void) {
    tileset = LoadTexture(TILESET_PATH);
    SetTextureFilter(tileset, TEXTURE_FILTER_POINT); /* keep pixels crisp */
}

void render_set_player(const Player *p) {
    render_player = p;
}

void render_shutdown(void) {
    UnloadTexture(tileset);
}

void render_frame(void) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_map();
        if (render_player) player_draw(render_player);
        DrawFPS(10, 10);
    EndDrawing();
}