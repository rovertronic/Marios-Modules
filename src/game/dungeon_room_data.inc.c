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
    .safe = TRUE,

    .rarity = 2,
};

// Mini Junction Room

struct DungeonObject sRoomJuncObjectList1[] = {
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonObject sRoomJuncObjectList2[] = {
    {.bhv = bhvDungeonElite, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariantCellList sRoomMiniJuncCellList[] = {
    {.x = 0, .y = 0, .doorFlags = 0xF},
    {.end = TRUE},
};

// Coins variant
struct DungeonRoomVariant sRoomMiniJunc1 = {
    .metaFlag = 0,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .doorBlockModel = MODEL_DUNGEON_JUNCBLOCK,
    .collision = minijunc_collision,
    .objectList = &sRoomJuncObjectList2,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,

    .rarity = 2,
};

// Enemy spawn variant
struct DungeonRoomVariant sRoomMiniJunc2 = {
    .metaFlag = 0,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .doorBlockModel = MODEL_DUNGEON_JUNCBLOCK,
    .collision = minijunc_collision,
    .objectList = &sRoomJuncObjectList1,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
    .safe = TRUE,

    .rarity = 4,
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
    .metaFlag = 1,
    .minimapDL = &rmaplobby_rmaplobby_mesh,

    .cellList = &sRoomLobbyCellList,
    .model = MODEL_ROOM_LOBBY,
    .collision = rlobby_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = &sRoomLobbyLootLocations,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
    .safe = TRUE,

    .rarity = 2,
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
    .metaFlag = 2,
    .minimapDL = &rmaptreasure_rmaptreasure_mesh,

    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_TREASURE,
    .collision = rtresure_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomTreasureLootLocations,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
    .safe = TRUE,
};

Vec4f sRoomSuperTreasureLootLocations[] = {
    {-26.4795f,.0f,3.27412f,    90.f},
    {-0.935974f,-6.25049,0.0f,  260.0f},
    {-13.9541f,2.81523,0.0f,    70.0f},
    {-24.4241f,6.42857f,2.69053f,   120.0f},
};

struct DungeonObject sRoomSuperTreasureObjectList[] = {
    {.bhv = bhvExclamationBox, .model = MODEL_EXCLAMATION_BOX, .param = 6,
    .angle = 0, .pos = {-16.8906f,0.0f,5.32855f}},
    {.end = TRUE},
};

s8 sRoomSuperTreasureRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomSuperTreasure = {
    .metaFlag = 3,
    .minimapDL = &rmaptreasure_rmaptreasure_mesh,

    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_SUPER_TREASURE,
    .collision = rtresure_collision,
    .objectList = &sRoomSuperTreasureObjectList,
    .maxLootCt = 4,
    .lootLocations = &sRoomSuperTreasureLootLocations,
    .requiredLoot = &sRoomSuperTreasureRequiredLoot,
    .generateOnce = TRUE,
    .needKey = TRUE,

    .rarity = 4,
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
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

Vec4f sRoomWallJumpLootLocations[] = {
    {-20.f,-20.8865f,1.f,180.f},
};

struct DungeonObject sRoomWallJumpObjectList[] = {
    {.bhv = bhvExclamationBox, .model = MODEL_EXCLAMATION_BOX, .param = 6,
    .angle = 0, .pos = {-20.f,5.54237f,3.f}},


    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-14.f,-0.457629f,0.f}},
    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-26.f,11.5424f,0.f}},
    {.bhv = bhvFireSpitter, .model = MODEL_BOWLING_BALL, .param = 1,
    .angle = 0, .pos = {-14.f,11.5424f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomWallJump = {
    .metaFlag = 4,
    .minimapDL = &rmapwallkick_rmapwallkick_mesh,

    .cellList = &sRoomWallJumpCellList,
    .model = MODEL_ROOM_WALLJUMP,
    .collision = rwalljump_collision,
    .objectList = &sRoomWallJumpObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomWallJumpLootLocations,
    .requiredLoot = &sRoomWallJumpRequiredLoot,
    .generateOnce = TRUE,
    .safe = TRUE,
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
    MOD_NONMOD_STAR, 2,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomLongJump = {
    .metaFlag = 5,
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
    {5.56952f,20.3674f,0.0f,270.0f},
};

struct DungeonRoomVariant sRoomVanishHop = {
    .metaFlag = 6,
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
    .metaFlag = 7,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

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
    MOD_NONMOD_STAR, 2,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomBtcm = {
    .metaFlag = 8,
    .minimapDL = &rmapbtcm_rmapbtcm_mesh,

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
    .metaFlag = 9,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

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
    .metaFlag = 10,
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
    {.x = 1, .y = 1, .doorFlags = DOOR_UP, .worldY = -776},
    {.x = 2, .y = 1},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 2, .y = -1},

    {.end = TRUE},
};

s8 sRoomSpaceworldRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_EMPTY,
};

struct DungeonObject sRoomSpaceworldObjectList[] = {
    {.bhv = bhvDungeonElite, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-38.1277f,-17.9106f,0.001601f}},
    {.bhv = bhvDungeonElite, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-14.9495f,8.34227f,-7.76702f}},
    {.bhv = bhvCoinFormation, .model = MODEL_NONE, .param = 2,
    .angle = 0x4000, .pos = {-20.f,-8.98236,-4.6068}},
    {.end = TRUE},
};

Vec4f sRoomSpaceworldLootLocations[] = {
    {-20.f,-8.98236,-4.6068,180.0f},
    {1.42143f,-18.3056f,5.53684f,315.0f},
};

struct DungeonRoomVariant sRoomSpaceworld = {
    .metaFlag = 11,
    .minimapDL = rmapspaceworld_rmapspaceworld_mesh,

