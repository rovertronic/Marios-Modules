#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "envfx_bubbles.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_misc_macros.h"
#include "level_table.h"
#include "level_update.h"
#include "levels/bob/header.h"
#include "levels/ttm/header.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_misc.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "spawn_object.h"
#include "spawn_sound.h"
#include "rumble_init.h"
#include "module.h"
#include "levels/temple/header.h"
#include "dungeon.h"

/**
 * @file obj_behaviors.c
 * This file contains a portion of the obj behaviors and many helper functions for those
 * specific behaviors. Few functions besides the bhv_ functions are used elsewhere in the repo.
 */

/**
 * Current object floor as defined in object_step.
 */
static struct Surface *sObjFloor;

/**
 * Set to false when an object close to the floor should not be oriented in reference
 * to it. Happens with boulder, falling pillar, and the rolling snowman body.
 */
static s8 sOrientObjWithFloor = TRUE;

/**
 * Keeps track of Mario's previous non-zero room.
 * Helps keep track of room when Mario is over an object.
 */
s16 sPrevCheckMarioRoom = 0;

/**
 * Tracks whether or not Yoshi has walked/jumped off the roof.
 */
s8 sYoshiDead = FALSE;

extern void *ccm_seg7_trajectory_snowman;
extern void *inside_castle_seg7_trajectory_mips;

/**
 * Resets yoshi as spawned/despawned upon new file select.
 * Possibly a function with stubbed code.
 */
void set_yoshi_as_not_dead(void) {
    sYoshiDead = FALSE;
}

/**
 * An unused geo function. Bears strong similarity to geo_bits_bowser_coloring, and relates something
 * of the opacity of an object to something else. Perhaps like, giving a parent object the same
 * opacity?
 */
Gfx UNUSED *geo_obj_transparency_something(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx) {
    Gfx *gfxHead = NULL;
    Gfx *gfx;

    if (callContext == GEO_CONTEXT_RENDER) {
        struct Object *heldObject = (struct Object *) gCurGraphNodeObject;
        struct Object *obj = (struct Object *) node;


        if (gCurGraphNodeHeldObject != NULL) {
            heldObject = gCurGraphNodeHeldObject->objNode;
        }

        gfxHead = alloc_display_list(3 * sizeof(Gfx));
        gfx = gfxHead;
        SET_GRAPH_NODE_LAYER(obj->header.gfx.node.flags, LAYER_TRANSPARENT);

        gDPSetEnvColor(gfx++, 255, 255, 255, heldObject->oOpacity);

        gSPEndDisplayList(gfx);
    }

    return gfxHead;
}

/**
 * Backwards compatibility, used to be a duplicate function
 */
#define absf_2 absf

/**
 * Turns an object away from floors/walls that it runs into.
 */
void turn_obj_away_from_surface(f32 velX, f32 velZ, f32 nX, UNUSED f32 nY, f32 nZ, f32 *objYawX,
                            f32 *objYawZ) {
    *objYawX = (nZ * nZ - nX * nX) * velX / (nX * nX + nZ * nZ)
               - 2 * velZ * (nX * nZ) / (nX * nX + nZ * nZ);

    *objYawZ = (nX * nX - nZ * nZ) * velZ / (nX * nX + nZ * nZ)
               - 2 * velX * (nX * nZ) / (nX * nX + nZ * nZ);
}

/**
 * Finds any wall collisions, applies them, and turns away from the surface.
 */
s8 obj_find_wall(f32 objNewX, f32 objY, f32 objNewZ, f32 objVelX, f32 objVelZ) {
    struct WallCollisionData hitbox;
    f32 wall_nX, wall_nY, wall_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

    hitbox.x = objNewX;
    hitbox.y = objY;
    hitbox.z = objNewZ;
    hitbox.offsetY = o->hitboxHeight / 2;
    hitbox.radius = o->hitboxRadius;

    if (find_wall_collisions(&hitbox) != 0) {
        o->oPosX = hitbox.x;
        o->oPosY = hitbox.y;
        o->oPosZ = hitbox.z;

        wall_nX = hitbox.walls[0]->normal.x;
        wall_nY = hitbox.walls[0]->normal.y;
        wall_nZ = hitbox.walls[0]->normal.z;

        objVelXCopy = objVelX;
        objVelZCopy = objVelZ;

        // Turns away from the first wall only.
        turn_obj_away_from_surface(objVelXCopy, objVelZCopy, wall_nX, wall_nY, wall_nZ, &objYawX, &objYawZ);

        o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
        return FALSE;
    }

    return TRUE;
}

/**
 * Turns an object away from steep floors, similarly to walls.
 */
s8 turn_obj_away_from_steep_floor(struct Surface *objFloor, f32 floorY, f32 objVelX, f32 objVelZ) {
    f32 floor_nX, floor_nY, floor_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

    if (objFloor == NULL) {
        o->oMoveAngleYaw += 0x8000;
        return FALSE;
    }

    floor_nX = objFloor->normal.x;
    floor_nY = objFloor->normal.y;
    floor_nZ = objFloor->normal.z;

    // If the floor is steep and we are below it (i.e. walking into it), turn away from the floor.
    if (floor_nY < 0.5f && floorY > o->oPosY) {
        objVelXCopy = objVelX;
        objVelZCopy = objVelZ;
        turn_obj_away_from_surface(objVelXCopy, objVelZCopy, floor_nX, floor_nY, floor_nZ, &objYawX, &objYawZ);
        o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
        return FALSE;
    }

    return TRUE;
}

/**
 * Orients an object with the given normals, typically the surface under the object.
 */
void obj_orient_graph(struct Object *obj, f32 normalX, f32 normalY, f32 normalZ) {
    Vec3f surfaceNormals;

    // Passes on orienting certain objects that shouldn't be oriented, like boulders.
    if (!sOrientObjWithFloor) {
        return;
    }

    // Passes on orienting billboard objects, i.e. coins, trees, etc.
    if (obj->header.gfx.node.flags & GRAPH_RENDER_BILLBOARD) {
        return;
    }

    vec3f_set(surfaceNormals, normalX, normalY, normalZ);
    quat_align_with_floor(obj->header.gfx.throwRotation,surfaceNormals);
    obj->oFlags |= OBJ_FLAG_THROW_ROTATION;
}

/**
 * Determines an object's forward speed multiplier.
 */
void calc_obj_friction(f32 *objFriction, f32 floor_nY) {
    if (floor_nY < 0.2f && o->oFriction < 0.9999f) {
        *objFriction = 0;
    } else {
        *objFriction = o->oFriction;
    }
}

/**
 * Updates an objects speed for gravity and updates Y position.
 */
void calc_new_obj_vel_and_pos_y(struct Surface *objFloor, f32 objFloorY, f32 objVelX, f32 objVelZ) {
    f32 floor_nX = objFloor->normal.x;
    f32 floor_nY = objFloor->normal.y;
    f32 floor_nZ = objFloor->normal.z;
    f32 objFriction;

    // Caps vertical speed with a "terminal velocity".
    o->oVelY -= o->oGravity;
    if (o->oVelY > 75.0) {
        o->oVelY = 75.0;
    }
    if (o->oVelY < -75.0) {
        o->oVelY = -75.0;
    }

    o->oPosY += o->oVelY;

    // Snap the object up to the floor.
    if (o->oPosY < objFloorY) {
        o->oPosY = objFloorY;

        // Bounces an object if the ground is hit fast enough.
        if (o->oVelY < -17.5f) {
            o->oVelY = -(o->oVelY / 2);
        } else {
            o->oVelY = 0;
        }
    }

    if ((o->oPosY >= objFloorY) && (o->oPosY < objFloorY + 37)) {
        // Adds horizontal component of gravity for horizontal speed.
        f32 nxz = sqr(floor_nX) + sqr(floor_nZ);
        f32 vel = ((nxz) / (nxz + sqr(floor_nY))) * o->oGravity * 2;
        objVelX += floor_nX * vel;
        objVelZ += floor_nZ * vel;

        if (objVelX < NEAR_ZERO && objVelX > -NEAR_ZERO) objVelX = 0;
        if (objVelZ < NEAR_ZERO && objVelZ > -NEAR_ZERO) objVelZ = 0;

        if (objVelX != 0 || objVelZ != 0) {
            o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
        }

        calc_obj_friction(&objFriction, floor_nY);
        o->oForwardVel = sqrtf(sqr(objVelX) + sqr(objVelZ)) * objFriction;
    }
}

