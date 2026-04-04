#include "engine/math_util.h"
#include "module.h"
#include "dungeon.h"
#include "game_init.h"
#include "utf8_print.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "model_ids.h"
#include "behavior_data.h"
#include "level_update.h"
#include "ingame_menu.h"
#include "engine/surface_collision.h"

#include "levels/rogue/header.h"
#include "levels/rf/header.h"

struct DungeonRoom * gDungeonMarioRoom = NULL;
u32 sDungeonDiscoveredFlags[2];

int sDungeonRoomCount = 0;
int sDungeonLoopCount = 0;
int sDungeonCurrentDepth = 0;
int sDungeonLootSlotsAvailible = 0;
int sDungeonCoinBalance = 0;
int sDungeonRedCoinRoomMax = 0;
int sDungeonTargetRoomCount = 0;
int sDungeonRemovedPointlessRooms = 0;
int sDungeonGeneratingLevelId = 0;
u32 sDungeonUniqueVariantGeneratedFlags;

u16 gDungeonTreeModel = 0;

Vec3f gDungeonSpawnLocation;

struct DungeonRoom sDungeonRoomList[64];
struct DungeonCell sDungeonCellGrid[32][32];

int sDungeonCellProcessCount = 0;
struct DungeonCell * sDungeonCellProcessList[256];

u8 sDungeonInventory[MOD_COUNT]; // Index = Mod Type, Value = Count
u8 sDungeonTotalInventory[MOD_COUNT]; // Tally of previous levels
u8 sDungeonForceRegen = FALSE;
u8 sDungeonEasterEggGenerated = FALSE;

u8 gDungeonNeedsToGenerate = FALSE;

s8 sDirectionList[4][2] = {
    { 1, 0}, // Right
    { 0, 1}, // Down
    {-1, 0}, // Left
    { 0,-1}, // Up
};

/* ENEMY DATA */

struct DungeonObject sBottomEnemyList[] = {
    {.bhv = bhvGoomba, .model = MODEL_GOOMBA, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvScuttlebug, .model = MODEL_SCUTTLEBUG, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvMrI, .model = MODEL_MR_I, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
};

struct DungeonObject sTopEnemyList[] = {
    {.bhv = bhvStackGoomba, .model = MODEL_GOOMBA, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvSnufit, .model = MODEL_SNUFIT, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvUtilityMace, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvFlamethrower, .model = MODEL_UTILITY_FLAME, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
};

struct DungeonObject * gDungeonEnemies[4];
struct DungeonObject * gDungeonAboomboomination[7];

#include "dungeon_room_data.inc.c"

struct DungeonRoomVariant * sLv1RoomVariantList[] = {
    // Freebie Star Room (Determined upon LV gen)
    NULL,

    // Transition Rooms
    &sRoomMiniJunc1,
    &sRoomMiniJunc2,
    &sRoomHall,
    &sRoomLobby,
    &sRoomThwomps,
    &sRoomSplitHall,

    // Special Rooms
    &sRoomTreasure,

    // Challenge Rooms
    &sRoomGardenHall,
    &sRoomVanishHop,
    &sRoomWallJump,
    &sRoomWood,
    &sRoomCaveJump,
    &sRoomSilverPillar,
    &sRoomRedCoin,
    &sRoomFlipPuzzle,
};

struct DungeonRoomVariant * sLv2RoomVariantList[] = {
    // Transition Rooms
    &sRoomMiniJunc1,
    &sRoomMiniJunc2,
    &sRoomHall,
    &sRoomLobby,
    &sRoomLobby2,
    &sRoomSplitHall,
    &sRoomSpaceworld,

    // Special Rooms
    &sRoomTreasure,
    &sRoomSuperTreasure,
    &sRoomGrindr,

    // Challenge Rooms
    &sRoomGardenHall,
    &sRoomClock,
    &sRoomLongJump,
    &sRoomVanishHop,
    &sRoomWallJump,
    &sRoomWood,
    &sRoomFurnace,
    &sRoomCaveJump,
    &sRoomAutoMaze,
    &sRoomSilverPillar,
    &sRoomRedCoin,
    &sRoomLavaDrop,
    &sRoomFlipPuzzle,

    // Easter-Egg Rooms
    &sRoomFnab,
    &sRoomBtcm,
    &sRoomBaldi,
    &sRoomBns,
};

struct DungeonRoomVariant * sLv3RoomVariantList[] = {
    &sRoomRfStraight,
    &sRoomRfRight,
    &sRoomRfLeft,
    
    &sRoomRfRamp,
    &sRoomRfSpinning,
    &sRoomRfTilting,
    &sRoomRfSquarish,
    &sRoomRfAmplected,
    &sRoomRfRino,