    .cellList = &sRoomSpaceworldCellList,
    .model = MODEL_ROOM_SPACEWORLD,
    .collision = spaceworld_collision,
    .objectList = &sRoomSpaceworldObjectList,
    .maxLootCt = 2,
    .lootLocations = &sRoomSpaceworldLootLocations,
    .requiredLoot = &sRoomSpaceworldRequiredLoot,
    .generateOnce = TRUE,
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
    {.bhv = bhvDungeonSpawn, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-8.16344f,1.99048f,-5.66878f}},
    {.end = TRUE},
};

Vec4f sRoomFacade1LootLocations[] = {
    {-76.7321f,-21.8378f,-7.76184,90.f},
};

struct DungeonRoomVariant sRoomFacade1 = {
    .metaFlag = 12,
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
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomGardenHall = {
    .metaFlag = 13,
    .minimapDL = &rmapgarden_rmapgarden_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_GARDENHALL,
    .collision = rgardenhall_collision,
    .objectList = NULL,//&sRoomHallObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomGardenHallLootLocations,
    .requiredLoot = &sRoomGardenHallRequiredLoot,
    .generateOnce = TRUE,
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
    MOD_NONMOD_STAR, 4,
    MOD_EMPTY,  
};

Vec4f sRoomFurnceLootLocations[] = {
    {-43.587f,-6.55491f,0.0f,0.0f},
};

struct DungeonRoomVariant sRoomFurnace = {
    .metaFlag = 14,
    .minimapDL = &rmapfurnace_rmapfurnace_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_FURNACE,
    .collision = rfurnace_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomFurnceLootLocations,
    .requiredLoot = &sRoomFurnaceRequiredLoot,
    .generateOnce = TRUE,
    .safe = TRUE,
};

// Grindr Room

struct DungeonRoomVariantCellList sRoomGrindrCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 0, .y = 1}, 
    {.x = 1, .y = 1},
    {.end = TRUE},
};

struct DungeonObject sRoomGrindrObjectList[] = {
    {.bhv = bhvRecycleInterface, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-19.9601f,12.6791f,4.38218f}},
    {.bhv = bhvRecycleChest, .model = MODEL_CHEST, .param = 0,
    .angle = 0x0, .pos = {-12.6849f,19.8072f,0.656494f}},
    {.bhv = bhvRecycleHole, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-16.9478f,12.2288f,4.38218f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomGrindr = {
    .metaFlag = 15,
    .minimapDL = &rmapgrindr_rmapgrindr_mesh,

    .cellList = &sRoomGrindrCellList,
    .model = MODEL_ROOM_GRINDR,
    .collision = rgrinder_collision,
    .objectList = &sRoomGrindrObjectList,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
    .needKey = TRUE,
};

// Cave Jump Room

s8 sRoomCaveJumpRequiredLoot[] = {

    MOD_JUMP, 2,
    MOD_HIT_WALL, 1,
    MOD_TIMER, 1,
    MOD_TORNADO, 1,
    MOD_POW, 1,
    MOD_NONMOD_STAR, 3,

    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomCaveJump = {
    .metaFlag = 16,
    .minimapDL = &rmapcavejump_rmapcavejump_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_CAVEJUMP,
    .collision = rcavejump_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = &sRoomCaveJumpRequiredLoot,
    .generateOnce = TRUE,
    .safe = TRUE,
};

// Split Hall Room

struct DungeonRoomVariantCellList sRoomSplitHallCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_UP},
    {.x = 0, .y = -1, .doorFlags = DOOR_LEFT}, 
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.x = 2, .y = -2, .doorFlags = DOOR_DOWN},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomSplitHall = {
    .metaFlag = 17,
    .minimapDL = &rmapsplithall_rmapsplithall_mesh,

    .cellList = &sRoomSplitHallCellList,
    .model = MODEL_ROOM_SPLITHALL,
    .collision = rsplithall_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
    .safe = TRUE,

    .rarity = 2,
};

// Auto-Maze Challenge Room