void calc_new_obj_vel_and_pos_y_underwater(struct Surface *objFloor, f32 floorY, f32 objVelX, f32 objVelZ, f32 waterY) {
    f32 floor_nX = objFloor->normal.x;
    f32 floor_nY = objFloor->normal.y;
    f32 floor_nZ = objFloor->normal.z;

    f32 netYAccel = (1.0f - o->oBuoyancy) * (-1.0f * o->oGravity);
    o->oVelY -= netYAccel;

    // Caps vertical speed with a "terminal velocity".
    if (o->oVelY > 75.0f) {
        o->oVelY = 75.0f;
    }
    if (o->oVelY < -75.0f) {
        o->oVelY = -75.0f;
    }

    o->oPosY += o->oVelY;

    // Snap the object up to the floor.
    if (o->oPosY < floorY) {
        o->oPosY = floorY;

        // Bounces an object if the ground is hit fast enough.
        if (o->oVelY < -17.5f) {
            o->oVelY = -(o->oVelY / 2);
        } else {
            o->oVelY = 0;
        }
    }

    // If moving fast near the surface of the water, flip vertical speed? To emulate skipping?
    if (o->oForwardVel > 12.5f && (waterY + 30.0f) > o->oPosY && (waterY - 30.0f) < o->oPosY) {
        o->oVelY = -o->oVelY;
    }

    if ((o->oPosY >= floorY) && (o->oPosY < floorY + 37)) {
        // Adds horizontal component of gravity for horizontal speed.
        f32 nxz = sqr(floor_nX) + sqr(floor_nZ);
        f32 velm = (nxz / (nxz + sqr(floor_nY))) * netYAccel * 2;
        objVelX += floor_nX * velm;
        objVelZ += floor_nZ * velm;
    }

    if (objVelX < NEAR_ZERO && objVelX > -NEAR_ZERO) objVelX = 0;
    if (objVelZ < NEAR_ZERO && objVelZ > -NEAR_ZERO) objVelZ = 0;

    if (o->oVelY < NEAR_ZERO && o->oVelY > -NEAR_ZERO) {
        o->oVelY = 0;
    }

    if (objVelX != 0 || objVelZ != 0) {
        o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
    }

    // Decreases both vertical velocity and forward velocity. Likely so that skips above
    // don't loop infinitely.
    o->oForwardVel = sqrtf(sqr(objVelX) + sqr(objVelZ)) * 0.8f;
    o->oVelY *= 0.8f;
}

/**
 * Updates an objects position from oForwardVel and oMoveAngleYaw.
 */
void obj_update_pos_vel_xz(void) {
    o->oPosX += o->oForwardVel * sins(o->oMoveAngleYaw);
    o->oPosZ += o->oForwardVel * coss(o->oMoveAngleYaw);
}

/**
 * Generates splashes if at surface of water, entering water, or bubbles
 * if underwater.
 */
void obj_splash(s32 waterY, s32 objY) {
    u32 globalTimer = gGlobalTimer;

    // Spawns waves if near surface of water and plays a noise if entering.
    if ((f32)(waterY + 30) > o->oPosY && o->oPosY > (f32)(waterY - 30)) {
        spawn_object(o, MODEL_IDLE_WATER_WAVE, bhvObjectWaterWave);

        if (o->oVelY < -20.0f) {
            cur_obj_play_sound_2(SOUND_OBJ_DIVING_INTO_WATER);
        }
    }

    // Spawns bubbles if underwater.
    if ((objY + 50) < waterY && !(globalTimer & 31)) {
        spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvObjectBubble);
    }
}

/**
 * Generic object move function. Handles walls, water, floors, and gravity.
 * Returns flags for certain interactions.
 */
s16 object_step(void) {
    f32 objX = o->oPosX;
    f32 objY = o->oPosY;
    f32 objZ = o->oPosZ;

    f32 floorY;
    f32 waterY = FLOOR_LOWER_LIMIT_MISC;

    f32 objVelX = o->oForwardVel * sins(o->oMoveAngleYaw);
    f32 objVelZ = o->oForwardVel * coss(o->oMoveAngleYaw);

    s16 collisionFlags = 0;

    // Find any wall collisions, receive the push, and set the flag.
    if (obj_find_wall(objX + objVelX, objY, objZ + objVelZ, objVelX, objVelZ) == 0) {
        collisionFlags += OBJ_COL_FLAG_HIT_WALL;
    }

    floorY = find_floor(objX + objVelX, objY, objZ + objVelZ, &sObjFloor);

    o->oFloor       = sObjFloor;
    o->oFloorHeight = floorY;

    if (turn_obj_away_from_steep_floor(sObjFloor, floorY, objVelX, objVelZ) == 1) {
        waterY = find_water_level(objX + objVelX, objZ + objVelZ);
        if (waterY > objY) {
            calc_new_obj_vel_and_pos_y_underwater(sObjFloor, floorY, objVelX, objVelZ, waterY);
            collisionFlags += OBJ_COL_FLAG_UNDERWATER;
        } else {
            calc_new_obj_vel_and_pos_y(sObjFloor, floorY, objVelX, objVelZ);
        }
    } else {
        // Treat any awkward floors similar to a wall.
        collisionFlags +=
            ((collisionFlags & OBJ_COL_FLAG_HIT_WALL) ^ OBJ_COL_FLAG_HIT_WALL);
    }

    obj_update_pos_vel_xz();

    if (sObjFloor && (o->oPosY >= floorY) && (o->oPosY < floorY + 37)) {
        obj_orient_graph(o, sObjFloor->normal.x, sObjFloor->normal.y, sObjFloor->normal.z);
    }

    if ((s32) o->oPosY == (s32) floorY) {
        collisionFlags += OBJ_COL_FLAG_GROUNDED;
    }

    if ((s32) o->oVelY == 0) {
        collisionFlags += OBJ_COL_FLAG_NO_Y_VEL;
    }

    // Generate a splash if in water.
    obj_splash(waterY, o->oPosY);
    return collisionFlags;
}

/**
 * Takes an object step but does not orient with the object's floor.
 * Used for boulders, falling pillars, and the rolling snowman body.
 */
s16 object_step_without_floor_orient(void) {
    sOrientObjWithFloor = FALSE;
    s16 collisionFlags = object_step();
    sOrientObjWithFloor = TRUE;

    return collisionFlags;
}

/**
 * Uses an object's forward velocity and yaw to move its X, Y, and Z positions.
 * This does accept an object as an argument, though it is always called with `o`.
 */
void obj_move_xyz_using_fvel_and_yaw(struct Object *obj) {
    obj->oVelX = obj->oForwardVel * sins(obj->oMoveAngleYaw);
    obj->oVelZ = obj->oForwardVel * coss(obj->oMoveAngleYaw);

    vec3f_add(&obj->oPosVec, &obj->oVelVec);
}

/**
 * Checks if a point is within distance from Mario's graphical position. Test is exclusive.
 */
s32 is_point_within_radius_of_mario(f32 x, f32 y, f32 z, s32 dist) {
    f32 dx = x - gMarioObject->header.gfx.pos[0];
    f32 dy = y - gMarioObject->header.gfx.pos[1];
    f32 dz = z - gMarioObject->header.gfx.pos[2];

    return sqr(dx) + sqr(dy) + sqr(dz) < (f32)sqr(dist);
}

/**
 * Checks whether a point is within distance of a given point. Test is exclusive.
 */
