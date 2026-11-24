#ifndef CONFIG_H
#define CONFIG_H

/* Map dimensions (in tiles) */
#define MAP_ROWS 13
#define MAP_COLS 17

/* Rendering */
#define SRC_TILE_PX  16
#define SCALE        2.5f
#define TILE_SIZE    ((float)SRC_TILE_PX * SCALE)
#define SCREEN_W     ((int)(MAP_COLS * TILE_SIZE))
#define SCREEN_H     ((int)(MAP_ROWS * TILE_SIZE))

#define WINDOW_TITLE "raylib"
#define TARGET_FPS   60
#define TILESET_PATH "assets/tiles.png"

/* Level generation */
#define BLOCK_CHANCE_PERCENT 75

#endif /* CONFIG_H */