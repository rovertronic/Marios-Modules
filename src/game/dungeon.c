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

#include "levels/rogue/header.h"

struct DungeonRoom * gDungeonMarioRoom = NULL;
u32 sDungeonDiscoveredFlags[2];

int sDungeonRoomCount = 0;
int sDungeonLoopCount = 0;
int sDungeonCurrentDepth = 0;
int sDungeonLootSlotsAvailible = 0;
int sDungeonCoinBalance = 0;
u32 sDungeonUniqueVariantGeneratedFlags;

u16 gDungeonTreeModel = 0;

struct DungeonRoom sDungeonRoomList[64];
struct DungeonCell sDungeonCellGrid[32][32];

int sDungeonCellProcessCount = 0;
struct DungeonCell * sDungeonCellProcessList[256];

u8 sDungeonInventory[MOD_COUNT]; // Index = Mod Type, Value = Count
u8 sDungeonForceRegen = FALSE;
u8 sDungeonEasterEggGenerated = FALSE;

s8 sDirectionList[4][2] = {
    { 1, 0}, // Right
    { 0, 1}, // Down
    {-1, 0}, // Left
    { 0,-1}, // Up
};

/* ROOM DATA */

// Hallway Room

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
    .minimapDL = &rmaphall_rmaphall_mesh,

    .cellList = &sRoomHallCellList,
    .model = MODEL_ROOM_SHORT_HALL,
    .collision = smallhall_collision,
    .objectList = &sRoomHallObjectList,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

// Mini Junction Room

struct DungeonRoomVariantCellList sRoomMiniJuncCellList[] = {
    {.x = 0, .y = 0, .doorFlags = 0xF},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomMiniJunc = {
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .collision = minijunc_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

// Lobby Room

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
    .minimapDL = &rmaplobby_rmaplobby_mesh,

    .cellList = &sRoomLobbyCellList,
    .model = MODEL_ROOM_LOBBY,
    .collision = rlobby_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomLobbyLootLocations,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

// Treasure Room

struct DungeonRoomVariantCellList sRoomTreasureCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.end = TRUE},
};

Vec4f sRoomTreasureLootLocations[] = {
    {-26.4795f,.0f,3.27412f,90.f},
};

struct DungeonRoomVariant sRoomTreasure = {
    .minimapDL = &rmaptreasure_rmaptreasure_mesh,

    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_TREASURE,
    .collision = rtresure_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomTreasureLootLocations,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
};

// Wall Kick Challenge Room

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

struct DungeonObject sRoomWallJumpObjectList[] = {
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 1,
    .angle = 0, .pos = {-20.f+.4f,5.54237f,0.f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 1,
    .angle = 0, .pos = {-20.f-.4f,5.54237f,0.f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 1,
    .angle = 0, .pos = {-20.f,    5.54237f+.4f,0.f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 1,
    .angle = 0, .pos = {-20.f,    5.54237f-.4f,0.f}},


    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-14.f,-0.457629f,0.f}},
    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-26.f,11.5424f,0.f}},
    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-14.f,11.5424f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomWallJump = {
    .minimapDL = &rmapwallkick_rmapwallkick_mesh,

    .cellList = &sRoomWallJumpCellList,
    .model = MODEL_ROOM_WALLJUMP,
    .collision = rwalljump_collision,
    .objectList = &sRoomWallJumpObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomWallJumpRequiredLoot,
    .generateOnce = FALSE,
};

// Long Jump Rotate Challenge Room

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
    .minimapDL = &rzmapigzag_rzmapigzag_mesh,
    
    .cellList = &sRoomLongJumpCellList,
    .model = MODEL_ROOM_LONGJUMP,
    .collision = rlongjump_collision,
    .objectList = NULL,
    .requiredLoot = sRoomLongJumpRequiredLoot,
    .generateOnce = FALSE,
};

// Vanish Cap Jump Challenge Room

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

Vec4f sRoomVanishHopLootLocations[] = {
    {5.56952f,20.3674f,270.0f},
};

struct DungeonRoomVariant sRoomVanishHop = {
    .minimapDL = &rmapvanishhop_rmapvanishhop_mesh,
    
    .cellList = &sRoomVanishHopCellList,
    .model = MODEL_ROOM_VANISHHOP,
    .collision = vanishhop_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomVanishHopLootLocations,
    .requiredLoot = &sRoomVanishHopRequiredLoot,
    .generateOnce = TRUE,
};

// Nightshift at the Beta Museum Easter Egg Room

struct DungeonRoomVariantCellList sRoomFnabCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT|DOOR_RIGHT)},
    {.x = 0, .y = 1},
    {.end = TRUE},
};

