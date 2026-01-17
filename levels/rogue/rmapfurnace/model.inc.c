Vtx rmapfurnace_rmapfurnace_mesh_vtx_0[4] = {
	{{ {-4923, 442, 0}, 0, {-7, 972}, {255, 255, 255, 255} }},
	{{ {-4923, -873, 0}, 0, {-7, 20}, {255, 255, 255, 255} }},
	{{ {923, -873, 0}, 0, {999, 20}, {255, 255, 255, 255} }},
	{{ {923, 442, 0}, 0, {999, 972}, {255, 255, 255, 255} }},
};

Gfx rmapfurnace_rmapfurnace_mesh_tri_0[] = {
	gsSPVertex(rmapfurnace_rmapfurnace_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx rmapfurnace_rmapfurnace_mesh_vtx_1[8] = {
	{{ {-4923, 442, 0}, 0, {-7, 972}, {0, 0, 127, 255} }},
	{{ {-4973, 492, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-4973, -923, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {923, 442, 0}, 0, {999, 972}, {0, 0, 127, 255} }},
	{{ {973, 492, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {923, -873, 0}, 0, {999, 20}, {0, 0, 127, 255} }},
	{{ {973, -923, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-4923, -873, 0}, 0, {-7, 20}, {0, 0, 127, 255} }},
};

Gfx rmapfurnace_rmapfurnace_mesh_tri_1[] = {
	gsSPVertex(rmapfurnace_rmapfurnace_mesh_vtx_1 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(3, 4, 1, 0, 5, 4, 3, 0),
	gsSP2Triangles(5, 6, 4, 0, 7, 6, 5, 0),
	gsSP2Triangles(7, 2, 6, 0, 0, 2, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmapfurnace_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapfurnace_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmapfurnace_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapfurnace_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmapfurnace_rmapfurnace_mesh[] = {
	gsSPDisplayList(mat_rmapfurnace_map_mat_1_layer1),
	gsSPDisplayList(rmapfurnace_rmapfurnace_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmapfurnace_map_mat_1_layer1),
	gsSPDisplayList(mat_rmapfurnace_map_mat_3_layer1),
	gsSPDisplayList(rmapfurnace_rmapfurnace_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmapfurnace_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

