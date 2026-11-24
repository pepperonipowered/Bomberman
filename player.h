#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#define PLAYER_SIZE 16
#define PLAYER_FRAME_H 24

typedef enum {
    DIR_DOWN,
    DIR_UP,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

/* What the player wants to do this frame, independent of the device
 * (keyboard, gamepad, AI) that produced it. */
typedef struct {
    int  move_x;        /* -1, 0, +1 */
    int  move_y;        /* -1, 0, +1 */
    bool place_bomb;
	bool kick_bomb;
	bool pick_up_bomb;
	bool throw_bomb;
} PlayerIntent;

typedef struct {
    /* identity */
    int       id;

    /* position (pixels, top-left of the hitbox) and size */
    float     x;
    float     y;
    float     width;
    float     height;
    Direction facing;

    /* stats (changed by power-ups) */
    float     speed;        /* pixels per second */
    int       max_bombs;
    float     blast_range;  /* pixels, or keep tiles if you prefer */

    /* state */
    int       bombs_active;
    bool      alive;

    /* animation */
    int       anim_frame;   /* current frame in the walk cycle (0-3) */
    float     anim_time;    /* seconds spent on the current frame */
} Player;

/* Sets default stats and places the player at a pixel position. */
void player_init(Player *p, int id, float x, float y);

/* Reads keyboard intent for this frame. Used by game_update. */
PlayerIntent read_keyboard_intent(void);

/* Applies one frame of intent: free movement, collision, bomb placement. */
void player_update(Player *p, PlayerIntent intent, float dt);

void player_draw(const Player *p);

/* Hitbox as plain numbers, so collision code doesn't depend on raylib types. */
void player_bounds(const Player *p, float *x, float *y, float *w, float *h);

/* Called by the game when something happens to the player. */
void player_kill(Player *p);

#endif /* PLAYER_H */