    // Super Challenges
    &sRoomRfSlope,
    &sRoomRfBlankJump,
    &sRoomRfVertical,
};

struct DungeonRoom * dungeon_get_mario_room(void) {
    if (!is_level_dungeon()) {return NULL;}
    u32 x = (((-gMarioState->pos[0])+32000.f + 1000.f)/2000.f);
    u32 y = (((-gMarioState->pos[2])+32000.f + 1000.f)/2000.f);
    if (sDungeonCellGrid[y][x].id == 0) {return NULL;}
    return &sDungeonRoomList[ sDungeonCellGrid[y][x].id-1 ];
}

s32 dungeon_is_cell_occupied(int x, int y) {
    // 0 and 32 are not allowed due to collision bugs
    if ((x < 1)||(x >= 31)||
        (y < 1)||(y >= 31)) {
        return TRUE;
    }
    return (sDungeonCellGrid[y][x].id != 0);
}

struct DungeonCell * sDungeonDoorOtherSideRet = NULL;
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
            (sDungeonCellGrid[y][x].doorFlags & oppositeflag) &&
            sDungeonCellGrid[y][x].worldY == sDungeonCellGrid[yp][xp].worldY) {
            sDungeonDoorOtherSideRet = &sDungeonCellGrid[y][x];
            return TRUE;
        }
    }
    sDungeonDoorOtherSideRet = NULL;
    return FALSE;
}

