Vtx rmapvanishhop_rmapvanishhop_mesh_vtx_0[4] = {
	{{ {-4923, -2650, 0}, 0, {-7, 999}, {255, 255, 255, 255} }},
	{{ {923, -2650, 0}, 0, {999, 999}, {255, 255, 255, 255} }},
	{{ {923, 2650, 0}, 0, {999, -7}, {255, 255, 255, 255} }},
	{{ {-4923, 2650, 0}, 0, {-7, -7}, {255, 255, 255, 255} }},
};

Gfx rmapvanishhop_rmapvanishhop_mesh_tri_0[] = {
	gsSPVertex(rmapvanishhop_rmapvanishhop_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx rmapvanishhop_rmapvanishhop_mesh_vtx_1[8] = {
	{{ {-4923, -2650, 0}, 0, {-7, 999}, {0, 0, 127, 255} }},
	{{ {-4923, 2650, 0}, 0, {-7, -7}, {0, 0, 127, 255} }},
	{{ {-4973, 2700, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {973, 2700, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {923, 2650, 0}, 0, {999, -7}, {0, 0, 127, 255} }},
	{{ {973, -2700, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {923, -2650, 0}, 0, {999, 999}, {0, 0, 127, 255} }},
	{{ {-4973, -2700, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
};

Gfx rmapvanishhop_rmapvanishhop_mesh_tri_1[] = {
	gsSPVertex(rmapvanishhop_rmapvanishhop_mesh_vtx_1 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
	gsSP2Triangles(1, 4, 3, 0, 4, 5, 3, 0),
	gsSP2Triangles(4, 6, 5, 0, 6, 7, 5, 0),
	gsSP2Triangles(6, 0, 7, 0, 0, 2, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmapvanishhop_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_LIGHTING | G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapvanishhop_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_LIGHTING | G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmapvanishhop_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapvanishhop_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmapvanishhop_rmapvanishhop_mesh[] = {
	gsSPDisplayList(mat_rmapvanishhop_map_mat_1_layer1),
	gsSPDisplayList(rmapvanishhop_rmapvanishhop_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmapvanishhop_map_mat_1_layer1),
	gsSPDisplayList(mat_rmapvanishhop_map_mat_3_layer1),
	gsSPDisplayList(rmapvanishhop_rmapvanishhop_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmapvanishhop_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

