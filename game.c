#include "game.h"
#include "config.h"
#include "map.h"
#include "player.h"
#include "render.h"
#include "raylib.h"

static Player player;

void game_init(void) {
    InitWindow(SCREEN_W, SCREEN_H, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    map_init();
    player_init(&player, 1, 2 * TILE_SIZE, 1 * TILE_SIZE);
    render_init();   /* needs the window/GL context */
    render_set_player(&player);
}

void game_update(void) {
    PlayerIntent in = read_keyboard_intent();
    player_update(&player, in, 1.0f / TARGET_FPS);
}

void game_draw(void) {
    render_frame();
}

void game_shutdown(void) {
    render_shutdown();
    CloseWindow();
}