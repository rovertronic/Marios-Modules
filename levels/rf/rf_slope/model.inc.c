Vtx rf_slope_rf_slope_mesh_layer_1_vtx_0[24] = {
	{{ {-4900, -71, 330}, 0, {405, 1303}, {129, 0, 0, 255} }},
	{{ {-4900, 0, 260}, 0, {496, 1212}, {129, 0, 0, 255} }},
	{{ {-4900, -590, -330}, 0, {-259, 457}, {129, 0, 0, 255} }},
	{{ {-4900, -661, -260}, 0, {-350, 548}, {129, 0, 0, 255} }},
	{{ {-4900, -661, -260}, 0, {-138, 6896}, {0, 166, 166, 255} }},
	{{ {-4900, -590, -330}, 0, {-48, 6896}, {0, 166, 166, 255} }},
	{{ {900, -590, -330}, 0, {-48, -528}, {0, 166, 166, 255} }},
	{{ {900, -661, -260}, 0, {-138, -528}, {0, 166, 166, 255} }},
	{{ {900, -661, -260}, 0, {-350, 700}, {127, 0, 0, 255} }},
	{{ {900, -590, -330}, 0, {-259, 791}, {127, 0, 0, 255} }},
	{{ {900, 0, 260}, 0, {496, 36}, {127, 0, 0, 255} }},
	{{ {900, -71, 330}, 0, {405, -55}, {127, 0, 0, 255} }},
	{{ {900, -71, 330}, 0, {617, 1264}, {0, 90, 90, 255} }},
	{{ {900, 0, 260}, 0, {707, 1264}, {0, 90, 90, 255} }},
	{{ {-4900, 0, 260}, 0, {707, -6160}, {0, 90, 90, 255} }},
	{{ {-4900, -71, 330}, 0, {617, -6160}, {0, 90, 90, 255} }},
	{{ {-4900, -661, -260}, 0, {-6032, 912}, {0, 166, 90, 255} }},
	{{ {900, -661, -260}, 0, {1392, 912}, {0, 166, 90, 255} }},
	{{ {900, -71, 330}, 0, {1392, 157}, {0, 166, 90, 255} }},
	{{ {-4900, -71, 330}, 0, {-6032, 157}, {0, 166, 90, 255} }},
	{{ {900, -590, -330}, 0, {-400, 1002}, {0, 90, 166, 255} }},
	{{ {-4900, -590, -330}, 0, {7024, 1002}, {0, 90, 166, 255} }},
	{{ {-4900, 0, 260}, 0, {7024, 247}, {0, 90, 166, 255} }},
	{{ {900, 0, 260}, 0, {-400, 247}, {0, 90, 166, 255} }},
};

Gfx rf_slope_rf_slope_mesh_layer_1_tri_0[] = {
	gsSPVertex(rf_slope_rf_slope_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};


Gfx mat_rf_slope_collision_slippery[] = {
	gsSPLightColor(LIGHT_1, 0xFF00E6FF),
	gsSPLightColor(LIGHT_2, 0x7F0072FF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rf_slope_collision_slippery[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx rf_slope_rf_slope_mesh_layer_1_with_revert[] = {
	gsSPDisplayList(mat_rf_slope_collision_slippery),
	gsSPDisplayList(rf_slope_rf_slope_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_rf_slope_collision_slippery),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