s32 dungeon_place_loot_in_random_previous_room(s8 loot) {
    int minLv = 0;
    if (loot == MOD_NONMOD_KEY) {
        // Make sure keys are actually challenging to get
        minLv = 2;
        if (sDungeonGeneratingLevelId == 0) {
            // Level 1 only has one key, make it a challenge to get.
            minLv = 3;
        }
    }

    int chosen_room_index = tinymt32_generate_u32(&gGlobalRandomState)%sDungeonRoomCount;
    for (int i = 0; i < 10; i++) {
        chosen_room_index = tinymt32_generate_u32(&gGlobalRandomState)%sDungeonRoomCount;
        if ((sDungeonRoomList[chosen_room_index].lootCount < sDungeonRoomList[chosen_room_index].variant->maxLootCt)&&
            sDungeonRoomList[chosen_room_index].challengeLv >= minLv &&
            // Prioritize treasure rooms and challenge rooms for loot
            ((i>5)||(sDungeonRoomList[chosen_room_index].variant == &sRoomTreasure)||(sDungeonRoomList[chosen_room_index].variant->requiredLoot))) {

            // Do not put stars in treasure rooms. Lame.
            if (loot == MOD_NONMOD_STAR && sDungeonRoomList[chosen_room_index].variant == &sRoomTreasure) {
                continue;
            }

            // Do not put stars in star rooms. Very lame!
            if (loot == MOD_NONMOD_STAR && sDungeonRoomList[chosen_room_index].variant->starCt > 0) {
                continue;
            }

            sDungeonRoomList[chosen_room_index].loot[sDungeonRoomList[chosen_room_index].lootCount] = loot;
            sDungeonRoomList[chosen_room_index].lootCount++;
            sDungeonLootSlotsAvailible --;
            return TRUE;
        }
    }

    // Guess I couldn't find a random room, try every availible room instead
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if (sDungeonRoomList[i].lootCount < sDungeonRoomList[i].variant->maxLootCt &&
            sDungeonRoomList[i].challengeLv >= minLv) {

            // Do not put stars in treasure rooms. Lame.
            if (loot == MOD_NONMOD_STAR && sDungeonRoomList[i].variant == &sRoomTreasure) {
                continue;
            }

            // Do not put stars in star rooms. Very lame!
            if (loot == MOD_NONMOD_STAR && sDungeonRoomList[i].variant->starCt > 0) {
                continue;
            }

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

void dungeon_room_set_neighbor_flag(struct DungeonRoom * room, int id) {
    int index = id/32;
    int flag = id%32;
    room->neighborFlag[index] |= (1 << flag);
}

s32 dungeon_room_is_visible(struct DungeonRoom * room) {
    if (room == NULL) {return TRUE;}
    if (gDungeonMarioRoom == NULL) {return TRUE;}
    if (gCurrLevelNum == LEVEL_RF) {return TRUE;}

    int id = room->id;
    int index = id/32;
    int flag = id%32;
    return (gDungeonMarioRoom->neighborFlag[index] & (1 << flag))!=0;
}

struct DungeonRoom * dungeon_create_room(struct DungeonRoomVariant * variant, int dir, int x, int y, int worldY) {
    struct DungeonRoomVariantCellList * cellList = variant->cellList;
    struct DungeonRoom * thisRoom = &sDungeonRoomList[sDungeonRoomCount]; 
    sDungeonRoomCount++;

    int ndir = (dir+1)%4;
    int index = 0;
    while(variant->cellList[index].end == FALSE) {
        int xp = x + (variant->cellList[index].x * sDirectionList[dir][0])
                   + (variant->cellList[index].y * sDirectionList[dir][1]);
        int yp = y + (variant->cellList[index].x * sDirectionList[ndir][0])
                   + (variant->cellList[index].y * sDirectionList[ndir][1]);
        sDungeonCellGrid[yp][xp].id = sDungeonRoomCount;
        sDungeonCellGrid[yp][xp].worldY = worldY + variant->cellList[index].worldY;

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
    thisRoom->direction = dir;
    thisRoom->variant = variant;
    thisRoom->xorigin = x;
    thisRoom->yorigin = y;
    thisRoom->id = sDungeonRoomCount-1;
    thisRoom->worldY = worldY;

    thisRoom->lootCount = 0;
    for (int i = 0; i < 4; i++) {
        thisRoom->loot[i] = MOD_EMPTY;
    }

    sDungeonLootSlotsAvailible+=variant->maxLootCt;
    sDungeonInventory[MOD_NONMOD_STAR]+=variant->starCt;

    return thisRoom;
}

void dungeon_deconstruct_room(struct DungeonRoom * room) {
    struct DungeonRoomVariant * variant = room->variant;
    int x = room->xorigin;
    int y = room->yorigin;
    int dir = room->direction;
    struct DungeonRoomVariantCellList * cellList = variant->cellList;

    room->variant = NULL;

    int ndir = (dir+1)%4;
    int index = 0;
    while(variant->cellList[index].end == FALSE) {
        int xp = x + (variant->cellList[index].x * sDirectionList[dir][0])
                   + (variant->cellList[index].y * sDirectionList[dir][1]);
        int yp = y + (variant->cellList[index].x * sDirectionList[ndir][0])
                   + (variant->cellList[index].y * sDirectionList[ndir][1]);
        sDungeonCellGrid[yp][xp].id = 0;
        sDungeonCellGrid[yp][xp].doorFlags = 0;

        index++;
    }
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

void dungeon_generate_rooms_at_doors(struct DungeonRoomVariant ** variantList, int size) {

    int needsResolution;
    do {
        needsResolution = FALSE;
        sDungeonCurrentDepth++;

        int iMax = sDungeonCellProcessCount;
        for (int i = 0; i < iMax; i++) {
            if (sDungeonCellProcessList[i]->resolved) {continue;}
            sDungeonCellProcessList[i]->resolved = TRUE;
            needsResolution = TRUE;

            struct DungeonRoom * originRoom = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];

            for (int j = 0; j < 4; j++) {
                // j = dir
                if (sDungeonCellProcessList[i]->doorFlags & (1 << j)) {
                    int x = sDungeonCellProcessList[i]->x + (sDirectionList[j][0]);
                    int y = sDungeonCellProcessList[i]->y - (sDirectionList[j][1]);
                    
                    int success = FALSE;
                    int trycount = 0;
                    while(!success && trycount < 30) {
                        u32 selectedVariantIndex = tinymt32_generate_u32(&gGlobalRandomState) % (size/4);
                        struct DungeonRoomVariant * selectedVariant = variantList[selectedVariantIndex];

                        if (sDungeonRoomCount >= sDungeonTargetRoomCount) {
                            success = TRUE;
                            continue;
                        }

                        // Treasure rooms spawning at the entrance can fuck right off into hell
                        if (selectedVariant == &sRoomTreasure && sDungeonCurrentDepth == 1) {
                            trycount++;
                            continue;
                        }

                        // Prevent chaining of hallways. Walking simulator not fun!
                        if (selectedVariant == originRoom->variant && sDungeonGeneratingLevelId != 2) {
                            trycount++;
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

                        if ((selectedVariant->easterEgg || selectedVariant == &sRoomRedCoin) && sDungeonCurrentDepth < 6) {
                            trycount++;
                            continue;
                        } 

                        if (sDungeonEasterEggGenerated && selectedVariant->easterEgg) {
                            // only generate 1 easter egg per level, and farther in
                            trycount++;
                            continue;
                        }

                        if (selectedVariant->starCt && sDungeonInventory[MOD_NONMOD_STAR] >= 8) {
                            trycount++;
                            continue;
                        }

                        if (selectedVariant->rarity > 0 &&
                            (tinymt32_generate_u32(&gGlobalRandomState) % selectedVariant->rarity != 0)) {
                            // Roll for room rarity
                            trycount++;
                            continue;
                        }

                        if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                            int isUnique = FALSE;
                            if (!(sDungeonUniqueVariantGeneratedFlags & (1<<selectedVariantIndex))) {
                                sDungeonUniqueVariantGeneratedFlags |= (1<<selectedVariantIndex);
                                isUnique = TRUE;
                            }

                            if (selectedVariant->easterEgg) {
                                sDungeonEasterEggGenerated = TRUE;
                            }

                            dungeon_propegate_loot_with_requirement_list(selectedVariant->requiredLoot);

                            if (selectedVariant->needKey) {
                                dungeon_place_loot_in_random_previous_room(MOD_NONMOD_KEY);
                            }

                            struct DungeonRoom * createdRoom = dungeon_create_room(selectedVariant,j,x,y,sDungeonCellProcessList[i]->worldY);

                            int challengeLv = originRoom->challengeLv;
                            if (selectedVariant->requiredLoot && isUnique) {
                                challengeLv++;
                            }
                            createdRoom->challengeLv = challengeLv;

                            if (selectedVariant == &sRoomRedCoin) {
                                sDungeonRedCoinRoomMax = sDungeonRoomCount;
                            }

                            success = TRUE;
                        }
                        trycount++;
                    }
                }
            }
        }
    } while (needsResolution);
}

s32 dungeon_generate_boss_room(struct DungeonRoomVariant * selectedVariant) {
    if (gModuleCreativeEnabled) {return FALSE;}
    int iMax = sDungeonCellProcessCount;

    // Check latest generated rooms first, which will make the boss room generate deep in
    for (int i = iMax-1; i >= 0; i--) {
        struct DungeonRoom * originRoom = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];

        if (originRoom->challengeLv < 4 && sDungeonGeneratingLevelId == 1) {
            continue;
        }

        for (int j = 0; j < 4; j++) {
            // j = dir
            if (sDungeonCellProcessList[i]->doorFlags & (1 << j)) {
                int x = sDungeonCellProcessList[i]->x + (sDirectionList[j][0]);
                int y = sDungeonCellProcessList[i]->y - (sDirectionList[j][1]);

                if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                    if (sDungeonGeneratingLevelId == 1) {
                        dungeon_place_loot_in_random_previous_room(MOD_NONMOD_KEY);
                        dungeon_place_loot_in_random_previous_room(MOD_NONMOD_KEY);
                    }
                    dungeon_create_room(selectedVariant,j,x,y,sDungeonCellProcessList[i]->worldY);
                    return TRUE;
                }
            }
        }
    }
    sDungeonForceRegen = TRUE;
    return FALSE;
}

void dungeon_calculate_all_neighbor_flags(void) {
    for (int i = 0; i < sDungeonRoomCount; i++) {
        sDungeonRoomList[i].neighborFlag[0] = 0;
        sDungeonRoomList[i].neighborFlag[1] = 0;
    }
    for (int i = 0; i < sDungeonCellProcessCount; i++) {
        struct DungeonCell * cell = sDungeonCellProcessList[i];
        dungeon_room_set_neighbor_flag( &sDungeonRoomList[cell->id-1] , cell->id-1);
        for (int j = 0; j < 4; j++) {
            if (dungeon_door_on_other_side(cell->x,cell->y,j)) {
                dungeon_room_set_neighbor_flag( &sDungeonRoomList[cell->id-1] , sDungeonDoorOtherSideRet->id-1);
            }
        }
    }
}

void dungeon_remove_pointless_rooms(void) {
    if (sDungeonGeneratingLevelId == 2) {return;}

    int pointlessRoomsThisPass;
    do {
        pointlessRoomsThisPass = 0;
        for (int i = 0; i < sDungeonRoomCount; i++) {
            struct DungeonRoom * room = &sDungeonRoomList[i];

            int neighborCount = 0;
            for (int j = 0; j < 32; j++) {
                if (room->neighborFlag[0] & (1<<j)) {
                    neighborCount++;
                }
            }
            for (int j = 0; j < 32; j++) {
                if (room->neighborFlag[1] & (1<<j)) {
                    neighborCount++;
                }
            }

            if (room->variant != NULL &&
                neighborCount <= 2 && room->lootCount == 0 &&
                // Keyed rooms, star rooms, origin rooms, and easter eggs are NOT pointless
                !room->variant->needKey && room->variant->starCt == 0 &&
                !room->variant->easterEgg &&
                room->variant != &sRoomFacade1 && room->variant != &sRoomFacade2) {

                pointlessRoomsThisPass++;
                sDungeonRemovedPointlessRooms++;
                dungeon_deconstruct_room(room);
            }
        }
        dungeon_calculate_all_neighbor_flags();
    } while (pointlessRoomsThisPass > 0);
}

void dungeon_spawn_room_objects(void) {
    // Rooms
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if (sDungeonRoomList[i].variant == NULL) {
            // Do not spawn deconstructed rooms
            continue;
        }
        struct Object * roomObj = spawn_object(gMarioObject, sDungeonRoomList[i].variant->model ,bhvDungeonProcGenRoom);
        roomObj->oPosX = 32000.f - (sDungeonRoomList[i].xorigin * 2000.f);
        roomObj->oPosZ = 32000.f - (sDungeonRoomList[i].yorigin * 2000.f);
        roomObj->oPosY = sDungeonRoomList[i].worldY;
        roomObj->oFaceAngleYaw = sDungeonRoomList[i].direction * 0x4000;
        if (sDungeonRoomList[i].variant->collision != NULL) {
            roomObj->collisionData = segmented_to_virtual(sDungeonRoomList[i].variant->collision);
        }
        roomObj->dungeonRoom[0] = &sDungeonRoomList[i];
        roomObj->dungeonRoom[1] = &sDungeonRoomList[i];

        sDungeonRoomList[i].obj = roomObj;

        if (sDungeonRoomList[i].variant->model == MODEL_ROOM_MINIJUNC) {
            for (int j = 0; j < 4; j ++) {
                if (dungeon_door_on_other_side(sDungeonRoomList[i].xorigin,sDungeonRoomList[i].yorigin,j)) {
                    struct Object * carpetPoint = spawn_object(gMarioObject, MODEL_DUNGEON_CARPET_POINT, bhvStaticObject);
                    carpetPoint->oPosX = 32000.f - (sDungeonRoomList[i].xorigin * 2000.f);
                    carpetPoint->oPosZ = 32000.f - (sDungeonRoomList[i].yorigin * 2000.f);
                    carpetPoint->oPosY = sDungeonRoomList[i].worldY;
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

            chest->dungeonRoom[0] = &sDungeonRoomList[i];
            chest->dungeonRoom[1] = &sDungeonRoomList[i];
            chest->oFlags |= OBJ_FLAG_DUNGEON_CULL;

            if (sDungeonRoomList[i].loot[j] == MOD_NONMOD_MYSTERY_CHEST) {
                chest->oBehParams2ndByte = 0;
                if (tinymt32_generate_u32(&gGlobalRandomState)%3==0) {
                    chest->oBehParams2ndByte = 1;
                }
            }

            /*
            if ((tinymt32_generate_u32(&gGlobalRandomState)%2==0)&&sDungeonCoinBalance>=10) {
                //randomly make chests cost money
                SET_BPARAM1(chest->oBehParams,10);
                sDungeonCoinBalance-=10;
            }
            */

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
                obj->oFaceAngleYaw = angle + details->angle;
                obj->oMoveAngleYaw = angle + details->angle;
                obj->oBehParams2ndByte = details->param;
                SET_BPARAM3(obj->oBehParams,details->param3);
                SET_BPARAM4(obj->oBehParams,details->param4);

                obj->dungeonRoom[0] = &sDungeonRoomList[i];
                obj->dungeonRoom[1] = &sDungeonRoomList[i];
                obj->oFlags |= OBJ_FLAG_DUNGEON_CULL;

                if (details->bhv == bhvDungeonSpawn) {
                    vec3f_copy(gDungeonSpawnLocation,&obj->oPosVec);
                }

                j++;
            }
        }
    }

    if (sDungeonGeneratingLevelId == 2) {return;}

    // Door holes
    for (int i = 0; i < sDungeonCellProcessCount; i++) {
        // j = dir
        for (int j = 0; j < 4; j++) {
            if (sDungeonCellProcessList[i]->doorFlags & (1<<j)) {
                struct Object * doorObj;
                struct Object * doorObj2 = NULL;
                if (dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                    if (j == 0 || j == 1) {
                        BehaviorScript * doorType = bhvDungeonDoor;
                        if (sDungeonRoomList[sDungeonDoorOtherSideRet->id-1].variant->needKey) {
                            doorType = bhvDungeonDoorLocked;
                        }
                        if (sDungeonRoomList[sDungeonCellProcessList[i]->id-1].variant->needKey) {
                            doorType = bhvDungeonDoorLocked;
                        }

                        doorObj2 = spawn_object(gMarioObject, MODEL_DUNGEON_DOOR ,doorType);
                        doorObj2->oFaceAngleYaw = (j+1) * 0x4000;
                        doorObj2->dungeonRoom[0] = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];
                        if (sDungeonDoorOtherSideRet) {
                            doorObj2->dungeonRoom[1] = &sDungeonRoomList[sDungeonDoorOtherSideRet->id-1];
                        }

                        doorObj = spawn_object(gMarioObject, MODEL_DUNGEON_DOORHOLE ,bhvDungeonProcGenRoom);
                        doorObj->collisionData = segmented_to_virtual(doorhole_collision);
                    } else {
                        doorObj = spawn_object(gMarioObject, MODEL_NONE, bhvStaticObject);
                        obj_mark_for_deletion(doorObj);
                    }
                } else {
                    ModelID16 model = MODEL_DUNGEON_DOORHOLE_COVERED;
                    struct DungeonRoomVariant * variant = sDungeonRoomList[sDungeonCellProcessList[i]->id-1].variant;
                    if (variant->doorBlockModel != 0) {
                        model = variant->doorBlockModel;
                    }
                    doorObj = spawn_object(gMarioObject, model ,bhvDungeonProcGenRoom);
                    doorObj->collisionData = segmented_to_virtual(juncblock_collision);
                }
                doorObj->oPosX = (32000.f - (sDungeonCellProcessList[i]->x * 2000.f)) - (1000.f * sDirectionList[j][0]);
                doorObj->oPosZ = (32000.f - (sDungeonCellProcessList[i]->y * 2000.f)) + (1000.f * sDirectionList[j][1]);
                doorObj->oPosY = sDungeonCellProcessList[i]->worldY;
                doorObj->oFaceAngleYaw = j * 0x4000;
                doorObj->dungeonRoom[0] = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];
                doorObj->dungeonRoom[1] = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];
                if (dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                    doorObj->dungeonRoom[1] =  &sDungeonRoomList[sDungeonDoorOtherSideRet->id-1];
                }
                if (doorObj2) {
                    vec3f_copy(&doorObj2->oPosVec,&doorObj->oPosVec);
                }
            }
        }
    }
}