s8 sRoomFnabRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_CAP, 1,
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomFnab = {
    .cellList = &sRoomFnabCellList,
    .model = MODEL_ROOM_FNAB,
    .collision = rfnab_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomFnabRequiredLoot,
    .easterEgg = TRUE,
};

// Beyond the Cursed Mirror Easter Egg Room

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

s8 sRoomBtcmRequiredLoot[] = {
    MOD_JUMP, 2,
    MOD_HIT_WALL, 1,
    MOD_POW2, 1,
    MOD_NONMOD_STAR, 3,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomBtcm = {
    .cellList = &sRoomBtcmCellList,
    .model = MODEL_ROOM_BTCM,
    .collision = rbtcm_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomBtcmRequiredLoot,
    .easterEgg = TRUE,
};

// Baldi Easter Egg Room

s8 sRoomBaldiRequiredLoot[] = {
    MOD_ATTACK, 1,
    MOD_EMPTY,
};

struct DungeonObject sRoomBaldiObjectList[] = {
    {.bhv = bhvBaldiDoor, .model = MODEL_DUNGEON_BALDI_DOOR, .param = 0,
    .angle = 0x0, .pos = {-2.26746f,0.f,0.f}},
    {.bhv = bhvMysteryChest, .model = MODEL_MCHEST, .param = 1,
    .angle = 0x4000, .pos = {-13.3897f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBaldi = {
    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_BALDI,
    .collision = baldi_collision,
    .objectList = &sRoomBaldiObjectList,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = &sRoomBaldiRequiredLoot,
    .easterEgg = TRUE,
};

// Wood Storage Room

struct DungeonObject sRoomWoodObjectList[] = {
    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {-5.f, -5.f,0.f}},
    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {-5.f, 5.f, 0.f}},
    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {-5.f, 0.f, 0.f}},

    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {.0f, -5.f,0.f}},
    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {.0f, 5.f, 0.f}},
    {.bhv = bhvBreakableBox, .model = MODEL_BREAKABLE_BOX, .param = 0,
    .angle = 0x0, .pos = {.0f, 0.f, 0.f}},

    {.end = TRUE},
};

struct DungeonRoomVariantCellList sRoomWoodCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.end = TRUE},
};

s8 sRoomWoodRequiredLoot[] = {
    MOD_ATTACK, 1,
    MOD_EMPTY
};

Vec4f sRoomWoodLootLocations[] = {
    {-5.0f,0.0f,0.0f,90.0f},
};

struct DungeonRoomVariant sRoomWood = {
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomWoodCellList,
    .model = MODEL_ROOM_WOOD,
    .collision = rboxes_collision,
    .objectList = &sRoomWoodObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomWoodLootLocations,
    .requiredLoot = &sRoomWoodRequiredLoot,
    .generateOnce = TRUE,
};

// Spaceworld Junction/Challenge Room

struct DungeonRoomVariantCellList sRoomSpaceworldCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 2, .y = -1},

    {.x = 1, .y = 2, .doorFlags = DOOR_UP},

    {.end = TRUE},
};

s8 sRoomSpaceworldRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomSpaceworld = {
    .minimapDL = rmapspaceworld_rmapspaceworld_mesh,

    .cellList = &sRoomSpaceworldCellList,
    .model = MODEL_ROOM_SPACEWORLD,
    .collision = spaceworld_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    //.lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomSpaceworldRequiredLoot,
};

// Facade 1 Room

struct DungeonRoomVariantCellList sRoomFacade1CellList[] = {
    {.x = 0, .y = 0},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 3, .y = 0, .doorFlags = DOOR_RIGHT},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},
    {.x = 3, .y = 1},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},
    {.x = 3, .y = -1},

    {.x = 4, .y = -1},

    {.end = TRUE},
};

struct DungeonObject sRoomFacade1ObjectList[] = {
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-16.7277f, -3.57431f, -3.86778f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-19.8878f, 3.55515f, -2.78238f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-25.2649f,-0.354316f,-0.866209f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-37.1143f,-18.4017f,-0.868808f}},
    {.end = TRUE},
};

Vec4f sRoomFacade1LootLocations[] = {
    {-76.7321f,-21.8378f,-7.76184,90.f},
};

struct DungeonRoomVariant sRoomFacade1 = {
    .minimapDL = &rmapfacade1_rmapfacade1_mesh,

    .cellList = &sRoomFacade1CellList,
    .model = MODEL_ROOM_FACADE1,
    .collision = facade1_collision,
    .objectList = &sRoomFacade1ObjectList,
    .maxLootCt = 0,
    .lootLocations = &sRoomFacade1LootLocations,
    .requiredLoot = NULL,
};

// Garden Hallway Room

struct DungeonRoomVariantCellList sRoomGardenHallCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