s32 is_point_close_to_object(struct Object *obj, f32 x, f32 y, f32 z, s32 dist) {
    f32 dx = x - obj->oPosX;
    f32 dy = y - obj->oPosY;
    f32 dz = z - obj->oPosZ;

    return sqr(dx) + sqr(dy) + sqr(dz) < (f32)sqr(dist);
}

/**
 * Sets an object as visible if within a certain distance of Mario's graphical position.
 */
void set_object_visibility(struct Object *obj, s32 dist) {
    COND_BIT(
        !is_point_within_radius_of_mario(obj->oPosX, obj->oPosY, obj->oPosZ, dist),
        obj->header.gfx.node.flags,
        GRAPH_RENDER_INVISIBLE
    );
}

/**
 * Turns an object towards home if Mario is not near to it.
 */
s32 obj_return_home_if_safe(struct Object *obj, f32 homeX, f32 y, f32 homeZ, s32 dist) {
    f32 homeDistX = homeX - obj->oPosX;
    f32 homeDistZ = homeZ - obj->oPosZ;
    s16 angleTowardsHome = atan2s(homeDistZ, homeDistX);

    if (is_point_within_radius_of_mario(homeX, y, homeZ, dist)) {
        return TRUE;
    } else {
        obj->oMoveAngleYaw = approach_s16_symmetric(obj->oMoveAngleYaw, angleTowardsHome, 320);
    }

    return FALSE;
}

/**
 * Randomly displaces an objects home if RNG says to, and turns the object towards its home.
 */
void obj_return_and_displace_home(struct Object *obj, f32 homeX, UNUSED f32 homeY, f32 homeZ, s32 baseDisp) {
    s16 angleToNewHome;
    f32 homeDistX, homeDistZ;

    if ((s32)(random_float() * 50.0f) == 0) {
        obj->oHomeX = (f32)(baseDisp * 2) * random_float() - (f32) baseDisp + homeX;
        obj->oHomeZ = (f32)(baseDisp * 2) * random_float() - (f32) baseDisp + homeZ;
    }

    homeDistX = obj->oHomeX - obj->oPosX;
    homeDistZ = obj->oHomeZ - obj->oPosZ;
    angleToNewHome = atan2s(homeDistZ, homeDistX);
    obj->oMoveAngleYaw = approach_s16_symmetric(obj->oMoveAngleYaw, angleToNewHome, 320);
}

/**
 * A series of checks using sin and cos to see if a given angle is facing in the same direction
 * of a given angle, within a certain range.
 */
s32 obj_check_if_facing_toward_angle(u32 base, u32 goal, s16 range) {
    s16 dAngle = (u16) goal - (u16) base;

    if (((f32) sins(-range) < (f32) sins(dAngle)) && ((f32) sins(dAngle) < (f32) sins(range))
        && (coss(dAngle) > 0)) {
        return TRUE;
    }

    return FALSE;
}

/**
 * Finds any wall collisions and returns what the displacement vector would be.
 */
s32 obj_find_wall_displacement(Vec3f dist, f32 x, f32 y, f32 z, f32 radius) {
    struct WallCollisionData hitbox;
    hitbox.x = x;
    hitbox.y = y;
    hitbox.z = z;
    hitbox.offsetY = 10.0f;
    hitbox.radius = radius;

    if (find_wall_collisions(&hitbox) != 0) {
        dist[0] = hitbox.x - x;
        dist[1] = hitbox.y - y;
        dist[2] = hitbox.z - z;
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Spawns a number of coins at the location of an object
 * with a random forward velocity, y velocity, and direction.
 */
void obj_spawn_yellow_coins(struct Object *obj, s8 nCoins) {
    struct Object *coin;
    s8 count;

    for (count = 0; count < nCoins; count++) {
        coin = spawn_object(obj, MODEL_YELLOW_COIN, bhvMovingYellowCoin);
        coin->oForwardVel = random_float() * 20;
        coin->oVelY = random_float() * 40 + 20;
        coin->oMoveAngleYaw = random_u16();
    }
}

/**
 * Controls whether certain objects should flicker/when to despawn.
 */
s32 obj_flicker_and_disappear(struct Object *obj, s16 lifeSpan) {
    if (obj->oTimer < lifeSpan) {
        return FALSE;
    }

    if (obj->oTimer < lifeSpan + 40) {
        COND_BIT((obj->oTimer & 0x1), obj->header.gfx.node.flags, GRAPH_RENDER_INVISIBLE);
    } else {
        obj->activeFlags = ACTIVE_FLAG_DEACTIVATED;
        return TRUE;
    }

    return FALSE;
}

/**
 * Checks if a given room is Mario's current room, even if on an object.
 */
s32 current_mario_room_check(RoomData room) {
    s32 result;

    // Since object surfaces have room 0, this tests if the surface is an
    // object first and uses the last room if so.
    if (gMarioCurrentRoom == 0) {
        return room == sPrevCheckMarioRoom;
    } else {
        result = room == gMarioCurrentRoom;

        sPrevCheckMarioRoom = gMarioCurrentRoom;
    }

    return result;
}

/**
 * Triggers dialog when Mario is facing an object and controls it while in the dialog.
 */
s32 trigger_obj_dialog_when_facing(s32 *inDialog, s16 dialogID, f32 dist, s32 actionArg) {
    if ((is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, (s32) dist)
         && obj_check_if_facing_toward_angle(o->oFaceAngleYaw, gMarioObject->header.gfx.angle[1] + 0x8000, 0x1000)
         && obj_check_if_facing_toward_angle(o->oMoveAngleYaw, o->oAngleToMario, 0x1000))
        || (*inDialog == TRUE)) {
        *inDialog = TRUE;

        if (set_mario_npc_dialog(actionArg) == MARIO_DIALOG_STATUS_SPEAK) { // If Mario is speaking.
            s16 dialogResponse = cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogID);
            if (dialogResponse != DIALOG_RESPONSE_NONE) {
                set_mario_npc_dialog(MARIO_DIALOG_STOP);
                *inDialog = FALSE;
                return dialogResponse;
            }
            return DIALOG_RESPONSE_NONE;
        }
    }

    return DIALOG_RESPONSE_NONE;
}

/**
 *Checks if a floor is one that should cause an object to "die".
 */
void obj_check_floor_death(s16 collisionFlags, struct Surface *floor) {
    if (floor == NULL) {
        return;
    }

    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
        switch (floor->type) {
            case SURFACE_BURNING:
                o->oAction = OBJ_ACT_LAVA_DEATH;
                break;
            case SURFACE_VERTICAL_WIND:
            case SURFACE_DEATH_PLANE:
                o->oAction = OBJ_ACT_DEATH_PLANE_DEATH;
                break;
            default:
                break;
        }
    }
}

/**
 * Controls an object dying in lava by creating smoke, sinking the object, playing
 * audio, and eventually despawning it. Returns TRUE when the obj is dead.
 */
s32 obj_lava_death(void) {
    struct Object *deathSmoke;

    if (o->oTimer > 30) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
        return TRUE;
    } else {
        // Sinking effect
        o->oPosY -= 10.0f;
    }

    if ((o->oTimer % 8) == 0) {
        cur_obj_play_sound_2(SOUND_OBJ_BULLY_EXPLODE_LAVA);
        deathSmoke = spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        deathSmoke->oPosX += random_float() * 20.0f;
        deathSmoke->oPosY += random_float() * 20.0f;
        deathSmoke->oPosZ += random_float() * 20.0f;
        deathSmoke->oForwardVel = random_float() * 10.0f;
    }

    return FALSE;
}

/**
 * Spawns an orange number object relatively, such as those that count up for secrets.
 */