void dungeon_spawn_room_red_coins(void) {
    int redCoinsSpawned = 0;
    while (redCoinsSpawned < 8) {
        int x = tinymt32_generate_u32(&gGlobalRandomState) % 32;
        int y = tinymt32_generate_u32(&gGlobalRandomState) % 32;

        if (sDungeonCellGrid[y][x].id < sDungeonRedCoinRoomMax) {
            f32 fx = 32000.f - (x * 2000.f) +    (tinymt32_generate_float(&gGlobalRandomState) * 2000.0f - 1000.0f);
            f32 fz = 32000.f - (y * 2000.f) +    (tinymt32_generate_float(&gGlobalRandomState) * 2000.0f - 1000.0f);;
            struct Surface * floor;
            f32 rcy = find_red_coin_zone_floor(fx,4000.0f,fz,&floor);

            if (floor) {
                // Red coin zone found, now find normal floor beneath it
                rcy = find_floor(fx,rcy-10.0f,fz,&floor);
                if (floor) {
                    struct Object * rc = spawn_object(gMarioObject, MODEL_RED_COIN ,bhvRedCoin);
                    rc->oPosX = fx;
                    rc->oPosZ = fz;
                    rc->oPosY = rcy;
                    redCoinsSpawned++;
                }
            }
        }
    }
}

