#include "sm64.h"
#include "types.h"

#ifndef DUNGEON_H
#define DUNGEON_H

//#define DUNGEON_DEBUG

extern struct DungeonRoom * gDungeonMarioRoom;
extern struct DungeonObject * gDungeonEnemies[3];
extern struct DungeonObject * gDungeonAboomboomination[7];
extern u16 gDungeonTreeModel;
extern Vec3f gDungeonSpawnLocation;
extern u8 gDungeonNeedsToGenerate;

#define DOOR_RIGHT (1<<0)
#define DOOR_DOWN  (1<<1)
#define DOOR_LEFT  (1<<2)
#define DOOR_UP    (1<<3)

struct DungeonObject {
    BehaviorScript bhv;
    ModelID16 model;
    s16 angle;
    u8 param;
    u8 param3;
    u8 param4;
    Vec3f pos;
    u8 end;
};

struct DungeonRoomVariantCellList {
    u8 end;
    s8 x;
    s8 y;
    u8 doorFlags;
    s16 worldY;
};

struct DungeonRoomVariant {
    Gfx * minimapDL;

    struct DungeonRoomVariantCellList * cellList;
    s8 * requiredLoot;
    Vec4f * lootLocations; // 4 = rot
    struct DungeonObject * objectList;

    ModelID16 model;
    ModelID16 doorBlockModel;
    Collision * collision;
    u8 maxLootCt:3;
    u8 starCt:2;
    u8 generateOnce:1;
    u8 easterEgg:1;
    u8 needKey:1;
    u8 safe:1;
    u8 rarity;
};

struct DungeonRoom {
    u32 neighborFlag[2];
    u8 direction;
    u8 xorigin;
    u8 yorigin;
    u8 lootCount;
    s16 worldY;
    u8 id;
    u8 challengeLv; // How many challenges to access this room
    s8 loot[4];
    struct DungeonRoomVariant * variant;
    struct Object * obj;
};

struct DungeonCell {
    u8 id;
    u8 doorFlags;
    u8 x;
    u8 y:7;
    u8 resolved:1;
    s16 worldY;
};

void dungeon_generate(int level);
void dungeon_debug_print(void);
void dungeon_set_mario_room(void);
s32 dungeon_room_is_visible(struct DungeonRoom * room);
void dungeon_print_minimap(f32 mapZoom);
void dungeon_spawn_room_red_coins(void);
void dungeon_clear_exploration_flags(void);

s32 is_level_dungeon(void);

#endif