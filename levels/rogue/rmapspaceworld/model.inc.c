Vtx rmapspaceworld_rmapspaceworld_mesh_vtx_0[8] = {
	{{ {-789, 2923, 0}, 0, {-16, 999}, {255, 255, 255, 255} }},
	{{ {923, -1211, 0}, 0, {979, 3}, {255, 255, 255, 255} }},
	{{ {923, 1211, 0}, 0, {979, 989}, {255, 255, 255, 255} }},
	{{ {-789, -2923, 0}, 0, {-16, -7}, {255, 255, 255, 255} }},
	{{ {-3211, -2923, 0}, 0, {-16, -5}, {255, 255, 255, 255} }},
	{{ {-3211, 2923, 0}, 0, {-16, 997}, {255, 255, 255, 255} }},
	{{ {-4923, -1211, 0}, 0, {-16, 3}, {255, 255, 255, 255} }},
	{{ {-4923, 1211, 0}, 0, {-16, 989}, {255, 255, 255, 255} }},
};

Gfx rmapspaceworld_rmapspaceworld_mesh_tri_0[] = {
	gsSPVertex(rmapspaceworld_rmapspaceworld_mesh_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsSP2Triangles(0, 4, 3, 0, 0, 5, 4, 0),
	gsSP2Triangles(5, 6, 4, 0, 5, 7, 6, 0),
	gsSPEndDisplayList(),
};

Vtx rmapspaceworld_rmapspaceworld_mesh_vtx_1[16] = {
	{{ {923, 1211, 0}, 0, {979, 989}, {0, 0, 127, 255} }},
	{{ {973, 1231, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {-769, 2973, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {923, -1211, 0}, 0, {979, 3}, {0, 0, 127, 255} }},
	{{ {973, -1231, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-789, -2923, 0}, 0, {-16, -7}, {0, 0, 127, 255} }},
	{{ {-769, -2973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3211, -2923, 0}, 0, {-16, -5}, {0, 0, 127, 255} }},
	{{ {-3231, -2973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-4923, -1211, 0}, 0, {-16, 3}, {0, 0, 127, 255} }},
	{{ {-4973, -1231, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-4923, 1211, 0}, 0, {-16, 989}, {0, 0, 127, 255} }},
	{{ {-4973, 1231, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-3211, 2923, 0}, 0, {-16, 997}, {0, 0, 127, 255} }},
	{{ {-3231, 2973, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-789, 2923, 0}, 0, {-16, 999}, {0, 0, 127, 255} }},
};

Gfx rmapspaceworld_rmapspaceworld_mesh_tri_1[] = {
	gsSPVertex(rmapspaceworld_rmapspaceworld_mesh_vtx_1 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(3, 4, 1, 0, 5, 4, 3, 0),
	gsSP2Triangles(5, 6, 4, 0, 7, 6, 5, 0),
	gsSP2Triangles(7, 8, 6, 0, 9, 8, 7, 0),
	gsSP2Triangles(9, 10, 8, 0, 11, 10, 9, 0),
	gsSP2Triangles(11, 12, 10, 0, 13, 12, 11, 0),
	gsSP2Triangles(13, 14, 12, 0, 15, 14, 13, 0),
	gsSP2Triangles(15, 2, 14, 0, 0, 2, 15, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmapspaceworld_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapspaceworld_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmapspaceworld_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmapspaceworld_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmapspaceworld_rmapspaceworld_mesh[] = {
	gsSPDisplayList(mat_rmapspaceworld_map_mat_1_layer1),
	gsSPDisplayList(rmapspaceworld_rmapspaceworld_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmapspaceworld_map_mat_1_layer1),
	gsSPDisplayList(mat_rmapspaceworld_map_mat_3_layer1),
	gsSPDisplayList(rmapspaceworld_rmapspaceworld_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmapspaceworld_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

