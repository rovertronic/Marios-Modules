#include "engine/math_util.h"
#include "module.h"
#include "dungeon.h"
#include "game_init.h"
#include "utf8_print.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "model_ids.h"
#include "behavior_data.h"
#include "module.h"

#include "levels/rogue/header.h"


#define DEBUG_PRINT_MODE 1
// 0 - Room Nums
// 1 - More Compact

int sDungeonRoomCount = 0;
int sDungeonLoopCount = 0;
int sDungeonCurrentDepth = 0;
int sDungeonLootSlotsAvailible = 0;
int sDungeonCoinBalance = 0;
u32 sDungeonUniqueVariantGeneratedFlags;

struct DungeonRoom sDungeonRoomList[64];
struct DungeonCell sDungeonCellGrid[32][32];

int sDungeonCellProcessCount = 0;
struct DungeonCell * sDungeonCellProcessList[256];

u8 sDungeonInventory[MOD_COUNT]; // Index = Mod Type, Value = Count
u8 sDungeonForceRegen = FALSE;

s8 sDirectionList[4][2] = {
    { 1, 0}, // Right
    { 0, 1}, // Down
    {-1, 0}, // Left
    { 0,-1}, // Up
};

// Room Definitions
struct DungeonRoomVariantCellList sRoomHallCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonObject sRoomHallObjectList[] = {
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 0,
    .angle = 0x4000, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 0,
    .angle = 0x4000, .pos = {-20.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomHall = {
    .cellList = &sRoomHallCellList,
    .model = MODEL_ROOM_SHORT_HALL,
    .collision = smallhall_collision,
    .objectList = &sRoomHallObjectList,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomMiniJuncCellList[] = {
    {.x = 0, .y = 0, .doorFlags = 0xF},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomMiniJunc = {
    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .collision = minijunc_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomLobbyCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0, .doorFlags = DOOR_DOWN},
    {.x = 2, .y = 0},

    {.x = 0, .y = 1, .doorFlags = DOOR_UP},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1, .doorFlags = DOOR_UP},
    {.end = TRUE},
};

Vec4f sRoomLobbyLootLocations[] = {
    {-38.851f,11.3259f,-2.12417f, 0.f},
};

struct DungeonRoomVariant sRoomLobby = {
    .cellList = &sRoomLobbyCellList,
    .model = MODEL_ROOM_LOBBY,
    .collision = rlobby_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomLobbyLootLocations,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomTreasureCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.end = TRUE},
};

Vec4f sRoomTreasureLootLocations[] = {
    {-26.4795f,.0f,3.27412f,90.f},
};

struct DungeonRoomVariant sRoomTreasure = {
    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_TREASURE,
    .collision = rtresure_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomTreasureLootLocations,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomWallJumpCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1, .doorFlags = DOOR_RIGHT},

    {.x = 1, .y = -1}, // Notch
    {.end = TRUE},
};

s8 sRoomWallJumpRequiredLoot[] = {
    MOD_JUMP, 2,
    MOD_HIT_WALL, 1,
    MOD_NONMOD_STAR, 2,
    MOD_EMPTY,
};

Vec4f sRoomWallJumpLootLocations[] = {
    {-20.f,-20.8865f,1.f,180.f},
};

struct DungeonRoomVariant sRoomWallJump = {
    .cellList = &sRoomWallJumpCellList,
    .model = MODEL_ROOM_WALLJUMP,
    .collision = rwalljump_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomWallJumpRequiredLoot,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomLongJumpCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 0, .y = 1},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP},
    {.end = TRUE},
};

s8 sRoomLongJumpRequiredLoot[] = {
    MOD_ZACTION, 1,
    MOD_HIT_WALL, 1,
    MOD_JUMP, 1,
    MOD_ROTATE, 1,
    MOD_NONMOD_STAR, 3,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomLongJump = {
    .cellList = &sRoomLongJumpCellList,
    .model = MODEL_ROOM_LONGJUMP,
    .collision = rlongjump_collision,
    .objectList = NULL,
    .requiredLoot = sRoomLongJumpRequiredLoot,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomVanishHopCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.end = TRUE},
};

