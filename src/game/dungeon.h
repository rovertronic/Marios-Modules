#include "sm64.h"
#include "types.h"

#ifndef DUNGEON_H
#define DUNGEON_H

#define DOOR_RIGHT (1<<0)
#define DOOR_DOWN  (1<<1)
#define DOOR_LEFT  (1<<2)
#define DOOR_UP    (1<<3)

struct DungeonRoomVariantCellList {
    u8 end;
    s8 x;
    s8 y;
    u8 doorFlags;
};

struct DungeonRoomVariant {
    struct DungeonRoomVariantCellList * cellList;
};

struct DungeonRoom {
    u32 neighborFlag;
    u8 roomVariant;
    struct Object * roomObject; 
};

struct DungeonCell {
    u8 id;
    u8 doorFlags;
    u8 x;
    u8 y;
};

void dungeon_generate(void);
void dungeon_debug_print(void);

#endif