/*
struct DungeonObject sRoomGardenHallObjectList[] = {
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 0,
    .angle = 0x4000, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 0,
    .angle = 0x4000, .pos = {-20.f,0.f,0.f}},
    {.end = TRUE},
};
*/

Vec4f sRoomGardenHallLootLocations[] = {
    {-35.9208f,-5.39018f,-3.69411f,90.0f},
};

s8 sRoomGardenHallRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomGardenHall = {
    .minimapDL = &rmapgarden_rmapgarden_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_GARDENHALL,
    .collision = rgardenhall_collision,
    .objectList = NULL,//&sRoomHallObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomGardenHallLootLocations,
    .requiredLoot = &sRoomGardenHallRequiredLoot,
    .generateOnce = FALSE,
};

// Furnace Room

s8 sRoomFurnaceRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_POW2, 1,
    MOD_GRAV, 1,
    MOD_PLATFORM, 2,
    MOD_ZACTION, 1,
    MOD_TIMER, 1,
    MOD_REPEAT, 1,
    MOD_NONMOD_STAR, 5,
    MOD_EMPTY,  
};

Vec4f sRoomFurnceLootLocations[] = {
    {-43.587f,-6.55491f,0.0f,0.0f},
};

struct DungeonRoomVariant sRoomFurnace = {
    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_FURNACE,
    .collision = rfurnace_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomFurnceLootLocations,
    .requiredLoot = &sRoomFurnaceRequiredLoot,
    .generateOnce = TRUE,
};

struct DungeonRoomVariant * sRoomVariantList[] = {
    &sRoomHall,
    &sRoomMiniJunc,
    &sRoomLobby,
    &sRoomSpaceworld,
    &sRoomGardenHall,
    &sRoomTreasure,

    // Challenge Rooms
    &sRoomLongJump,
    &sRoomVanishHop,
    &sRoomWallJump,
    &sRoomWood,
    &sRoomFurnace,

    // Easter-Egg Rooms
    &sRoomFnab,
    &sRoomBtcm,
    &sRoomBaldi,
};

struct DungeonRoom * dungeon_get_mario_room(void) {
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
            (sDungeonCellGrid[y][x].doorFlags & oppositeflag)) {
            sDungeonDoorOtherSideRet = &sDungeonCellGrid[y][x];
            return TRUE;
        }
    }
    sDungeonDoorOtherSideRet = NULL;
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

void dungeon_room_set_neighbor_flag(struct DungeonRoom * room, int id) {
    int index = id/32;
    int flag = id%32;
    room->neighborFlag[index] |= (1 << flag);
}

s32 dungeon_room_is_visible(struct DungeonRoom * room) {
    if (room == NULL) {return TRUE;}
    if (gDungeonMarioRoom == NULL) {return TRUE;}

    int id = room->id;
    int index = id/32;
    int flag = id%32;
    return (gDungeonMarioRoom->neighborFlag[index] & (1 << flag))!=0;
}

struct DungeonRoom * dungeon_create_room(struct DungeonRoomVariant * variant, int dir, int x, int y) {
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

    dungeon_room_set_neighbor_flag(thisRoom,sDungeonRoomCount-1); //self

    thisRoom->lootCount = 0;
    for (int i = 0; i < 4; i++) {
        thisRoom->loot[i] = MOD_EMPTY;
    }

    sDungeonLootSlotsAvailible+=variant->maxLootCt;

    return thisRoom;
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

        struct DungeonRoom * origin_room = &sDungeonRoomList[sDungeonCellProcessList[i]->id-1];

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

                    if (selectedVariant->easterEgg && sDungeonCurrentDepth < 6) {
                        trycount++;
                        continue;
                    } 

                    if (sDungeonEasterEggGenerated && selectedVariant->easterEgg) {
                        // only generate 1 easter egg per level, and farther in
                        trycount++;
                        continue;
                    }

                    if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                        sDungeonUniqueVariantGeneratedFlags |= (1<<selectedVariantIndex);

