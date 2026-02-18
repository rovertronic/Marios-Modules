#include "shitcull.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"

struct ShitCullVolume sShitCullVolumeList[100];
int sShitCullVolumeCount = 0;
u32 sShitCullVisibleFlags = 0;

int gShitCullCurrentRenderId = -1;

u8 gShitCullDoorIsOpenSignal = FALSE;

void shit_cull_init(void) {
    sShitCullVolumeCount = 0;
}

void shit_cull_add_volume(Vec3f pos, Vec3f scale, u8 flag) {
    vec3f_copy(sShitCullVolumeList[sShitCullVolumeCount].pos,pos);
    vec3f_copy(sShitCullVolumeList[sShitCullVolumeCount].scale,scale);
    sShitCullVolumeList[sShitCullVolumeCount].flag = flag;

    sShitCullVolumeCount++;
}

void shit_cull_update(void) {
    f32 doorOpenReach = 0.0f;

    if (!gShitCullDoorIsOpenSignal) {
        // All doors sealed
        sShitCullVisibleFlags = 0;
    } else {
        // At least one door open
        doorOpenReach = 500.0f;
    }

    gShitCullDoorIsOpenSignal = FALSE;

    for (int i = 0; i < sShitCullVolumeCount; i++) {
        int inside = TRUE;
        for (int j = 0; j < 3; j++) {
            if (ABS(sShitCullVolumeList[i].pos[j] - gMarioState->pos[j]) > 100.0f * sShitCullVolumeList[i].scale[j] + doorOpenReach) {
                // Outside box
                inside = FALSE;
            }
        }
        if (inside) {
            sShitCullVisibleFlags |= (1 << sShitCullVolumeList[i].flag);
        }
    }
    
}

void shit_cull_init_object(struct Object * obj) {
    for (int i = 0; i < sShitCullVolumeCount; i++) {
        int inside = TRUE;
        for (int j = 0; j < 3; j++) {
            if (ABS(sShitCullVolumeList[i].pos[j] - ((f32 *)(&obj->oPosX))[j]) > 100.0f * sShitCullVolumeList[i].scale[j] + 200.0f) {
                // Outside box
                inside = FALSE;
            }
        }
        if (inside) {
            obj->shitCullFlags |= (1 << sShitCullVolumeList[i].flag);
        }
    }
}

s32 shit_cull_visible(void) {
    if (gShitCullCurrentRenderId == -1) {
        return TRUE;
    }
    if ((1<<gShitCullCurrentRenderId) & sShitCullVisibleFlags) {
        return TRUE;
    }
    return FALSE;
}

s32 shit_cull_object_visible(struct Object * obj) {
    if (obj->shitCullFlags & sShitCullVisibleFlags) {
        return TRUE;
    }
    return FALSE;
}

Gfx *geo_shit_cull_id(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx) {

    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *currentGraphNode = (struct GraphNodeGenerated *) node;
        s32 parameter = currentGraphNode->parameter;

        gShitCullCurrentRenderId = parameter;
    }

    return NULL;
}