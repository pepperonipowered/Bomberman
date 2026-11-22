#include "raylib.h"
#include "game.h"

#define MAP_ROWS 13
#define MAP_COLS 17

#define SRC_TILE   16
#define SCALE      2.5f
#define TILE_SIZE  (SRC_TILE * SCALE)   // on-screen size
#define SCREEN_W   (MAP_COLS * TILE_SIZE)
#define SCREEN_H   (MAP_ROWS * TILE_SIZE)

typedef enum {
	VOID,
    WALL,
    BLOCK
} Tile;

typedef enum {
    SPR_PILLAR,
    SPR_BLOCK,
    SPR_BORDER,
    SPR_FLOOR,
    SPR_SHADOW
} Sprite;

static Texture2D tiles;
static Tile map[MAP_ROWS * MAP_COLS];


static int in_bounds(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

// the only place the index formula lives
static int index_of(int row, int col) {
    return row * MAP_COLS + col;
}

Tile get_tile(int row, int col) {
    if (!in_bounds(row, col)) return WALL;
    return map[index_of(row, col)];
}

void set_tile(int row, int col, Tile tile) {
    if (!in_bounds(row, col)) return;
    map[index_of(row, col)] = tile;
}

// clears a spawn corner: the corner tile plus one step along each axis
static void clear_spawn(int row, int col, int dr, int dc) {
    set_tile(row, col, VOID);
    set_tile(row + dr, col, VOID);
    set_tile(row, col + dc, VOID);
}


static int casts_shadow(Tile t) {
    return t == WALL || t == BLOCK;
}

static Sprite sprite_for(int row, int col) {
    switch (get_tile(row, col)) {
        case WALL:
            if (col == 0 || col == MAP_COLS - 1) return SPR_BORDER;
            return SPR_PILLAR;
        case BLOCK:
            return SPR_BLOCK;
        default:
            // floor: shaded if the tile above casts a shadow
            return casts_shadow(get_tile(row - 1, col)) ? SPR_SHADOW : SPR_FLOOR;
    }
}


void init_map(void) {
    // pass 1 + 2 + 3: everything empty, then border, then pillars
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            Tile t = VOID;
           if (r == 0 || r == MAP_ROWS - 1 ||
				c <= 1 || c >= MAP_COLS - 2) {
				t = WALL;
			} else if (r % 2 == 0 && c % 2 == 1) {
				t = WALL;
			}
            set_tile(r, c, t);
        }
    }

    // // pass 4: random destructible blocks on empty tiles
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (get_tile(r, c) == VOID && GetRandomValue(1, 100) <= 75) {
                set_tile(r, c, BLOCK);
            }
        }
    }

    // pass 5: clear the four spawn corners
   	clear_spawn(1, 2, 1, 1);
	clear_spawn(1, MAP_COLS - 3, 1, -1);
	clear_spawn(MAP_ROWS - 2, 2, -1, 1);
	clear_spawn(MAP_ROWS - 2, MAP_COLS - 3, -1, -1);
}

void init() {
    InitWindow(SCREEN_W, SCREEN_H, "raylib");
    SetTargetFPS(60);
	init_map();
	tiles = LoadTexture("assets/tiles.png");        // after InitWindow
    SetTextureFilter(tiles, TEXTURE_FILTER_POINT);  // keep pixels crisp
}

void draw() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
			for (int r = 0; r < MAP_ROWS; r++) {
				for (int c = 0; c < MAP_COLS; c++) {
					Sprite s = sprite_for(r, c);

					Rectangle src = { (float)(s * SRC_TILE), 0,
									SRC_TILE, SRC_TILE };
					Rectangle dst = { (float)(c * TILE_SIZE), (float)(r * TILE_SIZE),
									TILE_SIZE, TILE_SIZE };

					DrawTexturePro(tiles, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
				}
			}

        DrawFPS(10, 10);
    EndDrawing();
}

void update() {

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