void dungeon_clear_data(void) {
    sDungeonForceRegen = FALSE;
    sDungeonEasterEggGenerated = FALSE;

    sDungeonRoomCount = 0;
    sDungeonLoopCount = 0;
    sDungeonCurrentDepth = 0;
    sDungeonLootSlotsAvailible = 0;
    sDungeonCoinBalance = 0;
    sDungeonUniqueVariantGeneratedFlags = 0;
    sDungeonCellProcessCount = 0;
    sDungeonRedCoinRoomMax = 0;
    sDungeonRemovedPointlessRooms = 0;
    
    bzero(&sDungeonRoomList, sizeof(sDungeonRoomList));
    bzero(&sDungeonCellGrid, sizeof(sDungeonCellGrid));
    bcopy(&sDungeonTotalInventory, &sDungeonInventory, sizeof(sDungeonInventory));
}

void dungeon_sync_inventory(void) {
    bcopy(&sDungeonInventory, &sDungeonTotalInventory, sizeof(sDungeonInventory));
}

void dungeon_fill_empty_treasure_rooms(void) {
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if ((sDungeonRoomList[i].variant == &sRoomTreasure || sDungeonRoomList[i].variant->requiredLoot) &&
            sDungeonRoomList[i].lootCount == 0 && sDungeonRoomList[i].variant->maxLootCt > 0) {
            sDungeonRoomList[i].lootCount = 1;
            sDungeonRoomList[i].loot[0] = MOD_NONMOD_MYSTERY_CHEST;
        }
    }
}