struct DungeonRoomVariantCellList sRoomAutoMazeCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1, .doorFlags = DOOR_UP},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.end = TRUE},
};

s8 sRoomAutoMazeRequiredLoot[] = {
    MOD_PASSIVE, 1,
    MOD_SPD, 1,
    MOD_IF_WALL, 1,
    MOD_ROTATE, 1,
    MOD_TIMER, 1,
    MOD_ATTACK, 1,
    MOD_JUMP, 1,
    MOD_CAP, 1,
    MOD_EMPTY,
};

Vec4f sRoomAutoMazeLootLocations[] = {
    {-36.4302f,-27.7367f,1.29287f,180.0f},
};

struct DungeonObject sRoomAutoMazeObjectList[] = {
    {.bhv = bhvStar, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-44.6066f,0.0f,7.32332f}},
    // Entrance
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_DISCONNECT,
    .angle = 0x0, .pos = {-5.53006f,0.0f,0.0f}},
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_RECONNECT,
    .angle = 0x0, .pos = {-1.42793f,0.0f,0.0f}},

    // Entrance 2
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_DISCONNECT,
    .angle = 0x0, .pos = {-5.53006f,-5.72149f,0.0f}},
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_RECONNECT,
    .angle = 0x0, .pos = {-1.42793f,-5.72149f,0.0f}},

    // Treasure
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_DISCONNECT,
    .angle = 0x0, .pos = {-36.9067f,-17.463f,0.0f}},
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_RECONNECT,
    .angle = 0x0, .pos = {-36.9067f,-21.5635f,0.0f}},

    //Exit
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_DISCONNECT,
    .angle = 0x0, .pos = {-36.9067f,18.5196f,0.0f}},
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_RECONNECT,
    .angle = 0x0, .pos = {-36.9067f,22.5841f,0.0f}},
    
    //Star
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_DISCONNECT,
    .angle = 0x0, .pos = {-38.5384f,0.0f,6.84446f}},
    {.bhv = bhvVolume, .model = MODEL_NONE, .param = VOLUME_RECONNECT,
    .angle = 0x0, .pos = {-42.6803f,0.0f,6.84446f}},

    {.end = TRUE},
};

struct DungeonRoomVariant sRoomAutoMaze = {
    .metaFlag = 18,
    .minimapDL = &rmapvanishhop_rmapvanishhop_mesh,
    
    .cellList = &sRoomAutoMazeCellList,
    .model = MODEL_ROOM_AUTOMAZE,
    .collision = rmaze_collision,
    .objectList = &sRoomAutoMazeObjectList,
    .maxLootCt = 1,
    .starCt = 1,
    .lootLocations = &sRoomAutoMazeLootLocations,
    .requiredLoot = &sRoomAutoMazeRequiredLoot,
    .generateOnce = TRUE,
    .safe = TRUE,
};

// Silver Pillar Room

struct DungeonRoomVariantCellList sRoomSilverPillarCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 0, .y = -1}, 
    {.x = 1, .y = -1},
    {.end = TRUE},
};

s8 sRoomSilverPillarRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_POW2, 1,
    MOD_INPUT, 1,
    MOD_TORNADO, 1,
    MOD_FLIP_VEL, 1,

    MOD_NONMOD_STAR, 2,
    MOD_EMPTY,
};

struct DungeonObject sRoomSilverPillarObjectList[] = {
    {.bhv = bhvFloorSwitchHiddenObjects, .model = MODEL_PURPLE_SWITCH, .param3 = 45, .param4 = 3,
    .angle = 0x0, .pos = {-12.8849f,-3.73941f,0.0f}},

    {.bhv = bhvHiddenObject, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {-2.59638f,0.632364f,2.77627f + 2.f}},
    {.bhv = bhvHiddenObject, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {-5.16027f,-12.2529f,4.96802f + 2.f}},
    {.bhv = bhvHiddenObject, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {-17.2566f,-12.5816f,4.00643f + 2.f}},
    {.bhv = bhvHiddenObject, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {-23.1733f,-2.26022f,3.66879f + 2.f}},
    {.bhv = bhvHiddenObject, .model = MODEL_NONE, .param = 2,
    .angle = 0x0, .pos = {-19.7956f,2.15918f,1.2763f}},

    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {0.653854f, -13.7033f, 0.f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-14.2749f, -16.3512f, 0.f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-26.1449f, -9.86834f, 0.f}},
    
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomSilverPillar = {
    .metaFlag = 19,
    .minimapDL = &rmapsilverpillar_rmapsilverpillar_mesh,

    .cellList = &sRoomSilverPillarCellList,
    .model = MODEL_ROOM_SILVER_PILLAR,
    .collision = rsilverpillar_collision,
    .objectList = &sRoomSilverPillarObjectList,
    .maxLootCt = 0,
    .starCt = 1,
    .lootLocations = NULL,
    .requiredLoot = &sRoomSilverPillarRequiredLoot,
    .generateOnce = TRUE,
};