void spawn_orange_number(s8 behParam, s16 relX, s16 relY, s16 relZ) {
#ifdef DIALOG_INDICATOR
    if (behParam > ORANGE_NUMBER_F) return;
#else
    if (behParam > ORANGE_NUMBER_9) return;
#endif

    struct Object *orangeNumber = spawn_object_relative(behParam, relX, relY, relZ, o, MODEL_NUMBER, bhvOrangeNumber);
    orangeNumber->oPosY += 25.0f;
    orangeNumber->oOrangeNumberOffset = relX;
    orangeNumber->oHomeX = o->oPosX;
    orangeNumber->oHomeZ = o->oPosZ;
}

/**
 * Unused variables for debug_sequence_tracker.
 */
s8 sDebugSequenceTracker = 0;
s8 sDebugTimer = 0;

/**
 * Unused presumably debug function that tracks for a sequence of inputs.
 */
UNUSED s32 debug_sequence_tracker(s16 debugInputSequence[]) {
    // If end of sequence reached, return true.
    if (debugInputSequence[sDebugSequenceTracker] == 0) {
        sDebugSequenceTracker = 0;
        return TRUE;
    }

    // If the button pressed is next in sequence, reset timer and progress to next value.
    if (debugInputSequence[sDebugSequenceTracker] & gPlayer1Controller->buttonPressed) {
        sDebugSequenceTracker++;
        sDebugTimer = 0;
    // If wrong input or timer reaches 10, reset sequence progress.
    } else if (sDebugTimer == 10 || gPlayer1Controller->buttonPressed != 0) {
        sDebugSequenceTracker = 0;
        sDebugTimer = 0;
        return FALSE;
    }
    sDebugTimer++;

    return FALSE;
}

#include "behaviors/moving_coin.inc.c"
#include "behaviors/seaweed.inc.c"
#include "behaviors/bobomb.inc.c"
#include "behaviors/cannon_door.inc.c"
#include "behaviors/whirlpool.inc.c"
#include "behaviors/amp.inc.c"
#include "behaviors/butterfly.inc.c"
#include "behaviors/hoot.inc.c"
#include "behaviors/beta_holdable_object.inc.c"
#include "behaviors/bubble.inc.c"
#include "behaviors/water_wave.inc.c"
#include "behaviors/explosion.inc.c"
#include "behaviors/respawner.inc.c"
#include "behaviors/bully.inc.c"
#include "behaviors/water_ring.inc.c"
#include "behaviors/bowser_bomb.inc.c"
#include "behaviors/celebration_star.inc.c"
#include "behaviors/drawbridge.inc.c"
#include "behaviors/bomp.inc.c"
#include "behaviors/sliding_platform.inc.c"
#include "behaviors/moneybag.inc.c"
#include "behaviors/bowling_ball.inc.c"
#include "behaviors/cruiser.inc.c"
#include "behaviors/spindel.inc.c"
#include "behaviors/pyramid_wall.inc.c"
#include "behaviors/pyramid_elevator.inc.c"
#include "behaviors/pyramid_top.inc.c"
#include "behaviors/sound_waterfall.inc.c"
#include "behaviors/sound_volcano.inc.c"
#include "behaviors/castle_flag.inc.c"
#include "behaviors/sound_birds.inc.c"
#include "behaviors/sound_ambient.inc.c"
#include "behaviors/sound_sand.inc.c"
#include "behaviors/castle_cannon_grate.inc.c"
#include "behaviors/snowman.inc.c"
#include "behaviors/boulder.inc.c"
#include "behaviors/cap.inc.c"
#include "behaviors/koopa_shell.inc.c"
#include "behaviors/spawn_star.inc.c"
#include "behaviors/red_coin.inc.c"
#include "behaviors/hidden_star.inc.c"
#include "behaviors/rolling_log.inc.c"
#include "behaviors/mushroom_1up.inc.c"
#include "behaviors/controllable_platform.inc.c"
#include "behaviors/breakable_box_small.inc.c"
#include "behaviors/snow_mound.inc.c"
#include "behaviors/floating_platform.inc.c"
#include "behaviors/arrow_lift.inc.c"
#include "behaviors/orange_number.inc.c"
#include "behaviors/manta_ray.inc.c"
#include "behaviors/falling_pillar.inc.c"
#include "behaviors/floating_box.inc.c"
#include "behaviors/decorative_pendulum.inc.c"
#include "behaviors/treasure_chest.inc.c"
#include "behaviors/mips.inc.c"
#include "behaviors/yoshi.inc.c"

extern u8 world_module_timer;
extern Vec3f world_module_pos;
extern s8 world_module_id;

s8 gCutsceneCameraId = -1;
s8 gCutsceneRoom = -1;
s8 gButtonPressId = -1;
void bhv_cutscene_camera(void) {
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
     if (gCutsceneCameraId == o->oBehParams2ndByte) {
        gCutsceneRoom = get_room_at_pos(o->oPosX, o->oPosY, o->oPosZ);
        gCamera->cutscene = 1;
        vec3f_copy(gLakituState.goalPos,&o->oPosVec);
        gLakituState.goalFocus[0] = o->oPosX + sins(o->oFaceAngleYaw) * coss(o->oFaceAnglePitch) * 5.0f;
        gLakituState.goalFocus[1] = o->oPosY + sins(o->oFaceAnglePitch) * -5.0f;
        gLakituState.goalFocus[2] = o->oPosZ + coss(o->oFaceAngleYaw) * coss(o->oFaceAnglePitch) * 5.0f;

        if (o->oTimer >= 60 + GET_BPARAM4(o->oBehParams)) {
            gCutsceneRoom = -1;
            gCutsceneCameraId = -1;
            gCamera->cutscene = 0;
            disable_time_stop_including_mario();
        }
     } else {
        o->oTimer = 0;
     }
}

void bhv_asriel_cage(void) {
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    switch(o->oAction) {
        case 0:
            if (gButtonPressId == 0) {
                o->oAction = 1;
                gCutsceneCameraId = 0;
                enable_time_stop_including_mario();
            }
            break;
        case 1:
            o->oPosY += 15.0f;
            cur_obj_play_sound_1(SOUND_MOVING_AIM_CANNON);
            if (o->oTimer > 30) {
                o->oAction = 2;
                gButtonPressId = -1;
            }
            break;
    }
}

void bhv_chest_price_number(void) {
    u8 cost = GET_BPARAM1(o->parentObj->oBehParams);
    u8 place = GET_BPARAM3(o->oBehParams);

    f32 offset = -28.0f;
    o->oAnimState = cost/10;
    if (place == 1) {
        o->oAnimState = (cost%10);
        offset = 28.0f;
    }
    if (cost < 10) {
        offset = 0.0f;
    }
    if (place == 2) {
        //coin symbol
        o->oAnimState = 10;
        if (cost < 10) {
            offset = -65.0f;
        } else {
            offset = -95.0f;
        }
    }
    o->oPosX = o->parentObj->oPosX + sins(gCamera->nextYaw + 0x4000) * offset;
    o->oPosZ = o->parentObj->oPosZ + coss(gCamera->nextYaw + 0x4000) * offset;
    
    o->oPosY = o->parentObj->oPosY + 200.0f;

    if (o->oTimer > 0) {
        obj_mark_for_deletion(o);
    }
}

extern struct module_info module_infos[];
s8 lootTableVanity[] = {MOD_VAN_CAP,MOD_VAN_PANTS,MOD_VAN_HAIR,MOD_RED,MOD_BLUE,MOD_GREEN,MOD_YELLOW,MOD_BLACK,MOD_WHITE};
// 2x jump, 1x upg+1, 1x ground upg, 1x attack, 1x ground, 1x wall, 2x timer, 2x input, 1x heat sink, 1x down, 3x if conditions, 1x rotate, 1x time extend
s8 lootTableTier1[] = {MOD_JUMP, MOD_JUMP, MOD_POW, MOD_GROUND_UPG, MOD_ATTACK, MOD_HIT_GROUND, MOD_HIT_WALL, MOD_TIMER, MOD_TIMER, MOD_INPUT, MOD_INPUT, MOD_COOL, MOD_GRAV, MOD_IF_FLOOR, MOD_IF_DOWN, MOD_IF_INPUT, MOD_ROTATE, MOD_TIME_EXTEND};
//1x hover module, 1x repeat, 1x cap module, 1x tornado, 1x crouchact, 1x upg+2, 1x grav flip, 1x defense
s8 lootTableTier2[] = {MOD_PLATFORM, MOD_REPEAT, MOD_CAP, MOD_TORNADO, MOD_ZACTION, MOD_POW2, MOD_FLIP_VEL, MOD_DEFENSE};