void dungeon_shuffle_wood_room_treasure(void) {
    f32 * randomPos = sRoomWoodObjectList[tinymt32_generate_u32(&gGlobalRandomState)%6].pos;
    sRoomWoodLootLocations[0][0] = randomPos[0];
    sRoomWoodLootLocations[0][1] = randomPos[1];
    sRoomWoodLootLocations[0][2] = randomPos[2];
}

void dungeon_generate_lv1(void) {
    sDungeonGeneratingLevelId = 0;
    
    // Determine freebie star
    if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
        // Push block puzzle
        sLv1RoomVariantList[0] = &sRoomPush;
    } else {
        // Memorize Puzzle
        sLv1RoomVariantList[0] = &sRoomMemorize;

        // Variant 1 or 2
        if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
            sRoomMemorize.collision = rmemorize_1_collision;
            sRoomMemorize.objectList[0].model = MODEL_ROOM_MEMORIZE_PANEL_1;
        } else {
            sRoomMemorize.collision = rmemorize_2_collision;
            sRoomMemorize.objectList[0].model = MODEL_ROOM_MEMORIZE_PANEL_2;
        }
    }

    // Level 1 use a fixed tree type
    gDungeonTreeModel = MODEL_DUNGEON_TREE_3;

    // Pick random enemies
    int s = sizeof(sBottomEnemyList[0]);
    gDungeonEnemies[0] = &sBottomEnemyList[tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sBottomEnemyList)/s)];
    gDungeonEnemies[1] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    gDungeonEnemies[2] = NULL;
    gDungeonEnemies[3] = NULL;

    // The dungeon boss is just enemey spam, lol
    gDungeonAboomboomination[0] = &sBottomEnemyList[tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sBottomEnemyList)/s)];
    for (int i = 1; i < 7; i++) {
        gDungeonAboomboomination[i] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    }

    // Shuffle location of chest in wood room
    dungeon_shuffle_wood_room_treasure();

    redo_generate:

    // Clear dungeon data
    dungeon_clear_data();

    // Build First Room
    dungeon_create_room(&sRoomFacade2, 0, 2, 16, 0);

    // Generate dungeon rooms
    sDungeonTargetRoomCount = 20;
    dungeon_generate_rooms_at_doors(sLv1RoomVariantList,sizeof(sLv1RoomVariantList));

    // Place the boss key
    dungeon_place_loot_in_random_previous_room(MOD_NONMOD_KEY);

    if (sDungeonForceRegen || sDungeonRoomCount < 9 || sDungeonInventory[MOD_NONMOD_STAR] < 3) {
        goto redo_generate;
    }
}

