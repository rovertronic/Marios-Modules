#include "texgen.h"
#include "engine/math_util.h"
#include "levels/rogue/header.h"

u32 sColorPalletepixelFloats[][3] = {
    {0xFFE74C,0xFF5964,0xFFFFFF},
    {0x5C0029,0x61304B,0x857C8D},
    {0x023C40,0xC3979F,0x0AD3FF},
    {0xC1B098,0xE9D2F4,0x9B9B93},
    {0xAEFFD8,0x8AFFC1,0x8FDC97},
    {0xC9FBFF,0xC2FCF7,0x85BDBF},
    {0x054A91,0x3E7CB1,0x81A4CD},
    {0xDF2935,0x86BA90,0xF5F3BB},
    {0x65524D,0x817E9F,0x7FC29B},
    {0xEE6055,0x60D394,0xAAF683},
    {0x6290C3,0xC2E7DA,0xF1FFE7},
    {0x656565,0x808782,0xA6D3A0},
    {0x2274A5,0xF75C03,0xF1C40F},
    {0x845A6D,0x3E1929,0x6E75A8},
    {0x8F9491,0xBCA3AC,0xE5CEDC},
    {0x5F634F,0x9BC4CB,0xCFEBDF},
    {0x605B56,0x837A75,0xACC18A},
    {0x93827F,0xF3F9D2,0xBDC4A7},
    {0xAFD2E9,0x9D96B8,0x9A7197},
    {0xB5D6B2,0x53131E,0x5A464C},
};

u16 * sBrickTextures[] = {
    &texgensamples_Material27_rgba16,
    &texgensamples_SmoothBrickWhite_rgba16,
    &texgensamples_VarietyBrick_rgba16,
};

u16 * sTileTextures[] = {
    &texgensamples_itimatu2_rgba16,
    &texgensamples_tile2_rgba16,
};

u16 * sAltWallTextures[] = {
    &texgensamples_pipes_rgba16,
    &texgensamples_nightsky_rgba16,
    &texgensamples_stucco_rgba16,
};

void rgba5551_to_f32(u16 pixelBits, f32 *pixelFloat)
{
    u16 R = (pixelBits >> 11) & 0x1F;   // 5 bits
    u16 G = (pixelBits >> 6)  & 0x1F;   // 5 bits
    u16 B = (pixelBits >> 1)  & 0x1F;   // 5 bits
    u16 A =  pixelBits        & 0x01;   // 1 bit

    pixelFloat[0] = (f32)R / 31.0f;
    pixelFloat[1] = (f32)G / 31.0f;
    pixelFloat[2] = (f32)B / 31.0f;
    pixelFloat[3] = (f32)A;
}

void rgba888_to_f32(u32 pixelBits, f32 *pixelFloat)
{
    // No alpha
    u32 R = (pixelBits >> 16) & 0xFF;
    u32 G = (pixelBits >> 8) & 0xFF;
    u32 B = (pixelBits) & 0xFF;

    pixelFloat[0] = (f32)R / 255.0f;
    pixelFloat[1] = (f32)G / 255.0f;
    pixelFloat[2] = (f32)B / 255.0f;
    pixelFloat[3] = 1.0f;
}

u16 f32_to_rgba5551(f32 * pixelFloat) {
    f32 r = pixelFloat[0];
    f32 g = pixelFloat[1];
    f32 b = pixelFloat[2];
    f32 a = pixelFloat[3];

    /* Clamp */
    if (r < 0.0f) r = 0.0f; if (r > 1.0f) r = 1.0f;
    if (g < 0.0f) g = 0.0f; if (g > 1.0f) g = 1.0f;
    if (b < 0.0f) b = 0.0f; if (b > 1.0f) b = 1.0f;
    if (a < 0.0f) a = 0.0f; if (a > 1.0f) a = 1.0f;

    u16 R = (u16)(r * 31.0f + 0.5f) & 0x1F;
    u16 G = (u16)(g * 31.0f + 0.5f) & 0x1F;
    u16 B = (u16)(b * 31.0f + 0.5f) & 0x1F;
    u16 A = (u16)(a >= 0.5f);  // threshold for 1-bit alpha

    return (R << 11) | (G << 6) | (B << 1) | A;
}

void texgen_colorize_rgba16(u16 * texture, u16 * texout, int size, u32 color) {
    int pixelCount = size/2;
    for (int i = 0; i < pixelCount; i++) {
        f32 colorFloat[4];
        f32 texFloat[4];
        f32 newColorFloat[4];

        rgba5551_to_f32(texture[i],&colorFloat);
        rgba888_to_f32(color,&texFloat);

        for (int j = 0; j < 4; j++) {
            newColorFloat[j] = colorFloat[j] * texFloat[j];
        }

        texout[i] = f32_to_rgba5551(&newColorFloat);
    }
}

void texgen_generate(void) {
    int colorPalletIndex = tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sColorPalletepixelFloats)/12);
    int brickTextureIndex = tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sBrickTextures)/4);
    int tileTextureIndex = tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sTileTextures)/4);
    int altWallTextureIndex = tinymt32_generate_u32(&gGlobalRandomState)%(sizeof(sAltWallTextures)/4);

    u16 * texture;

    // Generate Brick Texture
    texture = segmented_to_virtual(sBrickTextures[brickTextureIndex]);
    texgen_colorize_rgba16( texture, segmented_to_virtual(&gDungeonTextureBrick), 4096, sColorPalletepixelFloats[colorPalletIndex][1]  );

    // Generate Carpet Texture
    texture = segmented_to_virtual(&texgensamples_CarpetGrayscale_rgba16);
    texgen_colorize_rgba16( texture, segmented_to_virtual(&gDungeonTextureCarpet), 2048, sColorPalletepixelFloats[colorPalletIndex][0]  );

    // Generate Tile Texture
    u32 tileColor = 0xFFFFFF;
    if (tinymt32_generate_u32(&gGlobalRandomState)%2==0) {
        tileColor = sColorPalletepixelFloats[colorPalletIndex][2];
    }

    texture = segmented_to_virtual(sTileTextures[tileTextureIndex]);
    texgen_colorize_rgba16( texture, segmented_to_virtual(&gDungeonTextureTile), 2048, tileColor  );

    // Generate Alt Wall Texture

    texture = segmented_to_virtual(sAltWallTextures[tileTextureIndex]);
    texgen_colorize_rgba16( texture, segmented_to_virtual(&gDungeonTextureAltWall), 4096, 0xFFFFFF  );

    // Generate Shingles Texture

    texture = segmented_to_virtual(&texgensamples_shingles_rgba16);
    texgen_colorize_rgba16( texture, segmented_to_virtual(&gDungeonTextureShingles), 2048, sColorPalletepixelFloats[colorPalletIndex][2]  );
}