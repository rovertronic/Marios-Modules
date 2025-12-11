#include "types.h"

#ifndef TEXGEN_H
#define TEXGEN_H

void texgen_generate(void);

// From levels/rogue/leveldata.c
extern u16 gDungeonTextureBrick[4096];
extern u16 gDungeonTextureCarpet[2048];
extern u16 gDungeonTextureTile[2048];
extern u16 gDungeonTextureAltWall[4096];

#endif