void dungeon_generate_lv2(void) {
    sDungeonGeneratingLevelId = 1;

    // Pick a random tree type
    gDungeonTreeModel = MODEL_DUNGEON_TREE_1;
    if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
        gDungeonTreeModel = MODEL_DUNGEON_TREE_2;
    }

    // Pick random enemies
    int s = sizeof(sBottomEnemyList[0]);
    gDungeonEnemies[0] = &sBottomEnemyList[tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sBottomEnemyList)/s)];
    gDungeonEnemies[1] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    gDungeonEnemies[2] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    gDungeonEnemies[3] = NULL;

    // Shuffle location of chest in wood room
    dungeon_shuffle_wood_room_treasure();

    redo_generate:

    // Clear dungeon data
    dungeon_clear_data();

    // Build First Room
    dungeon_create_room(&sRoomFacade1, 0, 2, 16, 0);

    // Generate dungeon rooms
    sDungeonTargetRoomCount = 45;
    dungeon_generate_rooms_at_doors(sLv2RoomVariantList,sizeof(sLv2RoomVariantList));

    // Place extra stars if not at 8 total
    int starDeficit = 8 - sDungeonInventory[MOD_NONMOD_STAR];
    for (int i = 0; i < starDeficit; i++) {
        dungeon_place_loot_in_random_previous_room(MOD_NONMOD_STAR);
    }

    // Place the boss room
    dungeon_generate_boss_room(&sRoomBoss);

    if (sDungeonEasterEggGenerated == FALSE) {
        sDungeonForceRegen = TRUE;
    }

    if (sDungeonLoopCount < 2 || sDungeonForceRegen || sDungeonRoomCount < 5) {
        goto redo_generate;
    }
}

void dungeon_generate_lv3(void) {
    sDungeonGeneratingLevelId = 2;

    // Pick random enemies
    int s = sizeof(sBottomEnemyList[0]);
    gDungeonEnemies[0] = &sBottomEnemyList[tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sBottomEnemyList)/s)];
    gDungeonEnemies[1] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    gDungeonEnemies[2] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];
    gDungeonEnemies[3] = &sTopEnemyList[   tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTopEnemyList)/s)   ];


    redo_generate:

    // Clear dungeon data
    dungeon_clear_data();

    // Build First Room
    dungeon_create_room(&sRoomRfFacade3, 0, 1, 16, 0);

    // Generate dungeon rooms
    sDungeonTargetRoomCount = 35;
    dungeon_generate_rooms_at_doors(sLv3RoomVariantList,sizeof(sLv3RoomVariantList));

    dungeon_generate_boss_room(&sRoomRfEnd);

    if (sDungeonForceRegen || sDungeonRoomCount < 30) {
        goto redo_generate;
    }
}

void dungeon_generate(int level) {
    // Randomize Seed
    tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);

    // Clear multi-level item tally
    bzero(&sDungeonTotalInventory, sizeof(sDungeonTotalInventory));

    switch(level) {
        case 0: // Mini Dungeon, Oasis
            dungeon_generate_lv1();
            texgen_generate_lv1();
            break;
        case 1: // Big Dungeon
            dungeon_generate_lv1();
            dungeon_sync_inventory();

#ifdef DUNGEON_DEBUG
            for (int i = 0; i < MOD_COUNT; i++) {
                for (int j = 0; j < sDungeonTotalInventory[i]; j++) {
                    if (sDungeonTotalInventory[i] == MOD_NONMOD_STAR||
                    sDungeonTotalInventory[i] == MOD_NONMOD_KEY) {
                        continue;
                    }
                    add_inventory(i);
                }
            }
#endif

            dungeon_generate_lv2();
            texgen_generate_lv2();
            break;
        case 2: // Bowser Level

            // Not needed, because bowser levels don't generate
            // with item logic

            //dungeon_generate_lv1();
            //dungeon_sync_inventory();
            //dungeon_generate_lv2();
            //dungeon_sync_inventory();

            dungeon_generate_lv3();
            texgen_generate_lv3();
            break;
    }

    dungeon_fill_empty_treasure_rooms();
    dungeon_calculate_all_neighbor_flags();
    dungeon_remove_pointless_rooms();

    dungeon_spawn_room_objects();
}

u8 sDebugColorList[][3] = {
    {255,255,255},
    {255,150,50},
    {50,50,255},
    {50,255,50},
    {255,255,50},
    {50,255,255},
};

