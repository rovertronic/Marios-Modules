Vtx carpetPoint_carpetPoint_mesh_layer_2_vtx_0[4] = {
	{{ {-246, 0, 246}, 0, {1831, 1398}, {190, 190, 190, 255} }},
	{{ {-246, 0, -246}, 0, {1831, -406}, {190, 190, 190, 255} }},
	{{ {-973, 0, -246}, 0, {-839, -406}, {190, 190, 190, 255} }},
	{{ {-973, 0, 246}, 0, {-839, 1398}, {190, 190, 190, 255} }},
};

Gfx carpetPoint_carpetPoint_mesh_layer_2_tri_0[] = {
	gsSPVertex(carpetPoint_carpetPoint_mesh_layer_2_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};


Gfx mat_carpetPoint_carpet_decal[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, ENVIRONMENT, 0, SHADE, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, gDungeonTextureCarpet),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_carpetPoint_carpet_decal[] = {
	gsSPSetGeometryMode(G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx carpetPoint_carpetPoint_mesh_layer_2[] = {
	gsSPDisplayList(mat_carpetPoint_carpet_decal),
	gsSPDisplayList(carpetPoint_carpetPoint_mesh_layer_2_tri_0),
	gsSPDisplayList(mat_revert_carpetPoint_carpet_decal),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

