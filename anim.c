#ifndef ANIM_RAYLIB_H
#define ANIM_RAYLIB_H

#include "raylib.h"
#include "anim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Source rectangle for the player's current frame. */
Rectangle anim_source_rect(const AnimGrid *g, const AnimPlayer *p);

/* Draws the current frame. `origin` is the pivot inside the frame in pixels
   (e.g. {frame_w/2, frame_h} for feet-center); `pos` is where that pivot lands. */
void anim_draw(Texture2D tex, const AnimGrid *g, const AnimPlayer *p,
               Vector2 pos, Vector2 origin, float scale, bool flip_x, Color tint);

#ifdef __cplusplus
}
#endif
#endif /* ANIM_RAYLIB_H */