#include "map.h"
#include "config.h"
#include "raylib.h"

static Tile map[MAP_ROWS * MAP_COLS];

/* ---- internal helpers -------------------------------------------------- */

/* The only place the index formula lives. */
static int index_of(int row, int col) {
    return row * MAP_COLS + col;
}

static bool is_border(int row, int col) {
    return row == 0 || row == MAP_ROWS - 1 ||
           col <= 1 || col >= MAP_COLS - 2;
}

static bool is_pillar(int row, int col) {
    return row % 2 == 0 && col % 2 == 1;
}

/* Clears a spawn corner: the corner tile plus one step along each axis. */
static void clear_spawn(int row, int col, int dr, int dc) {
    map_set(row, col, TILE_VOID);
    map_set(row + dr, col, TILE_VOID);
    map_set(row, col + dc, TILE_VOID);
}

/* ---- generation passes ------------------------------------------------- */

static void generate_layout(void) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            bool wall = is_border(r, c) || is_pillar(r, c);
            map_set(r, c, wall ? TILE_WALL : TILE_VOID);
        }
    }
}

static void scatter_blocks(void) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (map_get(r, c) == TILE_VOID &&
                GetRandomValue(1, 100) <= BLOCK_CHANCE_PERCENT) {
                map_set(r, c, TILE_BLOCK);
            }
        }
    }
}

static void clear_spawn_corners(void) {
    clear_spawn(1,             2,             1,  1);
    clear_spawn(1,             MAP_COLS - 3,  1, -1);
    clear_spawn(MAP_ROWS - 2,  2,            -1,  1);
    clear_spawn(MAP_ROWS - 2,  MAP_COLS - 3, -1, -1);
}

/* ---- public API -------------------------------------------------------- */

void map_init(void) {
    generate_layout();
    scatter_blocks();
    clear_spawn_corners();
}

bool map_in_bounds(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

Tile map_get(int row, int col) {
    if (!map_in_bounds(row, col)) return TILE_WALL;
    return map[index_of(row, col)];
}

void map_set(int row, int col, Tile tile) {
    if (!map_in_bounds(row, col)) return;
    map[index_of(row, col)] = tile;
}

bool tile_casts_shadow(Tile tile) {
    return tile == TILE_WALL || tile == TILE_BLOCK;
}