// Blade & Sorcery Room

struct DungeonRoomVariant sRoomBns = {
    .metaFlag = 20,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_BNS,
    .collision = rbns_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .easterEgg = TRUE,
};

// Clock Room

struct DungeonRoomVariantCellList sRoomClockCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_DOWN | DOOR_UP)},
    {.x = 1, .y = 0},
    {.end = TRUE},
};

s8 sRoomClockRequiredLoot[] = {
    MOD_CAP, 1,
    MOD_EMPTY,
};

struct DungeonObject sRoomClockObjectList[] = {
    {.bhv = bhvDungeonClockHand, .model = MODEL_DUNGEON_CLOCKHAND,
    .angle = 0x0, .pos = {0.0f,0.0f,0.0f}},
    {.end = TRUE},
};

Vec4f sRoomClockLootLocations[] = {
    {-11.0f,0.0f,0.0f,  90.f},
};

struct DungeonRoomVariant sRoomClock = {
    .metaFlag = 21,
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomClockCellList,
    .model = MODEL_ROOM_CLOCK,
    .doorBlockModel = MODEL_DUNGEON_CLOCKBLOCK,
    .collision = rclock_collision,
    .objectList = &sRoomClockObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomClockLootLocations,
    .requiredLoot = &sRoomClockRequiredLoot,
    .generateOnce = TRUE,
};

// Red Coin Room

struct DungeonObject sRoomRedCoinObjectList[] = {
    {.bhv = bhvFloorSwitchGrills, .model = MODEL_PURPLE_SWITCH, .param4 = 4,
    .angle = 0x4000, .pos = {-11.7972f,0.0f,1.0f}},
    {.bhv = bhvHiddenRedCoinStar, .model = MODEL_NONE, .param = 8,
    .angle = 0x0, .pos = {0.0f,0.0f,2.0f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRedCoin = {
    .metaFlag = 22,
    .minimapDL = &rmaptreasure_rmaptreasure_mesh,

    .cellList = &sRoomTreasureCellList,
    .model = MODEL_ROOM_RED_COIN,
    .collision = rredcoin_collision,
    .objectList = &sRoomRedCoinObjectList,
    .maxLootCt = 0,
    .starCt = 1,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
};

// Boss Room

struct DungeonRoomVariantCellList sRoomBossCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 3, .y = 0},
    {.x = 4, .y = 0},
    {.x = 5, .y = 0},

    // I wish I was joking
    {.x = 6, .y = 0},
    {.x = 7, .y = 0},
    {.x = 6, .y = 1},
    {.x = 7, .y = 1},
    {.x = 6, .y = -1},
    {.x = 7, .y = -1},
    {.end = TRUE},
};

struct DungeonObject sRoomBossObjectList[] = {
    {.bhv = bhvDungeonDoorLocked, .model = MODEL_DUNGEON_DOOR, .param = 1,
    .angle = 0x4000, .pos = {-90.0236f,0.0f,16.3737f}},
    {.bhv = bhvWigglerRogue, .model = MODEL_WIGHEAD, .param = 0,
    .angle = 0x4000, .pos = {-109.687f,0.0f,13.7393f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBoss = {
    .metaFlag = 23,
    .minimapDL = &rmapboss_rmapboss_mesh,

    .cellList = &sRoomBossCellList,
    .model = MODEL_ROOM_BOSS,
    .collision = rboss_collision,
    .objectList = &sRoomBossObjectList,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
    .safe = TRUE,
    .needKey = TRUE,
};

// Facade 2 (Level 1)

struct DungeonRoomVariantCellList sRoomFacade2CellList[] = {
    {.x = 0, .y = 0},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1, .doorFlags = DOOR_RIGHT, .worldY = 400},

    {.x = 0, .y = -2},
    {.x = 1, .y = -2, .doorFlags = DOOR_DOWN},
    {.x = 2, .y = -2},

    {.x = 3, .y = -2},
    {.x = 2, .y = -3},
    {.x = 3, .y = -3},

    {.x = 4, .y = -3},
    {.x = 3, .y = -4},
    {.x = 4, .y = -4},

    {.end = TRUE},
};

struct DungeonObject sRoomFacade2ObjectList[] = {
    {.bhv = bhvDungeonDoorLocked, .model = MODEL_DUNGEON_DOOR, .param = 1,
    .angle = -0x2000, .pos = {-53.0235f,-53.0235f,5.32303f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-30.5241f,-13.7438f,6.00313f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-30.5374f,-18.9782f,5.234f}},
    {.bhv = bhvTreeDungeon, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-8.9286f,-30.9459f,3.0471f}},
    {.bhv = bhvDungeonSpawn, .model = MODEL_NONE, .param = 0,
    .angle = 0x8000, .pos = {-1.57651f,0.551928f,5.13467f}},
    {.bhv = bhvAboomboomination, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-75.7161f,-75.716f,5.32964f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomFacade2 = {
    .metaFlag = 24,
    .minimapDL = &rmapfacade2_rmapfacade2_mesh,

    .cellList = &sRoomFacade2CellList,
    .model = MODEL_ROOM_FACADE2,
    .collision = rfacade2_collision,
    .objectList = &sRoomFacade2ObjectList,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
};

// Lobby 2

struct DungeonRoomVariantCellList sRoomLobby2CellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0, .doorFlags = DOOR_RIGHT, .worldY = 560},

    {.x = 0, .y = 1,},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP},
    {.x = 2, .y = 1, .doorFlags = DOOR_UP, .worldY = 560},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 2, .y = -1, .doorFlags = DOOR_DOWN, .worldY = 560},

