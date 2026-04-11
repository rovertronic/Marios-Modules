Vtx rmap_sdm_rmap_sdm_mesh_vtx_0[4] = {
	{{ {-2950, -2923, 0}, 0, {-3, 999}, {255, 255, 255, 255} }},
	{{ {923, -2923, 0}, 0, {995, 999}, {255, 255, 255, 255} }},
	{{ {923, 2923, 0}, 0, {995, -7}, {255, 255, 255, 255} }},
	{{ {-2950, 2923, 0}, 0, {-3, -7}, {255, 255, 255, 255} }},
};

Gfx rmap_sdm_rmap_sdm_mesh_tri_0[] = {
	gsSPVertex(rmap_sdm_rmap_sdm_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx rmap_sdm_rmap_sdm_mesh_vtx_1[8] = {
	{{ {-2950, -2923, 0}, 0, {-3, 999}, {0, 0, 127, 255} }},
	{{ {-2950, 2923, 0}, 0, {-3, -7}, {0, 0, 127, 255} }},
	{{ {-3000, 2973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {973, 2973, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {923, 2923, 0}, 0, {995, -7}, {0, 0, 127, 255} }},
	{{ {973, -2973, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {923, -2923, 0}, 0, {995, 999}, {0, 0, 127, 255} }},
	{{ {-3000, -2973, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
};

Gfx rmap_sdm_rmap_sdm_mesh_tri_1[] = {
	gsSPVertex(rmap_sdm_rmap_sdm_mesh_vtx_1 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
	gsSP2Triangles(1, 4, 3, 0, 4, 5, 3, 0),
	gsSP2Triangles(4, 6, 5, 0, 6, 7, 5, 0),
	gsSP2Triangles(6, 0, 7, 0, 0, 2, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_sdm_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_sdm_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_sdm_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_sdm_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmap_sdm_rmap_sdm_mesh[] = {
	gsSPDisplayList(mat_rmap_sdm_map_mat_1_layer1),
	gsSPDisplayList(rmap_sdm_rmap_sdm_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmap_sdm_map_mat_1_layer1),
	gsSPDisplayList(mat_rmap_sdm_map_mat_3_layer1),
	gsSPDisplayList(rmap_sdm_rmap_sdm_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmap_sdm_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

