#include "game.h"
#include "raylib.h"
 
int main(void) {
    game_init();
 
    while (!WindowShouldClose()) {
        game_update();
        game_draw();
    }
 
    game_shutdown();
    return 0;
}
 