    {.end = TRUE},
};

struct DungeonRoomVariant sRoomLobby2 = {
    .metaFlag = 25,
    .minimapDL = &rmap_lobby2_rmap_lobby2_mesh,

    .cellList = &sRoomLobby2CellList,
    .model = MODEL_ROOM_LOBBY2,
    .collision = rlobby2_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
    .safe = TRUE,
};

// Box Push Freebie Star Room

struct DungeonRoomVariantCellList sRoomPushCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.x = 0, .y = -2},
    {.x = 1, .y = -2},
    {.x = 2, .y = -2},

    {.end = TRUE},
};

struct DungeonObject sRoomPushObjectList[] = {
    {.bhv = bhvPushableMetalBox, .model = MODEL_METAL_BOX, .param = 0,
    .angle = 0, .pos = {-2.81838f,-20.03f,-0.469083f}},
    {.bhv = bhvPushableMetalBox, .model = MODEL_METAL_BOX, .param = 0,
    .angle = 0, .pos = {-8.44341f,-20.03f,-0.469083f}},
    {.bhv = bhvStar, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-27.4333f,8.49111f,7.34387f}},
    {.end = TRUE},
};

Vec4f sRoomPushLootLocations[] = {
    {-12.4375f,1.56714f,5.15592f,  90.f},
};

struct DungeonRoomVariant sRoomPush = {
    .metaFlag = 26,
    .minimapDL = &rmappush_rmappush_mesh,

    .cellList = &sRoomPushCellList,
    .model = MODEL_ROOM_PUSH,
    .collision = rpush_collision,
    .objectList = &sRoomPushObjectList,
    .maxLootCt = 1,
    .starCt = 1,
    .lootLocations = &sRoomPushLootLocations,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
};

// Thwomp Junction

struct DungeonRoomVariantCellList sRoomThwompsCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP},
    {.x = 2, .y = 1},

    {.x = 0, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},

    {.end = TRUE},
};

s8 sRoomThwompsRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_EMPTY,
};

struct DungeonObject sRoomThwompsObjectList[] = {
    {.bhv = bhvThwomp, .model = MODEL_THWOMP, .param = 0,
    .angle = 0x4000, .pos = {-2.48417f,0.0f,0.0f}},
    {.bhv = bhvThwomp, .model = MODEL_THWOMP, .param = 0,
    .angle = 0xC000, .pos = {-37.5158f,0.0f,0.0f}},
    {.bhv = bhvThwomp, .model = MODEL_THWOMP, .param = 0,
    .angle = 0x0000, .pos = {-20.f,-17.5158f,0.0f}},
    {.bhv = bhvThwomp, .model = MODEL_THWOMP, .param = 0,
    .angle = 0x8000, .pos = {-20.f,17.5158f,0.0f}},
    {.end = TRUE},
};

Vec4f sRoomThwompsLootLocations[] = {
    {-35.5406f,7.49708f,10.7205f,0.0f},
    {-41.3561f,-27.4415f,-0.000172f,90.0f},
};

struct DungeonRoomVariant sRoomThwomps = {
    .metaFlag = 27,
    .minimapDL = &rmap_thwomps_rmap_thwomps_mesh,

    .cellList = &sRoomThwompsCellList,
    .model = MODEL_ROOM_THWOMPS,
    .collision = rthwomps_collision,
    .objectList = &sRoomThwompsObjectList,
    .maxLootCt = 2,
    .lootLocations = &sRoomThwompsLootLocations,
    .requiredLoot = &sRoomThwompsRequiredLoot,
    .generateOnce = TRUE,
};

// Memorize Room

struct DungeonRoomVariantCellList sRoomMemorizeCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 0, .y = -1},
    {.end = TRUE},
};

struct DungeonObject sRoomMemorizeObjectList[] = {
    {.bhv = bhvStaticObject, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvStar, .model = MODEL_STAR, .param = 0,
    .angle = 0x0, .pos = {-39.099f,0.f,2.f}},
    {.end = TRUE},
};

Vec4f sRoomMemorizeLootLocations[] = {
    {-35.9208f,-5.39018f,-3.69411f,90.0f},
};