s8 sModuleChestLabelBuffer[10];
void bhv_moduleLabel(void) {
    if (GET_BPARAM4(o->oBehParams) > 0) {
        o->oBehParams2ndByte = sModuleChestLabelBuffer[GET_BPARAM4(o->oBehParams)];
    }
}

void obj_show_price(s8 cost) {
    struct Object * digit;
    if (cost >= 10) {
        digit = spawn_object(o,MODEL_NUMBER,bhvChestPriceNumber);
        SET_BPARAM3(digit->oBehParams,0);
    }
    digit = spawn_object(o,MODEL_NUMBER,bhvChestPriceNumber);
    SET_BPARAM3(digit->oBehParams,1);

    digit = spawn_object(o,MODEL_NUMBER,bhvChestPriceNumber);
    SET_BPARAM3(digit->oBehParams,2);
}

void bhv_chest(void) {
    u8 cost = GET_BPARAM1(o->oBehParams);

    switch(o->oAction) {
        case 0:
            o->oChestSeed = gMariosModulesSave.file[gMariosModulesSaveIndex].seed;

            obj_save_bin_count(SAVE_BIN_CHESTS);

            if (GET_BPARAM3(o->oBehParams) > 0) {
                s8 randomModule;

                s8 * lootTable = lootTableTier1;
                u8 lootCount = sizeof(lootTableTier1);

                randomModule = lootTable[tinymt32_generate_u32(&gGlobalRandomState)%lootCount];

                o->oBehParams2ndByte = randomModule;
                sModuleChestLabelBuffer[GET_BPARAM4(o->oBehParams)] = randomModule;
            }

            o->oAction = 1;
            if (obj_save_bin_read()) {
                o->oAction = 3;
            }
            break;
        case 1:
            if (cost > 0) {
                cur_obj_set_model(MODEL_CCHEST);
                if (o->oDistanceToMario < 400.0f) {
                    obj_show_price(cost);
                }
            }

            o->header.gfx.animInfo.animFrame = 0;
            o->header.gfx.animInfo.animFrameF = 0.0f;
            o->header.gfx.animInfo.animAccelF = 0.0f;
            if ((gMarioState->numCoins >= cost) && (o->oInteractStatus & INT_STATUS_INTERACTED)) {
                obj_save_bin_write(o);

                o->header.gfx.animInfo.animAccelF = 1.0f;
                gMarioState->numCoins-=cost;
                gHudDisplay.coins = gMarioState->numCoins;
                play_sound(SOUND_GENERAL_OPEN_CHEST, o->header.gfx.cameraToObject);
                o->oAction = 2;

                struct Object * moduleCollect = spawn_object(o,MODEL_MODULE,bhvModuleCollect);
                moduleCollect->oBehParams2ndByte = o->oBehParams2ndByte;


                switch(o->oBehParams2ndByte) {
                    case MOD_NONMOD_KEY:
                        gMarioState->numKeys++;
                        break;
                    case MOD_PASSIVE:
                        gMariosModulesSave.file[gMariosModulesSaveIndex].flags |= SAVE_FLAG_PASSIVE;
                        break;
                    default:
                        add_inventory(o->oBehParams2ndByte);
                        break;
                }
            }
            break;
        case 2:
            if (o->oTimer > 30) {
                display_module_message(o->oBehParams2ndByte);

                if (gModuleTutorialState == TUTORIAL_WAIT_FOR_MODULE_COLLECT) {
                    display_tutorial_message("Press @R@START@@ to open your inventory.",TUTORIAL_PRESS_START);
                    gModuleTutorialState = TUTORIAL_PRESS_START;
                }

                o->oAction = 3;
            }
            break;
    }

    obj_element_enemy_loop();

    if (gMariosModulesSave.file[gMariosModulesSaveIndex].seed != o->oChestSeed) {
        // Redo init on seed change
        o->oAction = 0;
    }
}

void bhv_mystery_chest(void) {
    u8 cost = GET_BPARAM1(o->oBehParams);

    switch(o->oAction) {
        case 0:
            o->oChestSeed = gMariosModulesSave.file[gMariosModulesSaveIndex].seed;

            obj_element_init(o,ELEMENT_NORMAL,100.0f);
            obj_save_bin_count(SAVE_BIN_CHESTS);

            // Normal loot
            s8 randomModule;
            s8 firstPick;

            s8 * lootTable = lootTableTier1;
            u8 lootCount = sizeof(lootTableTier1);
            if (o->oBehParams2ndByte == 1) {
                lootTable = lootTableTier2;
                lootCount = sizeof(lootTableTier2);  
            }

            // Normal Loot
            randomModule = lootTable[tinymt32_generate_u32(&gGlobalRandomState)%lootCount];
            SET_BPARAM1(o->oMysteryChestContents, randomModule);
            firstPick = randomModule;
            do {
                randomModule = lootTable[tinymt32_generate_u32(&gGlobalRandomState)%lootCount];
            } while (randomModule == firstPick
            || module_infos[randomModule].type == module_infos[firstPick].type);
            SET_BPARAM2(o->oMysteryChestContents, randomModule);

            // Vanity Loot
            randomModule = tinymt32_generate_u32(&gGlobalRandomState)%sizeof(lootTableVanity);
            SET_BPARAM3(o->oMysteryChestContents, lootTableVanity[randomModule]);
            firstPick = randomModule;
            do {
                randomModule = tinymt32_generate_u32(&gGlobalRandomState)%sizeof(lootTableVanity);
            } while (randomModule == firstPick);
            SET_BPARAM4(o->oMysteryChestContents, lootTableVanity[randomModule]);

            o->oAction = 1;
            if (obj_save_bin_read()) {
                o->oAction = 5;
            }
            break;
        case 1:
            o->header.gfx.animInfo.animFrame = 0;
            o->header.gfx.animInfo.animFrameF = 0.0f;
            o->header.gfx.animInfo.animAccelF = 0.0f;

            if (o->oDistanceToMario < 400.0f && cost > 0) {
                obj_show_price(cost);
            }

            if ((gMarioState->numCoins >= cost) && (o->oInteractStatus & INT_STATUS_INTERACTED)) {
                obj_save_bin_write(o);
                set_mario_action(gMarioState,ACT_WAITING_FOR_DIALOG,0);

                gMarioState->numCoins-=cost;
                gHudDisplay.coins = gMarioState->numCoins;

                o->header.gfx.animInfo.animAccelF = 1.0f;
                play_sound(SOUND_GENERAL_OPEN_CHEST, o->header.gfx.cameraToObject);
                o->oAction = 2;

                gMysteryModuleState = 1;
                gMysteryModuleChoice[0] = GET_BPARAM1(o->oMysteryChestContents);
                gMysteryModuleChoice[1] = GET_BPARAM2(o->oMysteryChestContents);
            }
            break;
        case 2:
            if ((gMysteryModuleState >= 3) && gPlayer1Controller->buttonPressed & B_BUTTON) {
                gMysteryModuleState = 0;
                set_mario_action(gMarioState,ACT_IDLE,0);
                o->oAction = 5;
                break;
            }
            s8 choice = MOD_EMPTY;
            gMysteryModuleSelection = -1;
            if (gPlayer1Controller->rawStickX < -20) {
                gMysteryModuleSelection = 0;
                choice = GET_BPARAM1(o->oMysteryChestContents);
            }
            if (gPlayer1Controller->rawStickX > 20) {
                gMysteryModuleSelection = 1;
                choice = GET_BPARAM2(o->oMysteryChestContents);
            }
            if (choice != MOD_EMPTY && (gPlayer1Controller->buttonPressed & A_BUTTON)) {
                o->oAction = 3;
                add_inventory(choice);

                struct Object * moduleCollect = spawn_object(o,MODEL_MODULE,bhvModuleCollect);
                moduleCollect->oBehParams2ndByte = choice;
                o->oBehParams2ndByte = choice;

                gMysteryModuleState++;
            }
            break;
        case 3:
            if (o->oTimer > 30) {
                display_module_message(o->oBehParams2ndByte);
                o->oAction = 4;
            }
            break;
        case 4:
            if (gMysteryModuleState == 2) {
                gMysteryModuleState = 3;
                o->oAction = 2;
        
                SET_BPARAM1(o->oMysteryChestContents,GET_BPARAM3(o->oMysteryChestContents));
                SET_BPARAM2(o->oMysteryChestContents,GET_BPARAM4(o->oMysteryChestContents));

                gMysteryModuleChoice[0] = GET_BPARAM1(o->oMysteryChestContents);
                gMysteryModuleChoice[1] = GET_BPARAM2(o->oMysteryChestContents);
            } else {
                gMysteryModuleState = 0;
                set_mario_action(gMarioState,ACT_IDLE,0);
                o->oAction = 5;
            }
            break;
    }

    if (gMariosModulesSave.file[gMariosModulesSaveIndex].seed != o->oChestSeed) {
        // Redo init on seed change
        o->oAction = 0;
    }
}

