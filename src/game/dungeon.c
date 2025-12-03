#include "engine/math_util.h"
#include "module.h"
#include "dungeon.h"
#include "game_init.h"
#include "utf8_print.h"

#define DEBUG_PRINT_MODE 1
// 0 - Room Nums
// 1 - More Compact

int sDungeonRoomCount = 0;
int sDungeonLoopCount = 0;
u32 sDungeonUniqueVariantGeneratedFlags;

struct DungeonRoom sDungeonRoomList[40];
struct DungeonCell sDungeonCellGrid[32][32];

int sDungeonCellProcessCount = 0;
struct DungeonCell * sDungeonCellProcessList[256];

s8 sDirectionList[4][2] = {
    { 1, 0}, // Right
    { 0, 1}, // Down
    {-1, 0}, // Left
    { 0,-1}, // Up
};

// Room Definitions
struct DungeonRoomVariantCellList sRoomMainCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
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
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomHallCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomHall = {
    .cellList = &sRoomHallCellList,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomLongHallCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 3, .y = 0, .doorFlags = DOOR_RIGHT},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomLongHall = {
    .cellList = &sRoomLongHallCellList,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomJunctionCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0, .doorFlags = DOOR_RIGHT},
    {.x = 1, .y = -1, .doorFlags = DOOR_DOWN},
    {.x = 1, .y = 1, .doorFlags = DOOR_UP},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomJunction = {
    .cellList = &sRoomJunctionCellList,
    .generateOnce = FALSE,
};


struct DungeonRoomVariantCellList sRoomCornerCellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_UP)},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomCorner = {
    .cellList = &sRoomCornerCellList,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomCorner2CellList[] = {
    {.x = 0, .y = 0, .doorFlags = (DOOR_LEFT | DOOR_DOWN)},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomCorner2 = {
    .cellList = &sRoomCorner2CellList,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomMiniJuncCellList[] = {
    {.x = 0, .y = 0, .doorFlags = 0xF},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomMiniJunc = {
    .cellList = &sRoomMiniJuncCellList,
    .generateOnce = FALSE,
};

struct DungeonRoomVariantCellList sRoomBigTestCellList[] = {
    {.x = 0, .y = 0, .doorFlags = DOOR_LEFT},
    {.x = 1, .y = 0},
    {.x = 2, .y = 0},
    {.x = 3, .y = 0, .doorFlags = DOOR_RIGHT},

    {.x = 0, .y = 1},
    {.x = 1, .y = 1},
    {.x = 2, .y = 1},
    {.x = 3, .y = 1},

    {.x = 1, .y = 2},
    {.x = 2, .y = 2},

    {.x = 0, .y = -1},
    {.x = 1, .y = -1},
    {.x = 2, .y = -1},
    {.x = 3, .y = -1},

    {.x = 1, .y = -2},
    {.x = 2, .y = -2},
    {.end = TRUE},
};

struct DungeonRoomVariant sRoomBigTest = {
    .cellList = &sRoomBigTestCellList,
    .generateOnce = TRUE,
};

struct DungeonRoomVariant * sRoomVariantList[] = {
    &sRoomHall,
    &sRoomCorner,
    &sRoomCorner2,
    &sRoomJunction,
    &sRoomMain,
    &sRoomMiniJunc,
    &sRoomLongHall,
    &sRoomBigTest,
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
                                sDungeonLoopCount++;
                                sDungeonCellGrid[y][x].id = 99;
                            }
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
                while(!success && trycount < 10){
                    int selectedVariantIndex = random_u16() %  (sizeof(sRoomVariantList)/4);
                    struct DungeonRoomVariant * selectedVariant = sRoomVariantList[selectedVariantIndex];

                    if (selectedVariant->generateOnce &&
                        sDungeonUniqueVariantGeneratedFlags & (1<<selectedVariantIndex)) {
                        trycount++;
                        continue;
                    }

                    if (dungeon_check_room_viability(selectedVariant,j,x,y)) {
                        sDungeonUniqueVariantGeneratedFlags |= (1<<selectedVariantIndex);

                        dungeon_create_room(selectedVariant,j,x,y);
                        success = TRUE;
                    }
                    trycount++;
                }
            }
        }
    }
}

void dungeon_generate(void) {
    redo_generate:

    // Clear dungeon data
    sDungeonRoomCount = 0;
    sDungeonLoopCount = 0;
    sDungeonUniqueVariantGeneratedFlags = 0;
    sDungeonCellProcessCount = 0;
    
    bzero(&sDungeonCellGrid,sizeof(sDungeonCellGrid));

    // Randomize Seed
    tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);

    // Build First Room
    dungeon_create_room(&sRoomJunction,0,8,8);
    //dungeon_create_room(&sRoomHall  ,0,8,8);

    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    dungeon_generate_rooms_at_doors();
    //dungeon_generate_rooms_at_doors();

    if (sDungeonLoopCount < 2) {
        goto redo_generate;
    }
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
    if (gGlobalTimer % 20 == 0) {
        dungeon_generate();
    }

    print_text_fmt_int(0, 200, "LOOP CT %d", sDungeonLoopCount);

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

                    if (sDungeonCellGrid[y][x].id == 99) {
                        print_utf8_color(".",40+x*3, 40+y*3,
                        255,
                        0,
                        0);
                    }
                #endif
            }
        }
    }
}