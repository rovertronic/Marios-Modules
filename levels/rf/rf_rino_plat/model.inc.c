Vtx rf_rino_plat_rf_rino_plat_mesh_layer_1_vtx_0[24] = {
	{{ {-404, -58, 300}, 0, {422, 1264}, {129, 0, 0, 255} }},
	{{ {-404, 58, 300}, 0, {570, 1264}, {129, 0, 0, 255} }},
	{{ {-404, 58, -300}, 0, {570, 496}, {129, 0, 0, 255} }},
	{{ {-404, -58, -300}, 0, {422, 496}, {129, 0, 0, 255} }},
	{{ {-404, -58, -300}, 0, {422, 1141}, {0, 0, 129, 255} }},
	{{ {-404, 58, -300}, 0, {570, 1141}, {0, 0, 129, 255} }},
	{{ {404, 58, -300}, 0, {570, 107}, {0, 0, 129, 255} }},
	{{ {404, -58, -300}, 0, {422, 107}, {0, 0, 129, 255} }},
	{{ {404, -58, -300}, 0, {422, 752}, {127, 0, 0, 255} }},
	{{ {404, 58, -300}, 0, {570, 752}, {127, 0, 0, 255} }},
	{{ {404, 58, 300}, 0, {570, -16}, {127, 0, 0, 255} }},
	{{ {404, -58, 300}, 0, {422, -16}, {127, 0, 0, 255} }},
	{{ {404, -58, 300}, 0, {422, 629}, {0, 0, 127, 255} }},
	{{ {404, 58, 300}, 0, {570, 629}, {0, 0, 127, 255} }},
	{{ {-404, 58, 300}, 0, {570, -405}, {0, 0, 127, 255} }},
	{{ {-404, -58, 300}, 0, {422, -405}, {0, 0, 127, 255} }},
	{{ {-404, -58, -300}, 0, {-277, 752}, {0, 129, 0, 255} }},
	{{ {404, -58, -300}, 0, {757, 752}, {0, 129, 0, 255} }},
	{{ {404, -58, 300}, 0, {757, -16}, {0, 129, 0, 255} }},
	{{ {-404, -58, 300}, 0, {-277, -16}, {0, 129, 0, 255} }},
	{{ {404, 58, -300}, 0, {235, 752}, {0, 127, 0, 255} }},
	{{ {-404, 58, -300}, 0, {1269, 752}, {0, 127, 0, 255} }},
	{{ {-404, 58, 300}, 0, {1269, -16}, {0, 127, 0, 255} }},
	{{ {404, 58, 300}, 0, {235, -16}, {0, 127, 0, 255} }},
};

Gfx rf_rino_plat_rf_rino_plat_mesh_layer_1_tri_0[] = {
	gsSPVertex(rf_rino_plat_rf_rino_plat_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};


Gfx mat_rf_rino_plat_collision_normal[] = {
	gsSPLightColor(LIGHT_1, 0xFFFFFFFF),
	gsSPLightColor(LIGHT_2, 0x7F7F7FFF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rf_rino_plat_collision_normal[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx rf_rino_plat_rf_rino_plat_mesh_layer_1_with_revert[] = {
	gsSPDisplayList(mat_rf_rino_plat_collision_normal),
	gsSPDisplayList(rf_rino_plat_rf_rino_plat_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_rf_rino_plat_collision_normal),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

