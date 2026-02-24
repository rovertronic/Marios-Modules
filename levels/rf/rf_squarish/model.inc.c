Vtx rf_squarish_rf_squarish_mesh_layer_1_vtx_0[16] = {
	{{ {-912, -50, 912}, 0, {29, 963}, {62, 111, 0, 255} }},
	{{ {-912, -50, -912}, 0, {29, 29}, {62, 111, 0, 255} }},
	{{ {-1000, 0, -1000}, 0, {-16, -16}, {62, 111, 0, 255} }},
	{{ {-1000, 0, 1000}, 0, {-16, 1008}, {62, 111, 0, 255} }},
	{{ {912, -50, 912}, 0, {963, 963}, {0, 111, 194, 255} }},
	{{ {-912, -50, 912}, 0, {29, 963}, {0, 111, 194, 255} }},
	{{ {-1000, 0, 1000}, 0, {-16, 1008}, {0, 111, 194, 255} }},
	{{ {1000, 0, 1000}, 0, {1008, 1008}, {0, 111, 194, 255} }},
	{{ {912, -50, -912}, 0, {963, 29}, {194, 111, 0, 255} }},
	{{ {912, -50, 912}, 0, {963, 963}, {194, 111, 0, 255} }},
	{{ {1000, 0, 1000}, 0, {1008, 1008}, {194, 111, 0, 255} }},
	{{ {1000, 0, -1000}, 0, {1008, -16}, {194, 111, 0, 255} }},
	{{ {-912, -50, -912}, 0, {29, 29}, {0, 111, 62, 255} }},
	{{ {912, -50, -912}, 0, {963, 29}, {0, 111, 62, 255} }},
	{{ {1000, 0, -1000}, 0, {1008, -16}, {0, 111, 62, 255} }},
	{{ {-1000, 0, -1000}, 0, {-16, -16}, {0, 111, 62, 255} }},
};

Gfx rf_squarish_rf_squarish_mesh_layer_1_tri_0[] = {
	gsSPVertex(rf_squarish_rf_squarish_mesh_layer_1_vtx_0 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSPEndDisplayList(),
};


Gfx mat_rf_squarish_collision_normal[] = {
	gsSPLightColor(LIGHT_1, 0xFFFFFFFF),
	gsSPLightColor(LIGHT_2, 0x7F7F7FFF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rf_squarish_collision_normal[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx rf_squarish_rf_squarish_mesh_layer_1_with_revert[] = {
	gsSPDisplayList(mat_rf_squarish_collision_normal),
	gsSPDisplayList(rf_squarish_rf_squarish_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_rf_squarish_collision_normal),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

