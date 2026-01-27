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
    .minimapDL = &rmapjunc_rmapjunc_mesh,

    .cellList = &sRoomMiniJuncCellList,
    .model = MODEL_ROOM_MINIJUNC,
    .doorBlockModel = MODEL_DUNGEON_JUNCBLOCK,
    .collision = minijunc_collision,
    .objectList = &sRoomJuncObjectList1,
    .maxLootCt = 0,
    .requiredLoot = NULL,
    .generateOnce = FALSE,

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
    .minimapDL = &rmaplobby_rmaplobby_mesh,

    .cellList = &sRoomLobbyCellList,
    .model = MODEL_ROOM_LOBBY,
    .collision = rlobby_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = &sRoomLobbyLootLocations,
    .requiredLoot = NULL,
    .generateOnce = TRUE,

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
    .generateOnce = TRUE,
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
    .minimapDL = &rmapfurnace_rmapfurnace_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_FURNACE,
    .collision = rfurnace_collision,
    .objectList = NULL,
    .maxLootCt = 1,
    .lootLocations = &sRoomFurnceLootLocations,
    .requiredLoot = &sRoomFurnaceRequiredLoot,
    .generateOnce = TRUE,
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
    .minimapDL = &rmapcavejump_rmapcavejump_mesh,

    .cellList = &sRoomGardenHallCellList,
    .model = MODEL_ROOM_CAVEJUMP,
    .collision = rcavejump_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = &sRoomCaveJumpRequiredLoot,
    .generateOnce = TRUE,
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
    .minimapDL = &rmapsplithall_rmapsplithall_mesh,

    .cellList = &sRoomSplitHallCellList,
    .model = MODEL_ROOM_SPLITHALL,
    .collision = rsplithall_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,

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
    .minimapDL = &rmap_maze_rmap_maze_mesh,
    
    .cellList = &sRoomAutoMazeCellList,
    .model = MODEL_ROOM_AUTOMAZE,
    .collision = rmaze_collision,
    .objectList = &sRoomAutoMazeObjectList,
    .maxLootCt = 1,
    .starCt = 1,
    .lootLocations = &sRoomAutoMazeLootLocations,
    .requiredLoot = &sRoomAutoMazeRequiredLoot,
    .generateOnce = TRUE,
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
    {.bhv = bhvKingBobomb, .model = MODEL_KINGBOBOMB_GLOBAL, .param = 0,
    .angle = 0x4000, .pos = {-109.687f,0.0f,13.7393f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBoss = {
    .minimapDL = &rmapboss_rmapboss_mesh,

    .cellList = &sRoomBossCellList,
    .model = MODEL_ROOM_BOSS,
    .collision = rboss_collision,
    .objectList = &sRoomBossObjectList,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
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
};

struct DungeonObject sRoomFacade2ObjectList[] = {
    {.bhv = bhvDungeonDoorLocked, .model = MODEL_DUNGEON_DOOR, .param = 1,
    .angle = -0x2000, .pos = {-53.0235f,-53.0235f,5.32303f}},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomFacade2 = {
    .minimapDL = NULL,//&rmapfacade1_rmapfacade1_mesh,

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
    .minimapDL = NULL,

    .cellList = &sRoomLobby2CellList,
    .model = MODEL_ROOM_LOBBY2,
    .collision = rlobby2_collision,
    .objectList = NULL,
    .maxLootCt = 0,
    .lootLocations = NULL,
    .requiredLoot = NULL,
    .generateOnce = TRUE,
};

// Box Push Freebie Star Room

struct DungeonRoomVariantCellList sRoomPushCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},

    {.x = 1, .y = -1},
    {.x = 2, .y = -1, .doorFlags = DOOR_DOWN, .worldY = 412},

    {.end = TRUE},
};

struct DungeonObject sRoomPushObjectList[] = {
    {.bhv = bhvPushableMetalBox, .model = MODEL_METAL_BOX, .param = 0,
    .angle = 0, .pos = {-21.6136f,-2.55578f,-0.374972f}},
    {.bhv = bhvPushableMetalBox, .model = MODEL_METAL_BOX, .param = 0,
    .angle = 0, .pos = {-41.0017f,1.94423f,-0.375026f}},
    {.bhv = bhvStar, .model = MODEL_NONE, .param = 0,
    .angle = 0x0, .pos = {-26.1232f,-2.55577f,5.12489f}},
    {.end = TRUE},
};

Vec4f sRoomPushLootLocations[] = {
    {-18.1967f,-11.5693f,4.12489f,  90.f},
};

struct DungeonRoomVariant sRoomPush = {
    .minimapDL = NULL,

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