s8 sRoomVanishHopRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_POW, 1,
    MOD_CAP, 1,
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomVanishHop = {
    .cellList = &sRoomVanishHopCellList,
    .model = MODEL_ROOM_VANISHHOP,
    .collision = vanishhop_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomVanishHopRequiredLoot,
    .generateOnce = TRUE,
};

struct DungeonRoomVariantCellList sRoomFnabCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT|DOOR_RIGHT)},
    {.x = 0, .y = 1},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomFnab = {
    .cellList = &sRoomFnabCellList,
    .model = MODEL_ROOM_FNAB,
    .collision = rfnab_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    //.requiredLoot = &sRoomVanishHopRequiredLoot,
    .generateOnce = TRUE,
};

struct DungeonRoomVariantCellList sRoomBtcmCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBtcm = {
    .cellList = &sRoomBtcmCellList,
    .model = MODEL_ROOM_BTCM,
    .collision = rbtcm_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    //.requiredLoot = &sRoomVanishHopRequiredLoot,
    .generateOnce = TRUE,
};

Vec4f sRoomBaldiLootLocations[] = {
    {-12.3897f,.0f,.0f,.0f},
};

struct DungeonObject sRoomBaldiObjectList[] = {
    {.bhv = bhvStaticObject, .model = MODEL_DUNGEON_BALDI_DOOR, .param = 0,
    .angle = 0x0, .pos = {-2.26746f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBaldi = {
    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_BALDI,
    .collision = baldi_collision,
    .objectList = sRoomBaldiObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomBaldiLootLocations,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
};

struct DungeonRoomVariant * sRoomVariantList[] = {
    &sRoomHall,
    &sRoomMiniJunc,
    &sRoomLobby,
    &sRoomTreasure,

    // Challenge Rooms
    &sRoomLongJump,
    &sRoomVanishHop,
    &sRoomWallJump,

    // Easter-Egg Rooms
    &sRoomFnab,
    &sRoomBtcm,
    &sRoomBaldi,
};


s32 dungeon_is_cell_occupied(int x, int y) {
    if ((x < 0)||(x >= 32)||
        (y < 0)||(y >= 32)) {
        return TRUE;
    }
    return (sDungeonCellGrid[y][x].id != 0);
}

s32 dungeon_door_on_other_side(int xp, int yp, int j) {
    int x = xp + (sDirectionList[j][0]);
    int y = yp - (sDirectionList[j][1]);
    
    if (x < 32 && x >= 0 &&
        y < 32 && y >= 0) {
        u8 oppositeflag;
        switch(j) {
            case 0:
                oppositeflag = DOOR_LEFT;
                break;
            case 1:
                oppositeflag = DOOR_UP;
                break;
            case 2:
                oppositeflag = DOOR_RIGHT;
                break;
            case 3:
                oppositeflag = DOOR_DOWN;
                break;
        }
        if (sDungeonCellGrid[y][x].id > 0 &&
            (sDungeonCellGrid[y][x].doorFlags & oppositeflag)) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 dungeon_place_loot_in_random_previous_room(s8 loot) {
    int chosen_room_index = tinymt32_generate_u32(&gGlobalRandomState)%sDungeonRoomCount;
    for (int i = 0; i < 10; i++) {
        chosen_room_index = tinymt32_generate_u32(&gGlobalRandomState)%sDungeonRoomCount;
        if ((sDungeonRoomList[chosen_room_index].lootCount < sDungeonRoomList[chosen_room_index].variant->maxLootCt)&&
            // Prioritize treasure rooms and challenge rooms for loot
            ((i>5)||(sDungeonRoomList[chosen_room_index].variant == &sRoomTreasure)||(sDungeonRoomList[chosen_room_index].variant->requiredLoot))) {
            sDungeonRoomList[chosen_room_index].loot[sDungeonRoomList[chosen_room_index].lootCount] = loot;
            sDungeonRoomList[chosen_room_index].lootCount++;
            sDungeonLootSlotsAvailible --;
            return TRUE;
        }
    }

    // Guess I couldn't find a random room, try every availible room instead
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if (sDungeonRoomList[i].lootCount < sDungeonRoomList[i].variant->maxLootCt) {
            sDungeonRoomList[i].loot[sDungeonRoomList[i].lootCount] = loot;
            sDungeonRoomList[i].lootCount++;
            sDungeonLootSlotsAvailible --;
            return TRUE;  
        }
    }

    sDungeonForceRegen = TRUE;
    return FALSE;
}

void dungeon_propegate_loot_with_requirement_list(s8 * requirementList) {
    if (requirementList != NULL) {
        int i = 0;
        while(requirementList[i] != MOD_EMPTY) {
            s8 lootType = requirementList[i];
            i++;
            s8 lootCount = requirementList[i];
            i++;

            for (int i = 0; i < lootCount; i++) {
                if (sDungeonInventory[lootType] < lootCount) {
                    sDungeonInventory[lootType] ++;
                    dungeon_place_loot_in_random_previous_room(lootType);
                }
            }
        }
    }
}

s32 dungeon_requirement_list_length(s8 * requirementList) {
    s32 lootSlotsNeeded = 0;
    if (requirementList != NULL) {
        int i = 0;
        while(requirementList[i] != MOD_EMPTY) {
            lootSlotsNeeded++;
            i+=2;
        }
    }
    return lootSlotsNeeded;
}

void dungeon_create_room(struct DungeonRoomVariant * variant, int dir, int x, int y) {
    struct DungeonRoomVariantCellList * cellList = variant->cellList;
    sDungeonRoomCount++;

    int ndir = (dir+1)%4;
    int index = 0;
    while(variant->cellList[index].end == FALSE) {
        int xp = x + (variant->cellList[index].x * sDirectionList[dir][0])
                   + (variant->cellList[index].y * sDirectionList[dir][1]);
        int yp = y + (variant->cellList[index].x * sDirectionList[ndir][0])
                   + (variant->cellList[index].y * sDirectionList[ndir][1]);
        sDungeonCellGrid[yp][xp].id = sDungeonRoomCount;

        u8 rotatedFlags = variant->cellList[index].doorFlags << dir;
        u8 loppedFlags = (rotatedFlags & ~0xF) >> 4;
        sDungeonCellGrid[yp][xp].doorFlags = rotatedFlags | loppedFlags;

        if (sDungeonCellGrid[yp][xp].doorFlags != 0) {
            sDungeonCellProcessList[sDungeonCellProcessCount] = &sDungeonCellGrid[yp][xp];
            sDungeonCellProcessCount++;

            // Count reconnections
            for (int j = 0; j < 4; j++) {
                if ((j+2)%4 == dir &&
                    variant->cellList[index].x == 0 &&
                    variant->cellList[index].y == 0) {
                    // Do not count origin door
                    continue;
                }

                // j = dir
                if (sDungeonCellGrid[yp][xp].doorFlags & (1 << j)) {
                    if (dungeon_door_on_other_side(x,y,j)) {
                        sDungeonLoopCount++;
                    }
                }
            }
        }

        // Copy of coordinates for self referencing
        sDungeonCellGrid[yp][xp].x = xp;
        sDungeonCellGrid[yp][xp].y = yp;

        index++;
    }

    //Populate room array with info
    sDungeonRoomList[sDungeonRoomCount-1].direction = dir;
    sDungeonRoomList[sDungeonRoomCount-1].variant = variant;
    sDungeonRoomList[sDungeonRoomCount-1].xorigin = x;
    sDungeonRoomList[sDungeonRoomCount-1].yorigin = y;

    sDungeonRoomList[sDungeonRoomCount-1].lootCount = 0;
    for (int i = 0; i < 4; i++) {
        sDungeonRoomList[sDungeonRoomCount-1].loot[i] = MOD_EMPTY;
    }

    sDungeonLootSlotsAvailible+=variant->maxLootCt;
}

int dungeon_check_room_viability(struct DungeonRoomVariant * variant, int dir, int x, int y) {
    struct DungeonRoomVariantCellList * cellList = variant->cellList;

    int ndir = (dir+1)%4;
    int index = 0;
    while(variant->cellList[index].end == FALSE) {
        int xp = x + (variant->cellList[index].x * sDirectionList[dir][0])
                   + (variant->cellList[index].y * sDirectionList[dir][1]);
        int yp = y + (variant->cellList[index].x * sDirectionList[ndir][0])
                   + (variant->cellList[index].y * sDirectionList[ndir][1]);
        
        if (dungeon_is_cell_occupied(xp,yp)) {
            return FALSE;
        }

        index++;
    }
    return TRUE;
}

void dungeon_generate_rooms_at_doors(void) {
    sDungeonCurrentDepth++;

    int i_max = sDungeonCellProcessCount;
    for (int i = 0; i < i_max; i++) {
        if (sDungeonCellProcessList[i]->resolved) {continue;}
        sDungeonCellProcessList[i]->resolved = TRUE;

        for (int j = 0; j < 4; j++) {
            // j = dir
            if (sDungeonCellProcessList[i]->doorFlags & (1 << j)) {
                int x = sDungeonCellProcessList[i]->x + (sDirectionList[j][0]);
                int y = sDungeonCellProcessList[i]->y - (sDirectionList[j][1]);
                
                int success = FALSE;
                int trycount = 0;
                while(!success && trycount < 10) {
                    u32 selectedVariantIndex = tinymt32_generate_u32(&gGlobalRandomState) %  (sizeof(sRoomVariantList)/4);
                    struct DungeonRoomVariant * selectedVariant = sRoomVariantList[selectedVariantIndex];

                    if (sDungeonRoomCount >= 64) {
                        success = TRUE;
                        continue;
                    }

                    if (selectedVariant->generateOnce &&
                        sDungeonUniqueVariantGeneratedFlags & (1<<selectedVariantIndex)) {
                        trycount++;
                        continue;
                    }

                    if (dungeon_requirement_list_length(selectedVariant->requiredLoot) > sDungeonLootSlotsAvailible) {
                        // if not enough treasure slots, don't make this room
                        trycount++;
                        continue;
                    }

                    if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                        sDungeonUniqueVariantGeneratedFlags |= (1<<selectedVariantIndex);

                        dungeon_propegate_loot_with_requirement_list(selectedVariant->requiredLoot);
                        dungeon_create_room(selectedVariant,j,x,y);
                        success = TRUE;
                    }
                    trycount++;
                }
            }
        }
    }
}

void dungeon_spawn_room_objects(void) {

    // Rooms
    for (int i = 0; i < sDungeonRoomCount; i++) {
        struct Object * roomObj = spawn_object(gMarioObject, sDungeonRoomList[i].variant->model ,bhvDungeonProcGenRoom);
        roomObj->oPosX = 32000.f - (sDungeonRoomList[i].xorigin * 2000.f);
        roomObj->oPosZ = 32000.f - (sDungeonRoomList[i].yorigin * 2000.f);
        roomObj->oFaceAngleYaw = sDungeonRoomList[i].direction * 0x4000;
        roomObj->collisionData = segmented_to_virtual(sDungeonRoomList[i].variant->collision);

        if (sDungeonRoomList[i]. variant == &sRoomMiniJunc) {
            for (int j = 0; j < 4; j ++) {
                if (dungeon_door_on_other_side(sDungeonRoomList[i].xorigin,sDungeonRoomList[i].yorigin,j)) {
                    struct Object * carpetPoint = spawn_object(gMarioObject, MODEL_DUNGEON_CARPET_POINT, bhvStaticObject);
                    carpetPoint->oPosX = 32000.f - (sDungeonRoomList[i].xorigin * 2000.f);
                    carpetPoint->oPosZ = 32000.f - (sDungeonRoomList[i].yorigin * 2000.f);
                    carpetPoint->oFaceAngleYaw = j * 0x4000;
                }
            }
        }


        // Spawn Loot (Chests, Stars, Keys)
        for (int j = 0; j < sDungeonRoomList[i].lootCount; j++) {
            struct Object * chest;
            switch (sDungeonRoomList[i].loot[j]) {
                case MOD_NONMOD_STAR:
                    chest = spawn_object(gMarioObject, MODEL_STAR, bhvStar);
                break;
                case MOD_NONMOD_MYSTERY_CHEST:
                    chest = spawn_object(gMarioObject, MODEL_MCHEST, bhvMysteryChest);
                    break;
                default:
                    chest = spawn_object(gMarioObject, MODEL_CHEST, bhvChest);
            }
            vec3f_copy(&chest->oPosVec,&roomObj->oPosVec);
            s16 angle = sDungeonRoomList[i].direction * 0x4000;
            chest->oPosX += (sDungeonRoomList[i].variant->lootLocations[j][0] * 100.f * sins(angle + 0x4000))
                + (sDungeonRoomList[i].variant->lootLocations[j][1] * 100.f * sins(angle + 0x8000));
            chest->oPosZ += (sDungeonRoomList[i].variant->lootLocations[j][0] * 100.f * coss(angle + 0x4000))
                + (sDungeonRoomList[i].variant->lootLocations[j][1] * 100.f * coss(angle + 0x8000));
            chest->oPosY += sDungeonRoomList[i].variant->lootLocations[j][2] * 100.f;
            chest->oFaceAngleYaw = angle + (182.f * sDungeonRoomList[i].variant->lootLocations[j][3]);
            chest->oBehParams2ndByte = sDungeonRoomList[i].loot[j];

            if (sDungeonRoomList[i].loot[j] == MOD_NONMOD_MYSTERY_CHEST) {
                chest->oBehParams2ndByte = 0;
                if (tinymt32_generate_u32(&gGlobalRandomState)%3==0) {
                    chest->oBehParams2ndByte = 1;
                }
            }

            if ((tinymt32_generate_u32(&gGlobalRandomState)%2==0)&&sDungeonCoinBalance>=10) {
                //randomly make chests cost money
                SET_BPARAM1(chest->oBehParams,10);
                sDungeonCoinBalance-=10;
            }

            // Raise the star a bit
            if (sDungeonRoomList[i].loot[j] == MOD_NONMOD_STAR) {
                chest->oPosY += 100.0f;
            }
        }

        // Spawn Objects
        if (sDungeonRoomList[i].variant->objectList != NULL) {
            int j = 0;
            while(sDungeonRoomList[i].variant->objectList[j].end == FALSE) {
                struct DungeonObject * details = &sDungeonRoomList[i].variant->objectList[j];
                s16 angle = sDungeonRoomList[i].direction * 0x4000;

                if (details->bhv == bhvCoinFormation) {
                    if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
                        // Sometimes, don't spawn coins
                        j++;
                        continue;
                    }
                    sDungeonCoinBalance+=5;
                    if (details->param == 2) {
                        sDungeonCoinBalance += 3;
                    }
                }

                struct Object * obj = spawn_object(gMarioObject, details->model, details->bhv);
                vec3f_copy(&obj->oPosVec,&roomObj->oPosVec);

                obj->oPosX += (details->pos[0] * 100.f * sins(angle + 0x4000))
                    + (details->pos[1] * 100.f * sins(angle + 0x8000));
                obj->oPosZ += (details->pos[0] * 100.f * coss(angle + 0x4000))
                    + (details->pos[1] * 100.f * coss(angle + 0x8000));
                obj->oPosY += details->pos[2] * 100.f;
                obj->oFaceAngleYaw = angle + details->angle;;
                obj->oBehParams2ndByte = details->param;

                j++;
            }
        }
    }

    // Door holes
    for (int i = 0; i < sDungeonCellProcessCount; i++) {
        // j = dir
        for (int j = 0; j < 4; j++) {
            if (sDungeonCellProcessList[i]->doorFlags & (1<<j)) {
                struct Object * doorObj;
                if (dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                    if (j == 0 || j == 1) {
                        doorObj = spawn_object(gMarioObject, MODEL_DUNGEON_DOORHOLE ,bhvDungeonProcGenRoom);
                        doorObj->collisionData = segmented_to_virtual(doorhole_collision);
                    } else {
                        doorObj = spawn_object(gMarioObject, MODEL_NONE, bhvStaticObject);
                        obj_mark_for_deletion(doorObj);
                    }
                } else {
                    doorObj = spawn_object(gMarioObject, MODEL_DUNGEON_DOORHOLE_COVERED ,bhvDungeonProcGenRoom);
                    doorObj->collisionData = segmented_to_virtual(doorhole_covered_collision);
                }
                doorObj->oPosX = (32000.f - (sDungeonCellProcessList[i]->x * 2000.f)) - (1000.f * sDirectionList[j][0]);
                doorObj->oPosZ = (32000.f - (sDungeonCellProcessList[i]->y * 2000.f)) + (1000.f * sDirectionList[j][1]);
                doorObj->oFaceAngleYaw = j * 0x4000;
            }
        }
    }
}

void dungeon_generate(void) {
    // Randomize Seed
    tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);