void dungeon_debug_print(void) {
    print_text_fmt_int(30, 200, "LOOP CT %d", sDungeonLoopCount);
    print_text_fmt_int(30, 220, "ITEM SLOTS %d", sDungeonLootSlotsAvailible);

    int nflagct = 0;
    if (gDungeonMarioRoom) {
        for (int i = 0; i < 32; i++) {
            if ((gDungeonMarioRoom->neighborFlag[0] & (1<<i)) != 0) {
                nflagct++;
            }
        }
        for (int i = 0; i < 32; i++) {
            if ((gDungeonMarioRoom->neighborFlag[1] & (1<<i)) != 0) {
                nflagct++;
            }
        }
        print_text_fmt_int(30, 160, "CHLV %d", gDungeonMarioRoom->challengeLv);
    }
    print_text_fmt_int(30, 180, "NFLAGS %d", nflagct);
    print_text_fmt_int(30, 140, "SHAVED %d", sDungeonRemovedPointlessRooms);

    u32 x = (((-gMarioState->pos[0])+32000.f + 1000.f)/2000.f);
    u32 y = (((-gMarioState->pos[2])+32000.f + 1000.f)/2000.f);
    print_text_fmt_int(0, 140, "Y %d", sDungeonCellGrid[y][x].worldY);

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            if (sDungeonCellGrid[y][x].id != 0) {
                utf8_print_reset();
                int roomIndex = sDungeonCellGrid[y][x].id-1;
                if (roomIndex < 0) {return;}
                if (dungeon_room_is_visible(&sDungeonRoomList[roomIndex])) {
                    print_utf8_color(".",40+x*3, 40+y*3,
                    255,
                    255,
                    230);
                } else {
                    print_utf8_color(".",40+x*3, 40+y*3,
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][0],
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][1],
                    sDebugColorList[sDungeonCellGrid[y][x].id%6][2]);
                }
            }
        }
    }

    if (dungeon_get_mario_room()) {
        int x = dungeon_get_mario_room()->xorigin;
        int y = dungeon_get_mario_room()->yorigin;
        print_utf8_color(".",40+x*3, 40+y*3,255,0,0);
    }
}

void dungeon_set_mario_room(void) {
    gDungeonMarioRoom = dungeon_get_mario_room();

    if (gDungeonMarioRoom != NULL) {
        int id = gDungeonMarioRoom->id;
        int index = id/32;
        int flag = id%32;
        sDungeonDiscoveredFlags[index] |= (1 << flag);

        save_set_meta_flag(METAFLAGS_ROOMS,gDungeonMarioRoom->variant->metaFlag);
    }
}

void dungeon_print_minimap(f32 mapZoom) {
    create_dl_translation_matrix(MENU_MTX_PUSH, 160.f, 120.f, 0);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 0.02f * mapZoom, 0.02f * mapZoom, 1.0f);

    // Render all discovered rooms
    for (int i = 0; i < sDungeonRoomCount; i++) {

        int id = sDungeonRoomList[i].id;
        int index = id/32;
        int flag = id%32;
        
        if (sDungeonDiscoveredFlags[index] & (1 << flag)) {
            f32 dungeon_room_x = (32000.0f - (sDungeonRoomList[i].xorigin * 2000.0f)) - gMarioState->pos[0] + gMiniMapOffsetX;
            f32 dungeon_room_y = (32000.0f - (sDungeonRoomList[i].yorigin * 2000.0f)) - gMarioState->pos[2] + gMiniMapOffsetZ;

            Gfx * minimapDL = sDungeonRoomList[i].variant->minimapDL;
            if (minimapDL != NULL && ABS(dungeon_room_x) < 15000.0f && ABS(dungeon_room_y) < 10000.0f) {
                create_dl_translation_matrix(MENU_MTX_PUSH, dungeon_room_x, dungeon_room_y, 0);
                create_dl_rotation_matrix(MENU_MTX_NOPUSH, sDungeonRoomList[i].direction*-90.0f, 0, 0, 1.0f);
                gSPDisplayList(gDisplayListHead++, minimapDL);
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }

    // Render all doors in discovered rooms
    // Doors are rendered separately so that doors that lead nowhere are invisible
    for (int i = 0; i < sDungeonCellProcessCount; i++) {

        int id = sDungeonCellProcessList[i]->id-1;
        int index = id/32;
        int flag = id%32;
        
        for (int j = 0; j < 2; j++) {
            if ( ((1<<j) & sDungeonCellProcessList[i]->doorFlags) && dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                
                int id2 = sDungeonDoorOtherSideRet->id-1;
                int index2 = id2/32;
                int flag2 = id2%32;

                if ((sDungeonDiscoveredFlags[index] & (1 << flag)) || (sDungeonDiscoveredFlags[index2] & (1 << flag2))) {
                    f32 dungeon_door_x = ((32000.0f - (sDungeonCellProcessList[i]->x * 2000.0f)) - (1000.f * sDirectionList[j][0])) - gMarioState->pos[0] + gMiniMapOffsetX;
                    f32 dungeon_door_y = ((32000.0f - (sDungeonCellProcessList[i]->y * 2000.0f)) + (1000.f * sDirectionList[j][1])) - gMarioState->pos[2] + gMiniMapOffsetZ;

                    if (ABS(dungeon_door_x) < 15000.0f && ABS(dungeon_door_y) < 10000.0f) {
                        create_dl_translation_matrix(MENU_MTX_PUSH, dungeon_door_x, dungeon_door_y, 0);
                        create_dl_rotation_matrix(MENU_MTX_NOPUSH, j*-90.0f, 0, 0, 1.0f);
                        gSPDisplayList(gDisplayListHead++, rmapdoor_rmapdoor_mesh);
                        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                    }
                }
            }
        }
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

s32 is_level_dungeon(void) {
    return (gCurrLevelNum == LEVEL_ROGUE) || (gCurrLevelNum == LEVEL_RF);
}

void dungeon_clear_exploration_flags(void) {
    sDungeonDiscoveredFlags[0] = 0;
    sDungeonDiscoveredFlags[1] = 0;
}