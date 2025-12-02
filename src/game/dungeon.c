#include "engine/math_util.h"
#include "module.h"
#include "dungeon.h"
#include "game_init.h"

int sDungeonRoomCount = 0;

struct DungeonRoom sDungeonRoomList[20];
struct DungeonCell sDungeonCellGrid[32][32];

int sDungeonCellProcessCount = 0;
struct DungeonCell * sDungeonCellProcessList[32];

s8 sDirectionList[4][2] = {
    { 1, 0}, // Right
    { 0, 1}, // Down
    {-1, 0}, // Left
    { 0,-1}, // Up
};

// Room Definitions
struct DungeonRoomVariantCellList sRoomMainCellList[] = {
    {.x = 0, .y = 0},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},
    {.x = 2, .y = -1, .doorFlags = DOOR_DOWN}, // thumb for testing rotation, lol
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomMain = {
    .cellList = &sRoomMainCellList,
};

struct DungeonRoomVariantCellList sRoomHallCellList[] = {
    {.x = 0, .y = 0},
    {.x = 1, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomHall = {
    .cellList = &sRoomHallCellList,
};

struct DungeonRoomVariantCellList sRoomJunctionCellList[] = {
    {.x = 0, .y = 0},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomJunction = {
    .cellList = &sRoomJunctionCellList,
};




s32 dungeon_is_cell_occupied(int x, int y) {
    if ((x < 0)||(x >= 32)||
        (y < 0)||(y >= 32)) {
        return TRUE;
    }
    return (sDungeonCellGrid[y][x].id != 0);
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
        }

        // Copy of coordinates for self referencing
        sDungeonCellGrid[yp][xp].x = xp;
        sDungeonCellGrid[yp][xp].y = yp;

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


void dungeon_generate_rooms_at_doors(void) {
    int imax = sDungeonCellProcessCount;
    for (int i = 0; i < imax; i++) {
        if (sDungeonCellProcessList[i]->resolved) {continue;}
        sDungeonCellProcessList[i]->resolved = TRUE;

        for (int j = 0; j < 4; j++) {
            // j = dir
            if (sDungeonCellProcessList[i]->doorFlags & (1 << j)) {
                int x = sDungeonCellProcessList[i]->x + (sDirectionList[j][0]);
                int y = sDungeonCellProcessList[i]->y - (sDirectionList[j][1]);
                
                struct DungeonRoomVariant * selectedVariant = &sRoomMain;
                int select = random_u16()%3;
                if (select == 1) {
                    selectedVariant = &sRoomHall;
                }
                if (select == 2) {
                    selectedVariant = &sRoomJunction;
                }

                if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                    dungeon_create_room(selectedVariant,j,x,y);
                }
            }
        }
    }
}

void dungeon_generate(void) {
    // Clear dungeon data
    sDungeonRoomCount = 0;
    sDungeonCellProcessCount = 0;
    bzero(&sDungeonCellGrid,sizeof(sDungeonCellGrid));

    // Randomize Seed
    tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);

    // Build First Room
    dungeon_create_room(&sRoomJunction,0,8,8);

    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    //dungeon_generate_rooms_at_doors();
}

void dungeon_debug_print(void) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            if (sDungeonCellGrid[y][x].id != 0) { 
                print_text_fmt_int(x*16, y*16, "%d", sDungeonCellGrid[y][x].id);
            }
        }
    }
}