    redo_generate:
    sDungeonForceRegen = FALSE;

    // Clear dungeon data
    sDungeonRoomCount = 0;
    sDungeonLoopCount = 0;
    sDungeonCurrentDepth = 0;
    sDungeonLootSlotsAvailible = 0;
    sDungeonCoinBalance = 0;
    sDungeonUniqueVariantGeneratedFlags = 0;
    sDungeonCellProcessCount = 0;
    
    bzero(&sDungeonCellGrid, sizeof(sDungeonCellGrid));
    bzero(&sDungeonInventory, sizeof(sDungeonInventory));

    // Build First Room
    dungeon_create_room(&sRoomMiniJunc  ,0,16,16);

    for (int i = 0; i < 50; i++) {
        dungeon_generate_rooms_at_doors();
    }

    // Manually fill remaining empty treasure rooms and challenge rooms with mystery chests and stars
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if ((sDungeonRoomList[i].variant == &sRoomTreasure || sDungeonRoomList[i].variant->requiredLoot) &&
            sDungeonRoomList[i].lootCount == 0 && sDungeonRoomList[i].variant->maxLootCt > 0) {
            sDungeonRoomList[i].lootCount = 1;
            if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
                sDungeonRoomList[i].loot[0] = MOD_NONMOD_STAR;
            } else {
                sDungeonRoomList[i].loot[0] = MOD_NONMOD_MYSTERY_CHEST;
            }
        }
    }

    if (sDungeonLoopCount < 2 || sDungeonForceRegen || sDungeonRoomCount < 50) {
        goto redo_generate;
    }

    // Finished, spawn objects
    dungeon_spawn_room_objects();
}

int debugmode = 0;

u8 sDebugColorList[][3] = {
    {255,255,255},
    {255,150,50},
    {50,50,255},
    {50,255,50},
    {255,255,50},
    {50,255,255},
};

void dungeon_debug_print(void) {
    print_text_fmt_int(0, 200, "LOOP CT %d", sDungeonLoopCount);
    print_text_fmt_int(0, 220, "ITEM SLOTS %d", sDungeonLootSlotsAvailible);

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            if (sDungeonCellGrid[y][x].id != 0) {
                #if DEBUG_PRINT_MODE == 0
                    print_text_fmt_int(x*16, y*16, "%d", sDungeonCellGrid[y][x].id);
                #else
                    utf8_print_reset();
                    print_utf8_color(".",40+x*3, 40+y*3,
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][0],
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][1],
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][2]);
                #endif
            }
        }
    }
}