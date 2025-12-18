Vtx rmapdoor_rmapdoor_mesh_vtx_0[4] = {
	{{ {-27, 200, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-27, -200, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {27, -200, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {27, 200, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
};

Gfx rmapdoor_rmapdoor_mesh_tri_0[] = {
	gsSPVertex(rmapdoor_rmapdoor_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmapdoor_map_mat_2_layer1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 0, 12, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapdoor_map_mat_2_layer1[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmapdoor_rmapdoor_mesh[] = {
	gsSPDisplayList(mat_rmapdoor_map_mat_2_layer1),
	gsSPDisplayList(rmapdoor_rmapdoor_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmapdoor_map_mat_2_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