void bhv_recycle_chest(void) {
    switch(o->oAction) {
        case 0:
            o->header.gfx.animInfo.animAccelF = 1.0f;
            o->oAction = 3;
            break;
        case 1:
            o->header.gfx.animInfo.animFrame = 0;
            o->header.gfx.animInfo.animFrameF = 0.0f;
            o->header.gfx.animInfo.animAccelF = 0.0f;
            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                obj_save_bin_write(o);
                o->header.gfx.animInfo.animAccelF = 1.0f;

                play_sound(SOUND_GENERAL_OPEN_CHEST, o->header.gfx.cameraToObject);
                o->oAction = 2;

                struct Object * moduleCollect = spawn_object(o,MODEL_MODULE,bhvModuleCollect);
                moduleCollect->oBehParams2ndByte = gRecycleChestContent;

                add_inventory(gRecycleChestContent);
            }
            break;
        case 2:
            if (o->oTimer > 30) {
                display_module_message(gRecycleChestContent);
                gRecycleChestContent = MOD_EMPTY;
                o->oAction = 3;
            }
            break;
        case 3: // No items
            if (gRecycleChestContent != MOD_EMPTY) {
                o->header.gfx.animInfo.animAccelF = -1.0f;
                
                if (o->header.gfx.animInfo.animFrameF <= 0.0f) {
                    o->oAction = 1;
                }
            }
            break;
    }
}

void bhv_hover(void) {
    cur_obj_scale(1.0f + (o->oBehParams2ndByte * .5f));
    s16 time = 30 + (GET_BPARAM4(o->oBehParams) * 30);
    o->oCollisionDistance += 100.0f * o->oBehParams2ndByte;
    if (o->oAction == 0) {
        if (o->oOpacity < 250) {
            o->oOpacity = approach_f32_asymptotic(o->oOpacity,255,0.3f);
        } else {
            o->oAction = 1;
        }
        load_object_collision_model();
    } else {
        if (o->oTimer >= time) {
            o->oOpacity *= .7f;
            if (o->oOpacity < 5) {
                obj_mark_for_deletion(o);
            }
        } else {
            load_object_collision_model();
        }
    }
}

s8 dungeon_seq_change = -1;
s8 dungeon_seq_cur = -1;
u8 dungeon_seq_timer = 0;

extern void seq_player_fade_to_target_volume(s32 player, s32 fadeDuration, u8 targetVolume);

u8 sForceDoorShut = FALSE;
s16 spline_seg = 0;
f32 spline_prog = 0;
void bhv_dungeon_manager(void) {
    if (gMainMenuState != MAIN_MENU_CLOSED) {
        gCamera->cutscene = 1;
        if (gMainMenuState != MAIN_MENU_OPENING_CUTSCENE) {
            spline_prog = 0;
            spline_seg = 0;
        }
        if (move_point_along_spline(gLakituState.goalPos,segmented_to_virtual(temple_area_1_spline_ic_pos),&spline_seg,&spline_prog)) {
            gCamera->cutscene = 0;
            gMainMenuState = MAIN_MENU_CLOSED;
        }
        move_point_along_spline(gLakituState.goalFocus,segmented_to_virtual(temple_area_1_spline_ic_foc),&spline_seg,&spline_prog);
    }

    if (dungeon_seq_change != dungeon_seq_cur) {
        dungeon_seq_timer++;
        if (dungeon_seq_timer > 120) {
            stop_background_music(SEQUENCE_ARGS(4, dungeon_seq_cur));
            play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, dungeon_seq_change), 0);
            seq_player_fade_to_target_volume(SEQ_PLAYER_LEVEL,100,255);
            dungeon_seq_cur = dungeon_seq_change;
            dungeon_seq_timer = 0;
        }
    }

    sForceDoorShut = FALSE;
}

void bhv_volume(void) {
    f32 scale = (GET_BPARAM4(o->oBehParams)+1)*400.0f;
    if (
        (ABS(gMarioState->pos[0] - o->oPosX) < scale)&&
        (ABS(gMarioState->pos[1] - o->oPosY) < scale)&&
        (ABS(gMarioState->pos[2] - o->oPosZ) < scale)
    ) {
        switch(o->oBehParams2ndByte) {
            case VOLUME_RESPAWN:
                ;struct Object * respawn = cur_obj_nearest_object_with_behavior(bhvDeathWarp);
                if (respawn) {
                    f32 y = find_floor_height(gMarioState->pos[0],gMarioState->pos[1],gMarioState->pos[2]);
                    respawn->oPosX = gMarioState->pos[0];
                    respawn->oPosY = y;
                    respawn->oPosZ = gMarioState->pos[2];
                    respawn->oFaceAngleYaw = gMarioState->faceAngle[1];
                    respawn->oMoveAngleYaw = gMarioState->faceAngle[1];
                }
                break;
            case VOLUME_SCUTTLE_BATTLE:
                {
                    struct Object * enemy = cur_obj_nearest_object_with_behavior(bhvScuttlebug);
                    if (enemy) {
                        sForceDoorShut = TRUE;
                    }
                }
                break;
            case VOLUME_SNUFIT_BATTLE:
                {
                    struct Object * enemy = cur_obj_nearest_object_with_behavior(bhvSnufit);
                    if (enemy && GET_BPARAM4(enemy->oBehParams) == 1) {
                        sForceDoorShut = TRUE;
                    }
                }
                break;
            case VOLUME_SEQ_CHANGE:
                if (dungeon_seq_timer == 0) {
                    if (dungeon_seq_change != GET_BPARAM1(o->oBehParams)) {
                        dungeon_seq_change = GET_BPARAM1(o->oBehParams);
                        dungeon_seq_timer = 0;
                        if (dungeon_seq_cur == -1) {
                            dungeon_seq_timer = 120;
                        }
                        //fadeout_level_music(1000);
                        seq_player_fade_to_target_volume(SEQ_PLAYER_LEVEL,600,0);
                    }
                } else {
                    if (dungeon_seq_change != GET_BPARAM1(o->oBehParams)) {
                        dungeon_seq_change = GET_BPARAM1(o->oBehParams);
                        stop_background_music(SEQUENCE_ARGS(4, dungeon_seq_cur));
                        play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, dungeon_seq_change), 0);
                        seq_player_fade_to_target_volume(SEQ_PLAYER_LEVEL,100,255);
                        dungeon_seq_cur = dungeon_seq_change;
                        dungeon_seq_timer = 0;
                    }
                }
                //fadeout_level_music(126);
                //play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, dungeon_seq_change), 0);
                break;
            case VOLUME_DISCONNECT:
                if (gModuleTutorialState == TUTORIAL_DONE && gMarioState->action != ACT_EATEN_BY_BUBBA) {
                    display_tutorial_message("Remote disconnected. Press @R@START@@ to self-destruct.", TUTORIAL_DISCONNECTED);
                    gModuleTutorialState = TUTORIAL_DISCONNECTED;
                }
                break;
            case VOLUME_RECONNECT:
                if (gModuleTutorialState == TUTORIAL_DISCONNECTED) {
                    gModuleTutorialState = TUTORIAL_DONE;
                }
                break;
        }
    }
}

