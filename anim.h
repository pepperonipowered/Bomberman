#ifndef ANIM_H
#define ANIM_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Data ---------- */

/* Fires when playback enters frame_index (an index into the clip's frames[]). */
typedef struct AnimEvent {
    int frame_index;
    int id;                    /* your own enum value, e.g. EVT_HIT */
} AnimEvent;

typedef struct AnimClip {
    const int       *frames;       /* cell indices into the sheet */
    int              frame_count;
    float            fps;          /* used when durations == NULL */
    const float     *durations;    /* optional per-frame seconds, length frame_count */
    bool             loop;
    const AnimEvent *events;       /* optional */
    int              event_count;
} AnimClip;

/* ---------- Player (pure logic) ---------- */

typedef void (*AnimEventFn)(int event_id, void *user);

typedef struct AnimPlayer {
    const AnimClip *clip;
    int             index;         /* current position in clip->frames */
    float           time;          /* seconds spent on current frame */
    float           speed;         /* 1.0 = normal */
    bool            finished;      /* set by non-looping clips */
    AnimEventFn     on_event;
    void           *user;
} AnimPlayer;

void anim_init(AnimPlayer *p, const AnimClip *clip);
void anim_play(AnimPlayer *p, const AnimClip *clip, bool restart);
void anim_update(AnimPlayer *p, float dt);
int  anim_frame(const AnimPlayer *p);          /* current cell index */
bool anim_finished(const AnimPlayer *p);

/* ---------- Sheet layout (pure math, still no raylib) ---------- */

typedef struct AnimRect { float x, y, w, h; } AnimRect;

typedef struct AnimGrid {
    int frame_w, frame_h;
    int columns;
    int margin;                    /* border around the whole sheet */
    int spacing;                   /* gap between cells */
} AnimGrid;

AnimRect anim_grid_rect(const AnimGrid *g, int cell);

#ifdef __cplusplus
}
#endif
#endif /* ANIM_H */