#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

typedef enum {
    TILE_VOID,
    TILE_WALL,
    TILE_BLOCK
} Tile;

/* Generates a fresh level. Uses raylib's GetRandomValue(). */
void map_init(void);

/* Out-of-bounds reads return TILE_WALL; out-of-bounds writes are ignored. */
Tile map_get(int row, int col);
void map_set(int row, int col, Tile tile);

bool map_in_bounds(int row, int col);

/* True if the tile blocks light (used for floor shadows). */
bool tile_casts_shadow(Tile tile);

#endif /* MAP_H */