void bhv_init_bdoor(void) {
    obj_save_bin_count(SAVE_BIN_DOORS);
    if (obj_save_bin_read()) {
        o->oBehParams2ndByte=0;
    }
}

void bhv_bdoor(void) {
    f32 dist;
    u8 needs_key = (o->oBehParams2ndByte==1);
    u8 open = FALSE;
    vec3_get_dist(gMarioState->pos,&o->oHomeVec,&dist);
    if (dist < 800.0f && (o->oAction == 1 || o->oAction == 2)) {
        open = TRUE;
    }
    if (dist < 400.0f && o->oAction != 4) {
        open = TRUE;

        if (needs_key && gMarioState->numKeys > 0) {
            o->oAction = 4;
            o->oTimer = 0;
            spawn_object(o,MODEL_KEY,bhvKeyOpen);
        }
    }
    if (sForceDoorShut) {
        open = FALSE;
    }
    if (needs_key) {
        open = FALSE;
        cur_obj_set_model(MODEL_BDOOR_LOCKED);
    }

    switch(o->oAction) {
        case 0:
            if (open) {
                o->oAction = 1;
                cur_obj_play_sound_2(SOUND_GENERAL_STAR_DOOR_OPEN);
            }
            break;
        case 1:
            o->oPosY += 25.0f;
            if (o->oPosY > o->oHomeY + 500.0f) {
                o->oAction = 2;
                o->oPosY = o->oHomeY + 500.0f;
            }
            break;
        case 2:
            if (!open) {
                o->oAction = 3;
                cur_obj_play_sound_2(SOUND_GENERAL_STAR_DOOR_CLOSE);
            }
            break;
        case 3:
            o->oPosY -= 25.0f;
            if (o->oPosY < o->oHomeY) {
                o->oAction = 0;
                o->oPosY = o->oHomeY;
            }
            break;
        case 4://door unlock anim
            if (o->oTimer>=50) {
                obj_save_bin_write(o);
                gMarioState->numKeys--;
                o->oBehParams2ndByte = 0;
                cur_obj_set_model(MODEL_BDOOR);
                o->oAction = 0;
            }
            break;
    }
}

void bhv_module_preview_box(void) {
    vec3f_copy(gModulePreviewPos,&o->oPosVec);
}

struct ObjectHitbox sSaveBoxHitbox = {
    .interactType      = INTERACT_BREAKABLE,
    .downOffset        = 5,
    .damageOrCoinValue = 0,
    .health            = 1,
    .numLootCoins      = 0,
    .radius            = 40,
    .height            = 30,
    .hurtboxRadius     = 40,
    .hurtboxHeight     = 30,
};


void bhv_save_box(void) {
    switch(o->oAction) {
        case 0:
            obj_set_hitbox(o, &sSaveBoxHitbox);
            o->oAction = 1;
            //cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
            o->header.gfx.animInfo.animAccelF = 0.33f;
            break;
        case 1:
            if (cur_obj_was_attacked_or_ground_pounded()) {
                o->oAction = 2;
                o->oVelY = 6.0f;

                f32 floor = find_floor_height(o->oPosX, o->oPosY-200.0f, o->oPosZ);
                Vec3f savePos = {o->oPosX,floor,o->oPosZ};
                save_marios_modules(savePos);
            }
            o->oInteractStatus = 0;
            break;
        case 2:
            o->oPosY += o->oVelY;
            o->oVelY -= 2.0f;
            if (o->oPosY < o->oHomeY) {
                o->oPosY = o->oHomeY;
                o->oAction = 1;
            }
            break;
    }
    load_object_collision_model();
}

void bhv_module_collect(void) {
    if (o->oBehParams2ndByte == MOD_NONMOD_KEY) {
        cur_obj_set_model(MODEL_KEY);
    }

    f32 p = o->oTimer/30.0f;
    o->oPosX = approach_f32_asymptotic(o->oHomeX,gMarioState->pos[0],p);
    o->oPosY = 80.0f + approach_f32_asymptotic(o->oHomeY,gMarioState->pos[1],p) + (sins(p * 0x8000) * p * 300.0f);
    o->oPosZ = approach_f32_asymptotic(o->oHomeZ,gMarioState->pos[2],p);

    if (p > .8f) {
        cur_obj_scale(1.0f-((p-.8f)*5.f));
    }
    if (p>=1.0f){
        obj_mark_for_deletion(o);
    }
}

void bhv_key_open(void) {
    struct Object * keyDoor = cur_obj_nearest_object_with_behavior(bhvBdoor);

    if (keyDoor) {
        o->oHomeX = keyDoor->oHomeX;
        o->oHomeY = keyDoor->oHomeY + 200.0f;
        o->oHomeZ = keyDoor->oHomeZ;

        o->oFaceAngleYaw = keyDoor->oFaceAngleYaw+0x4000;
        if ((obj_angle_to_object(o,keyDoor) - keyDoor->oFaceAngleYaw) < 0x4000) {
            o->oFaceAngleYaw = keyDoor->oFaceAngleYaw-0x4000;
        }
    }

    switch(o->oAction) {
        case 0:;
            f32 p = o->oTimer/30.0f;
            p = 1.0f-p;
            o->oPosX = approach_f32_asymptotic(o->oHomeX,gMarioState->pos[0],p);
            o->oPosY = 80.0f + approach_f32_asymptotic(o->oHomeY,gMarioState->pos[1],p) + (sins(p * 0x8000) * p * 300.0f);
            o->oPosZ = approach_f32_asymptotic(o->oHomeZ,gMarioState->pos[2],p);

            if (o->oTimer >= 30) {
                o->oAction = 1;
                cur_obj_play_sound_2(SOUND_GENERAL_DOOR_TURN_KEY);
            }
            break;
        case 1:
            o->oFaceAnglePitch += 0x400;
            if (o->oTimer >= 20) {
                obj_mark_for_deletion(o);
            }
            break;
    }
}

int sShredding = FALSE;

void bhv_recycle_interface(void) {
    switch(o->oAction) {
        case 0:
            if (GROUNDED && o->oDistanceToMario < 100.0f && gModuleMenuOpen == FALSE && gRecycleChestContent == MOD_EMPTY) {
                o->oAction ++;
                gModuleMenuOpen = TRUE;
                gModuleMenuMode = MODULE_MENU_MODE_RECYCLE;
            }
            break;
        case 1: // Waiting for input
            if (o->oDistanceToMario >= 100.0f) {
                o->oAction = 0;
                break;
            }
            if (gRecycleChestContent != MOD_EMPTY) {
                o->oAction ++;
                struct Object * visualShred = spawn_object(o,MODEL_MODULE,bhvModuleShred);
                visualShred->oBehParams2ndByte = gRecycledModule;
            }
            break;
        case 2: // Shred animation
            if (o->oTimer > 30) {
                sShredding = TRUE;
                cur_obj_play_sound_1(SOUND_ENV_SHREDDER);
            }
            if (o->oTimer > 120) {
                sShredding = FALSE;
                o->oAction ++;
            }
            break;
        case 3: // Cooldown
            if (o->oDistanceToMario > 100.0f) {
                o->oAction = 0;
            }
            break;
    }
}

void bhv_shredder(void) {
    if (sShredding) {
        o->oFaceAngleRoll += 0x200;
    }
}

