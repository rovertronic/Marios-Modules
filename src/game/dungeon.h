#include "sm64.h"
#include "types.h"

#ifndef DUNGEON_H
#define DUNGEON_H

extern struct DungeonRoom * gDungeonMarioRoom;

#define DOOR_RIGHT (1<<0)
#define DOOR_DOWN  (1<<1)
#define DOOR_LEFT  (1<<2)
#define DOOR_UP    (1<<3)

struct DungeonObject {
    BehaviorScript bhv;
    ModelID16 model;
    s16 angle;
    u8 param;
    Vec3f pos;
    u8 end;
};

struct DungeonRoomVariantCellList {
    u8 end;
    s8 x;
    s8 y;
    u8 doorFlags;
};

struct DungeonRoomVariant {
    struct DungeonRoomVariantCellList * cellList;
    s8 * requiredLoot;
    Vec4f * lootLocations; // 4 = rot
    struct DungeonObject * objectList;

    ModelID16 model;
    Collision * collision;
    u8 maxLootCt:2;
    u8 generateOnce:1;
};

struct DungeonRoom {
    u32 neighborFlag[2];
    u8 direction;
    u8 xorigin;
    u8 yorigin;
    u8 lootCount;
    u8 id;
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
};

void dungeon_generate(void);
void dungeon_debug_print(void);
void dungeon_set_mario_room(void);
s32 dungeon_room_check_neighbor_flag(struct DungeonRoom * room, int id);

#endif