struct DungeonRoomVariant sRoomMemorize = {
    .metaFlag = 28,
    .minimapDL = &rmapgarden_rmapgarden_mesh,

    .cellList = &sRoomMemorizeCellList,
    .model = MODEL_ROOM_MEMORIZE,
    .collision = rmemorize_1_collision,
    .objectList = &sRoomMemorizeObjectList,
    .maxLootCt = 0,
    .starCt = 1,
    .lootLocations = &sRoomMemorizeLootLocations,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
};

// Room Lava Drop

s8 sRoomLavaDropRequiredLoot[] = {
    MOD_PASSIVE, 1,
    MOD_REWIND_TIME, 1,
    MOD_IF_SENSOR, 1,
    MOD_EMPTY,
};

struct DungeonObject sRoomLavaDropObjectList[] = {
    {.bhv = bhvFloorSwitchHiddenObjects, .model = MODEL_PURPLE_SWITCH, .param = 0, .param3 = 200, .param4 = 3,
    .angle = 0x0, .pos = {-24.3105f,1.23448f,0.0f}},

    {.bhv = bhvSilverStar, .model = MODEL_SILVER_STAR, .param = 0,
    .angle = 0x0, .pos = {-27.6758f,16.9188f,-15.8636f}},
    {.bhv = bhvSilverStar, .model = MODEL_SILVER_STAR, .param = 0,
    .angle = 0x0, .pos = {-16.2968f,1.66695f,-15.8636f}},
    {.bhv = bhvSilverStar, .model = MODEL_SILVER_STAR, .param = 0,
    .angle = 0x0, .pos = {-16.2968f,7.69899f,-15.8636f}},
    {.bhv = bhvSilverStar, .model = MODEL_SILVER_STAR, .param = 0,
    .angle = 0x0, .pos = {2.44177f,25.895f,-15.8636f}},
    {.bhv = bhvSilverStar, .model = MODEL_SILVER_STAR, .param = 0,
    .angle = 0x0, .pos = {2.68387f,16.9237f,-15.8636f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomLavaDrop = {
    .metaFlag = 29,
    .minimapDL = &rmapgrindr_rmapgrindr_mesh,

    .cellList = &sRoomGrindrCellList,
    .model = MODEL_ROOM_LAVADROP,
    .collision = rlavadrop_collision,
    .objectList = &sRoomLavaDropObjectList,
    .maxLootCt = 0,
    .starCt = 1,
    .lootLocations = NULL,
    .requiredLoot = &sRoomLavaDropRequiredLoot,
    .generateOnce = TRUE,
};

// Room Flip Puzzle

struct DungeonObject sRoomFlipPuzzleObjectList[] = {
    {.bhv = bhvStar, .model = MODEL_STAR, .param = 0,
    .angle = 0x0, .pos = {-25.5208f,12.6797f,3.43345f}},
    {.bhv = bhvFlipPuzzle, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-5.16932f,13.0536f,0.773787f}},
    {.bhv = bhvMessagePanel, .model = MODEL_WOODEN_SIGNPOST, .param = NEWTEXT_ANTIVANISH_2,
    .angle = 0x4000, .pos = {-22.4621f,6.67959f,0.f}},
    {.bhv = bhvFpBar, .model = MODEL_DUNGEON_FP_BAR, .param = 0,
    .angle = 0x0, .pos = {-23.562f,12.6797f,0.f}},
    {.end = TRUE},
};

s8 sRoomFlipPuzzleRequiredLoot[] = {
    MOD_JUMP, 1,
    MOD_GRAV, 1,
    MOD_ZACTION, 1,
    MOD_NONMOD_STAR, 1,
    MOD_EMPTY,
};

struct DungeonRoomVariant sRoomFlipPuzzle = {
    .metaFlag = 30,
    .minimapDL = &rmapgrindr_rmapgrindr_mesh,

    .cellList = &sRoomGrindrCellList,
    .model = MODEL_ROOM_FLIPPUZZLE,
    .collision = rflippuzzle_collision,
    .objectList = &sRoomFlipPuzzleObjectList,
    .maxLootCt = 0,
    .starCt = 1,
    .lootLocations = NULL,
    .requiredLoot = &sRoomFlipPuzzleRequiredLoot,
    .generateOnce = TRUE,
};

// Scarlet Devil Mansion Lobby

struct DungeonRoomVariantCellList sRoomSdmCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},

    {.x = 0, .y = 1, .doorFlags = DOOR_UP},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP, .worldY = 1000},

    {.x = 0, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN, .worldY = 1000},

    {.end = TRUE},
};

struct DungeonObject sRoomSdmObjectList[] = {
    {.bhv = bhvExclamationBox, .model = MODEL_EXCLAMATION_BOX, .param = 6,
    .angle = 0, .pos = {5.63327f,-25.6026f,14.f}},
    {.bhv = bhvDungeonElite, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {-27.1766f,0.0f,10.f}},
    {.end = TRUE},
};

