#ifndef RENDER_H
#define RENDER_H

#include "player.h"

/* Call after InitWindow(). */
void render_init(void);
void render_shutdown(void);

/* Set the player to draw each frame. Call after render_init(). */
void render_set_player(const Player *p);

/* Draws the whole frame (call once per frame). */
void render_frame(void);

#endif /* RENDER_H */