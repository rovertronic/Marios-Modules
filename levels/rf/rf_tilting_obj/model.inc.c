Vtx rf_tilting_obj_rf_tilting_obj_mesh_layer_1_vtx_0[24] = {
	{{ {264, 100, 796}, 0, {368, 1008}, {0, 0, 127, 255} }},
	{{ {-264, 100, 796}, 0, {368, 752}, {0, 0, 127, 255} }},
	{{ {-264, 0, 796}, 0, {624, 752}, {0, 0, 127, 255} }},
	{{ {264, 0, 796}, 0, {624, 1008}, {0, 0, 127, 255} }},
	{{ {-264, 100, 796}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {-264, 100, -796}, 0, {368, 496}, {129, 0, 0, 255} }},
	{{ {-264, 0, -796}, 0, {624, 496}, {129, 0, 0, 255} }},
	{{ {-264, 0, 796}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-264, 100, -796}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {264, 100, -796}, 0, {368, 240}, {0, 0, 129, 255} }},
	{{ {264, 0, -796}, 0, {624, 240}, {0, 0, 129, 255} }},
	{{ {-264, 0, -796}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {264, 100, -796}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {264, 100, 796}, 0, {368, -16}, {127, 0, 0, 255} }},
	{{ {264, 0, 796}, 0, {624, -16}, {127, 0, 0, 255} }},
	{{ {264, 0, -796}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {-264, 100, 796}, 0, {112, 496}, {0, 127, 0, 255} }},
	{{ {264, 100, 796}, 0, {112, 240}, {0, 127, 0, 255} }},
	{{ {264, 100, -796}, 0, {368, 240}, {0, 127, 0, 255} }},
	{{ {-264, 100, -796}, 0, {368, 496}, {0, 127, 0, 255} }},
	{{ {-264, 0, -796}, 0, {624, 496}, {0, 129, 0, 255} }},
	{{ {264, 0, -796}, 0, {624, 240}, {0, 129, 0, 255} }},
	{{ {264, 0, 796}, 0, {880, 240}, {0, 129, 0, 255} }},
	{{ {-264, 0, 796}, 0, {880, 496}, {0, 129, 0, 255} }},
};

Gfx rf_tilting_obj_rf_tilting_obj_mesh_layer_1_tri_0[] = {
	gsSPVertex(rf_tilting_obj_rf_tilting_obj_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};


Gfx mat_rf_tilting_obj_wood[] = {
	gsSPLightColor(LIGHT_1, 0x8E4D1BFF),
	gsSPLightColor(LIGHT_2, 0x442208FF),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rf_tilting_obj_wood[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx rf_tilting_obj_rf_tilting_obj_mesh_layer_1_with_revert[] = {
	gsSPDisplayList(mat_rf_tilting_obj_wood),
	gsSPDisplayList(rf_tilting_obj_rf_tilting_obj_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_rf_tilting_obj_wood),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