void bhv_module_shred(void) {
    o->oFaceAngleYaw = 0x4000;
    switch(o->oAction) {
        case 0:;
            struct Object * recyclehole = cur_obj_nearest_object_with_behavior(bhvRecycleHole);
            if (recyclehole == NULL) {return;}
            vec3f_copy(&o->oHomeVec,&recyclehole->oPosVec);

            o->oAction++;
            break;
        case 1:;
            f32 p = 1.0f - (o->oTimer/30.0f);
            o->oPosX = approach_f32_asymptotic(o->oHomeX,gMarioState->pos[0],p);
            o->oPosY = 80.0f + approach_f32_asymptotic(o->oHomeY,gMarioState->pos[1],p) + (sins(p * 0x8000) * p * 300.0f);
            o->oPosZ = approach_f32_asymptotic(o->oHomeZ,gMarioState->pos[2],p);
            if (o->oTimer >= 30) {
                o->oAction++;
            }
            break;
        case 2:
            o->oPosY -= 1.0f;
            o->oFaceAnglePitch = (random_u16()%2000)-1000;
            if (o->oTimer > 50) {
                o->oFaceAnglePitch = 0;
            }
            break;
    }

}

void bhv_orangepole(void) {
    switch(o->oAction) {
        case 0:
            if ( (gMariosModulesSave.file[gMariosModulesSaveIndex].flags & SAVE_FLAG_PASSIVE) != 0 ) {
                o->oAction = 3;
            } else {
                o->oAction = 1;
            }
            break;
        case 1:
            o->oPosY = o->oHomeY + 2000.0f;
            if (gButtonPressId == 1) {
                gCutsceneCameraId = 1;
                o->oAction++;
                enable_time_stop_including_mario();
            }
            break;
        case 2:
            o->oPosY += o->oVelY;
            o->oVelY -= 4.0f;
            if (o->oPosY < o->oHomeY) {
                if (o->oVelY < -20.0f) {
                    cur_obj_play_sound_2(SOUND_GENERAL_TOX_BOX_MOVE);
                }
                o->oVelY = ABS(o->oVelY)*.4f;
                o->oPosY = o->oHomeY;
            }
            if (o->oTimer > 120) {
                o->oAction++;
            }
            break;
    }
}

void bhv_red_coin_spawner(void) {
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    switch(o->oAction) {
        case 0:;
            struct Object * myManager = cur_obj_nearest_object_with_behavior(bhvRedCoinManager);
            if (myManager && myManager->oAction == 3) {
                struct Object * red = spawn_object(o,MODEL_RED_COIN,bhvRedCoin);
                red->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                spawn_mist_particles();
                o->oAction++;
            }
            break;
    }
}

void bhv_red_coin_manager(void) {
    switch(o->oAction) {
        case 0:
            if (gButtonPressId == 2) {
                enable_time_stop_including_mario();
                gCutsceneCameraId = 2;
                o->oAction++;
            }
            break;
        case 1:
            if (o->oTimer >= 50) {
                enable_time_stop_including_mario();
                gCutsceneCameraId = 3;
                o->oAction++;
            }
            break;
        case 2:
            if (o->oTimer > 15) {
                o->oAction++;
            }
            break;
    }
}

void bhv_red_coin_fake(void) {
    o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    switch(o->oAction) {
        case 0:;
            struct Object * myManager = cur_obj_nearest_object_with_behavior(bhvRedCoinManager);
            if (myManager && myManager->oAction == 1) {
                o->oAction++;
            }
            break;
        case 1:
            if (o->oTimer > 30) {
                o->oPosY -= 10.0f;
            }
            if (o->oTimer > 60) {
                obj_mark_for_deletion(o);
            }
            break;
    }
}

void bhv_goomba_stack(void) {
    struct Object * goomba1 = spawn_object(o,MODEL_GOOMBA,bhvStackGoomba);
    struct Object * goomba2 = spawn_object(o,MODEL_GOOMBA,bhvStackGoomba);
    struct Object * goomba3 = spawn_object(o,MODEL_GOOMBA,bhvStackGoomba);

    goomba1->parentObj = goomba1->parentObj;
    goomba2->parentObj = goomba2->parentObj;
    goomba3->parentObj = goomba3->parentObj;

    obj_ride_obj(goomba3,goomba2);
    obj_ride_obj(goomba2,goomba1);
}

void bhv_fire_goomba(void) {
    struct Object * goomba1 = spawn_object(o,MODEL_GOOMBA,bhvStackGoomba);
    struct Object * goomba2 = spawn_object(o,MODEL_BOWLING_BALL,bhvFireSpitter);
    //struct Object * goomba3 = spawn_object(o,MODEL_GOOMBA,bhvStackGoomba);

    goomba1->parentObj = goomba1->parentObj;
    goomba2->parentObj = goomba2->parentObj;
    //goomba3->parentObj = goomba3->parentObj;

    //obj_ride_obj(goomba3,goomba2);
    obj_ride_obj(goomba2,goomba1);
}

// lmao
void bhv_baldi_door(void) {
    switch(o->oAction) {
        case 0:
            if ((gMarioState->flags & MARIO_PUNCHING) && lateral_dist_between_objects(o,gMarioObject) < 600.0f) {
                o->oAction = 1;
                o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
            }
            break;
        case 1:
            if (o->oTimer > 50) {
                o->oAction = 0;
                o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
            }
            break;
    }
}

void bhv_dungeon_tree(void) {
    cur_obj_set_model(gDungeonTreeModel);
}

void bhv_dungeon_door(void) {
    u8 open = !(!dungeon_room_is_visible(o->dungeonRoom[0]) || !dungeon_room_is_visible(o->dungeonRoom[1]));
    u8 visible = !(!dungeon_room_is_visible(o->dungeonRoom[0]) && !dungeon_room_is_visible(o->dungeonRoom[1]));
    if (!visible) {
        o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    }


    switch(o->oAction) {
        case 0:
            if (open) {
                o->oAction = 1;
            }
            break;
        case 1:
            o->oPosY += 25.0f;
            if (o->oPosY > o->oHomeY + 500.0f) {
                o->oAction = 2;
                o->oPosY = o->oHomeY + 500.0f;
            }
            break;
        case 2:
            if (!open) {
                o->oAction = 3;
            }
            break;
        case 3:
            o->oPosY -= 25.0f;
            if (o->oPosY < o->oHomeY) {
                o->oAction = 0;
                o->oPosY = o->oHomeY;
            }
            break;
        case 4://door unlock anim
            if (o->oTimer>=50) {
                obj_save_bin_write(o);
                gMarioState->numKeys--;
                o->oBehParams2ndByte = 0;
                cur_obj_set_model(MODEL_BDOOR);
                o->oAction = 0;
            }
            break;
    }
}

void bhv_dungeon_room(void) {
    u8 visible = !(!dungeon_room_is_visible(o->dungeonRoom[0]) && !dungeon_room_is_visible(o->dungeonRoom[1]));

    switch(o->oAction) {
        case 0:
            o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
            if (visible) {
                o->oAction = 1;
            }
            break;
        case 1:
            o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
            if (visible) {
                o->oTimer = 0;
            }
            if (o->oTimer > 30) {
                o->oAction = 0;
            }
            break;
    }
}

void bhv_dungeon_elite(void) {
    struct Object * goomba1 = spawn_object(o,gDungeonEnemies[0]->model,gDungeonEnemies[0]->bhv);
    struct Object * goomba2 = spawn_object(o,gDungeonEnemies[1]->model,gDungeonEnemies[1]->bhv);
    struct Object * goomba3 = spawn_object(o,gDungeonEnemies[2]->model,gDungeonEnemies[2]->bhv);

    goomba1->parentObj = goomba1->parentObj;
    goomba2->parentObj = goomba2->parentObj;
    goomba3->parentObj = goomba3->parentObj;

    obj_ride_obj(goomba3,goomba2);
    obj_ride_obj(goomba2,goomba1);
}