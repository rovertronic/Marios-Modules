#include "sm64.h"

#ifndef SHITCULL_H
#define SHITCULL_H

struct ShitCullVolume {
    Vec3f pos;
    Vec3f scale;
    u16 flag;
};

void shit_cull_init(void);
void shit_cull_add_volume(Vec3f pos, Vec3f scale, u8 flag);
void shit_cull_update(void);
s32 shit_cull_visible(void);
s32 shit_cull_object_visible(struct Object * obj);

Gfx *geo_shit_cull_id(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);

extern int gShitCullCurrentRenderId;
extern u8 gShitCullDoorIsOpenSignal;

#endif