Vec4f sRoomSdmLootLocations[] = {
    {5.63327f,25.6026f,10.f,270.0f},
};

struct DungeonRoomVariant sRoomSdm = {
    .metaFlag = 34,
    .minimapDL = &rmap_sdm_rmap_sdm_mesh,

    .cellList = &sRoomSdmCellList,
    .model = MODEL_ROOM_SDM,
    .collision = rsdm_collision,
    .objectList = &sRoomSdmObjectList,
    .maxLootCt = 1,
    .lootLocations = &sRoomSdmLootLocations,
    .requiredLoot = sRoomSpaceworldRequiredLoot,
    .generateOnce = TRUE,
    .safe = TRUE,
};

// Rogue Final: Paths

struct DungeonObject sRoomRfStraight1ObjectList[] = {
    {.bhv = bhvDungeonElite, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariantCellList sRoomRfStraightCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_RIGHT)},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfStraight = {
    .cellList = &sRoomRfStraightCellList,
    .model = MODEL_ROOM_RF_STRAIGHT,
    .collision = rf_straight_collision,

    .maxLootCt = 0,
    .objectList = &sRoomRfStraight1ObjectList,
    .requiredLoot = NULL,
};

struct DungeonRoomVariantCellList sRoomRfRightCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_UP)},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfRight = {
    .cellList = &sRoomRfRightCellList,
    .model = MODEL_ROOM_RF_RIGHT,
    .collision = rf_right_collision,
    .safe = TRUE,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 2,
};

struct DungeonRoomVariantCellList sRoomRfLeftCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_DOWN)},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfLeft = {
    .cellList = &sRoomRfLeftCellList,
    .model = MODEL_ROOM_RF_LEFT,
    .collision = rf_left_collision,
    .safe = TRUE,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 2,
};

// Final Ramp

struct DungeonRoomVariantCellList sRoomRfRampCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_RIGHT), .worldY = 2000},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfRamp = {
    .cellList = &sRoomRfRampCellList,
    .model = MODEL_ROOM_RF_RAMP,
    .collision = rf_ramp_collision,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 5,
    .generateOnce = TRUE,
};

// Final Spinning Platform

struct DungeonObject sRoomRfSpinningObjectList[] = {
    {.bhv = bhvRfSpinner, .model = MODEL_RF_SPINNER, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfSpinning = {
    .cellList = &sRoomRfStraightCellList,
    .model = MODEL_NONE,
    .collision = NULL,

    .maxLootCt = 0,
    .objectList = &sRoomRfSpinningObjectList,
    .requiredLoot = NULL,

    .rarity = 2,
};

// Final Tilting Wooden Platform
struct DungeonRoomVariantCellList sRoomRfTiltingCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT|DOOR_RIGHT), .worldY = 500},
    {.end = TRUE},
};