                        if (selectedVariant->easterEgg) {
                            sDungeonEasterEggGenerated = TRUE;
                        }

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

void dungeon_calculate_all_neighbor_flags(void) {
    for (int i = 0; i < sDungeonCellProcessCount; i++) {
        struct DungeonCell * cell = sDungeonCellProcessList[i];
        for (int j = 0; j < 4; j++) {
            if (dungeon_door_on_other_side(cell->x,cell->y,j)) {
                dungeon_room_set_neighbor_flag( &sDungeonRoomList[cell->id-1] , sDungeonDoorOtherSideRet->id-1);
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
        roomObj->dungeonRoom[0] = &sDungeonRoomList[i];
        roomObj->dungeonRoom[1] = &sDungeonRoomList[i];

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
                struct Object * doorObj2 = NULL;
                if (dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                    if (j == 0 || j == 1) {
                        doorObj2 = spawn_object(gMarioObject, MODEL_DUNGEON_DOOR ,bhvDungeonDoor);
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
                    doorObj = spawn_object(gMarioObject, MODEL_DUNGEON_DOORHOLE_COVERED ,bhvDungeonProcGenRoom);
                    doorObj->collisionData = segmented_to_virtual(doorhole_covered_collision);
                }
                doorObj->oPosX = (32000.f - (sDungeonCellProcessList[i]->x * 2000.f)) - (1000.f * sDirectionList[j][0]);
                doorObj->oPosZ = (32000.f - (sDungeonCellProcessList[i]->y * 2000.f)) + (1000.f * sDirectionList[j][1]);
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

void dungeon_generate(void) {
    // Randomize Seed
    tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);

    texgen_generate();
    gDungeonTreeModel = MODEL_DUNGEON_TREE_1;
    if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
        gDungeonTreeModel = MODEL_DUNGEON_TREE_2;
    }

    redo_generate:
    sDungeonForceRegen = FALSE;
    sDungeonEasterEggGenerated = FALSE;

    // Clear dungeon data
    sDungeonRoomCount = 0;
    sDungeonLoopCount = 0;
    sDungeonCurrentDepth = 0;
    sDungeonLootSlotsAvailible = 0;
    sDungeonCoinBalance = 0;
    sDungeonUniqueVariantGeneratedFlags = 0;
    sDungeonCellProcessCount = 0;
    
    bzero(&sDungeonRoomList, sizeof(sDungeonRoomList));
    bzero(&sDungeonCellGrid, sizeof(sDungeonCellGrid));
    bzero(&sDungeonInventory, sizeof(sDungeonInventory));

    // Build First Room
    dungeon_create_room(&sRoomFacade1  ,0,16,16);

    for (int i = 0; i < 50; i++) {
        dungeon_generate_rooms_at_doors();
    }

    // Manually fill remaining empty treasure rooms and challenge rooms with mystery chests
    for (int i = 0; i < sDungeonRoomCount; i++) {
        if ((sDungeonRoomList[i].variant == &sRoomTreasure || sDungeonRoomList[i].variant->requiredLoot) &&
            sDungeonRoomList[i].lootCount == 0 && sDungeonRoomList[i].variant->maxLootCt > 0) {
            sDungeonRoomList[i].lootCount = 1;
            sDungeonRoomList[i].loot[0] = MOD_NONMOD_MYSTERY_CHEST;
        }
    }

    if (sDungeonEasterEggGenerated == FALSE) {
        sDungeonForceRegen = TRUE;
    }

    if (sDungeonLoopCount < 2 || sDungeonForceRegen || sDungeonRoomCount < 5) {
        goto redo_generate;
    }

    // Calculate cull flags
    dungeon_calculate_all_neighbor_flags();

    // Finished, spawn objects
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
    print_text_fmt_int(0, 200, "LOOP CT %d", sDungeonLoopCount);
    print_text_fmt_int(0, 220, "ITEM SLOTS %d", sDungeonLootSlotsAvailible);

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
    }
    print_text_fmt_int(0, 180, "NFLAGS %d", nflagct);

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
            f32 dungeon_room_x = (32000.0f - (sDungeonRoomList[i].xorigin * 2000.0f)) - gMarioState->pos[0];
            f32 dungeon_room_y = (32000.0f - (sDungeonRoomList[i].yorigin * 2000.0f)) - gMarioState->pos[2];

            Gfx * minimapDL = sDungeonRoomList[i].variant->minimapDL;
            if (minimapDL != NULL) {
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
            if (dungeon_door_on_other_side(sDungeonCellProcessList[i]->x,sDungeonCellProcessList[i]->y,j)) {
                
                int id2 = sDungeonDoorOtherSideRet->id-1;
                int index2 = id2/32;
                int flag2 = id2%32;

                if ((sDungeonDiscoveredFlags[index] & (1 << flag)) || (sDungeonDiscoveredFlags[index2] & (1 << flag2))) {
                    f32 dungeon_door_x = ((32000.0f - (sDungeonCellProcessList[i]->x * 2000.0f)) - (1000.f * sDirectionList[j][0])) - gMarioState->pos[0];
                    f32 dungeon_door_y = ((32000.0f - (sDungeonCellProcessList[i]->y * 2000.0f)) + (1000.f * sDirectionList[j][1])) - gMarioState->pos[2];

                    create_dl_translation_matrix(MENU_MTX_PUSH, dungeon_door_x, dungeon_door_y, 0);
                    create_dl_rotation_matrix(MENU_MTX_NOPUSH, j*-90.0f, 0, 0, 1.0f);
                    gSPDisplayList(gDisplayListHead++, rmapdoor_rmapdoor_mesh);
                    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                }
            }
        }
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}