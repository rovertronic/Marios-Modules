Vtx rf_squarish_obj_rf_squarish_obj_mesh_layer_1_vtx_cull[8] = {
	{{ {-500, -264, 500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-500, 178, 500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-500, 178, -500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-500, -264, -500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {500, -264, 500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {500, 178, 500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {500, 178, -500}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {500, -264, -500}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx rf_squarish_obj_rf_squarish_obj_mesh_layer_1_vtx_0[24] = {
	{{ {0, 178, 0}, 0, {244, 244}, {213, 120, 0, 255} }},
	{{ {-500, 0, -500}, 0, {-16, -16}, {213, 120, 0, 255} }},
	{{ {-500, 0, 500}, 0, {-16, 1008}, {213, 120, 0, 255} }},
	{{ {0, 178, 0}, 0, {244, 748}, {0, 120, 43, 255} }},
	{{ {-500, 0, 500}, 0, {-16, 1008}, {0, 120, 43, 255} }},
	{{ {500, 0, 500}, 0, {1008, 1008}, {0, 120, 43, 255} }},
	{{ {0, 178, 0}, 0, {748, 748}, {43, 120, 0, 255} }},
	{{ {500, 0, 500}, 0, {1008, 1008}, {43, 120, 0, 255} }},
	{{ {500, 0, -500}, 0, {1008, -16}, {43, 120, 0, 255} }},
	{{ {0, 178, 0}, 0, {748, 244}, {0, 120, 213, 255} }},
	{{ {500, 0, -500}, 0, {1008, -16}, {0, 120, 213, 255} }},
	{{ {-500, 0, -500}, 0, {-16, -16}, {0, 120, 213, 255} }},
	{{ {0, -264, 0}, 0, {244, 244}, {197, 144, 0, 255} }},
	{{ {-500, 0, 500}, 0, {-16, 1008}, {197, 144, 0, 255} }},
	{{ {-500, 0, -500}, 0, {-16, -16}, {197, 144, 0, 255} }},
	{{ {0, -264, 0}, 0, {244, 748}, {0, 144, 59, 255} }},
	{{ {500, 0, 500}, 0, {1008, 1008}, {0, 144, 59, 255} }},
	{{ {-500, 0, 500}, 0, {-16, 1008}, {0, 144, 59, 255} }},
	{{ {0, -264, 0}, 0, {748, 748}, {59, 144, 0, 255} }},
	{{ {500, 0, -500}, 0, {1008, -16}, {59, 144, 0, 255} }},
	{{ {500, 0, 500}, 0, {1008, 1008}, {59, 144, 0, 255} }},
	{{ {0, -264, 0}, 0, {748, 244}, {0, 144, 197, 255} }},
	{{ {-500, 0, -500}, 0, {-16, -16}, {0, 144, 197, 255} }},
	{{ {500, 0, -500}, 0, {1008, -16}, {0, 144, 197, 255} }},
};

Gfx rf_squarish_obj_rf_squarish_obj_mesh_layer_1_tri_0[] = {
	gsSPVertex(rf_squarish_obj_rf_squarish_obj_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
	gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
	gsSPEndDisplayList(),
};


Gfx mat_rf_squarish_obj_collision_normal[] = {
	gsSPLightColor(LIGHT_1, 0xFFFFFFFF),
	gsSPLightColor(LIGHT_2, 0x7F7F7FFF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rf_squarish_obj_collision_normal[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx rf_squarish_obj_rf_squarish_obj_mesh_layer_1[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(rf_squarish_obj_rf_squarish_obj_mesh_layer_1_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_rf_squarish_obj_collision_normal),
	gsSPDisplayList(rf_squarish_obj_rf_squarish_obj_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_rf_squarish_obj_collision_normal),
	gsSPEndDisplayList(),
};

Gfx rf_squarish_obj_final_revert_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