struct DungeonObject sRoomRfTiltingObjectList[] = {
    {.bhv = bhvSeesawPlatform, .model = MODEL_RF_TILTING, .param = 8,
    .angle = 0x4000, .pos = {0.f,0.f,1.5f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfTilting = {
    .cellList = &sRoomRfTiltingCellList,
    .model = MODEL_ROOM_RF_TILTING,
    .collision = NULL,

    .maxLootCt = 0,
    .objectList = &sRoomRfTiltingObjectList,
    .requiredLoot = NULL,

    .rarity = 2,
};

// Final Squarish Platforms

struct DungeonObject sRoomRfSquarishObjectList[] = {
    {.bhv = bhvRfSquarish, .model = MODEL_RF_SQUARISH, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.bhv = bhvRfSquarish, .model = MODEL_RF_SQUARISH, .param = 1,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfSquarish = {
    .cellList = &sRoomRfStraightCellList,
    .model = MODEL_ROOM_RF_SQUARISH,
    .collision = NULL,

    .maxLootCt = 0,
    .objectList = &sRoomRfSquarishObjectList,
    .requiredLoot = NULL,

    .rarity = 2,
};

// Final Amp Path

struct DungeonRoomVariantCellList sRoomRfAmplectedCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1, .doorFlags = DOOR_UP, .worldY = 1000},
    {.end = TRUE},
};

struct DungeonObject sRoomRfAmplectedObjectList[] = {
    {.bhv = bhvCirclingAmp, .model = MODEL_AMP, .param = 0,
    .angle = 0, .pos = {-9.10144f,-1.84873f,2.51063f}},
    {.bhv = bhvCirclingAmp, .model = MODEL_AMP, .param = 0,
    .angle = 0, .pos = {-18.9429f,16.3522f,5.23763f}},
    {.bhv = bhvCirclingAmp, .model = MODEL_AMP, .param = 0,
    .angle = 0, .pos = {-31.3328f,25.1212f,10.2415f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfAmplected = {
    .cellList = &sRoomRfAmplectedCellList,
    .model = MODEL_ROOM_RF_AMPLECTED,
    .collision = rf_amplected_collision,

    .maxLootCt = 0,
    .objectList = &sRoomRfAmplectedObjectList,
    .requiredLoot = NULL,

    .rarity = 4,
};

// Final Rino
struct DungeonRoomVariantCellList sRoomRfRinoCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT|DOOR_RIGHT), .worldY = 1000},
    {.end = TRUE},
};

struct DungeonObject sRoomRfRinoObjectList[] = {
    {.bhv = bhvRfRinoX, .model = MODEL_RF_RINO, .param = 0,
    .angle = -0x4000, .pos = {0.f,0.f,5.f}},
    {.bhv = bhvRfRinoPlat, .model = MODEL_RF_RINO_PLAT, .param = 0,
    .angle = -0x4000, .pos = {0.f,0.f,5.f}},
    {.bhv = bhvRfRinoPlat, .model = MODEL_RF_RINO_PLAT, .param = 1,
    .angle = -0x4000, .pos = {0.f,0.f,5.f}},
    {.bhv = bhvRfRinoPlat, .model = MODEL_RF_RINO_PLAT, .param = 2,
    .angle = -0x4000, .pos = {0.f,0.f,5.f}},
    {.bhv = bhvRfRinoPlat, .model = MODEL_RF_RINO_PLAT, .param = 3,
    .angle = -0x4000, .pos = {0.f,0.f,5.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfRino = {
    .cellList = &sRoomRfRinoCellList,
    .model = MODEL_ROOM_RF_RINO,
    .collision = NULL,

    .maxLootCt = 0,
    .objectList = &sRoomRfRinoObjectList,
    .requiredLoot = NULL,

    .rarity = 2,
};

// Final Challenge Ice Slope

struct DungeonRoomVariantCellList sRoomRfSlopeCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfSlope = {
    .metaFlag = 31,
    .cellList = &sRoomRfSlopeCellList,
    .model = MODEL_ROOM_RF_SLOPE,
    .collision = &rf_slope_collision,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 4,
    .generateOnce = TRUE,
};

// Final Challenge 4000 unit horizontal jump

struct DungeonRoomVariantCellList sRoomRfBlankJumpCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfBlankJump = {
    .metaFlag = 32,
    .cellList = &sRoomRfBlankJumpCellList,
    .model = MODEL_ROOM_RF_BLANKJUMP,
    .collision = NULL,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 4,
    .generateOnce = TRUE,
};

// Final Challenge 2000 unit vertical jump with wallkick assistance

struct DungeonRoomVariant sRoomRfVertical = {
    .metaFlag = 33,
    .cellList = &sRoomRfRampCellList,
    .model = MODEL_ROOM_RF_VERTICAL,
    .collision = rf_vertical_collision,

    .maxLootCt = 0,
    .objectList = NULL,
    .requiredLoot = NULL,

    .rarity = 3,
    .generateOnce = TRUE,
};

// Room Facade 3

struct DungeonObject sRoomFacade3ObjectList[] = {
    {.bhv = bhvDungeonSpawn, .model = MODEL_NONE, .param = 0,
    .angle = 0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomRfFacade3 = {
    .cellList = &sRoomRfStraightCellList,
    .model = MODEL_ROOM_RF_STRAIGHT,
    .collision = rf_straight_collision,

    .maxLootCt = 0,
    .objectList = &sRoomFacade3ObjectList,
    .requiredLoot = NULL,
};

// Final Room Ending
struct DungeonObject sRoomRfEndObjectList[] = {
    {.bhv = bhvWarpPipe, .model = MODEL_GLOBAL_PIPE, .param = 1,
    .angle = 0, .pos = {-31.1418f,0.f,1.64602f}},
    {.bhv = bhvChest, .model = MODEL_CHEST, .param = MOD_FIREBALL,
    .angle = 0x4000, .pos = {-26.1418f,0.f,1.64602f}},
    {.end = TRUE},
};


struct DungeonRoomVariant sRoomRfEnd = {
    .cellList = &sRoomRfBlankJumpCellList,
    .model = MODEL_ROOM_RF_END,
    .collision = rf_end_collision,

    .maxLootCt = 0,
    .objectList = sRoomRfEndObjectList,
    .requiredLoot = NULL,
};

// Personalized "Ending" room
struct DungeonObject sRoomPersonalizedEndObjectList[] = {
    {.bhv = bhvWarpPipe, .model = MODEL_GLOBAL_PIPE, .param = 2,
    .angle = 0x0, .pos = {0.f,0.f,0.f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomPersonalizedEnd = {
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .doorBlockModel = MODEL_DUNGEON_JUNCBLOCK,
    .collision = minijunc_collision,
    .objectList = &sRoomPersonalizedEndObjectList,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,
    .needKey = TRUE,
};