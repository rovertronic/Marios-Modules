Vtx rmaplobby_rmaplobby_mesh_vtx_0[10] = {
	{{ {-4950, 923, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {-4950, -2923, 0}, 0, {-16, -15}, {255, 255, 255, 255} }},
	{{ {392, -2923, 0}, 0, {-16, -13}, {255, 255, 255, 255} }},
	{{ {392, -267, 0}, 0, {140, 177}, {255, 255, 255, 255} }},
	{{ {-1665, 402, 0}, 0, {-9, 764}, {255, 255, 255, 255} }},
	{{ {392, 267, 0}, 0, {105, 904}, {255, 255, 255, 255} }},
	{{ {923, 267, 0}, 0, {941, 937}, {255, 255, 255, 255} }},
	{{ {923, -267, 0}, 0, {950, 64}, {255, 255, 255, 255} }},
	{{ {392, 402, 0}, 0, {-6, 981}, {255, 255, 255, 255} }},
	{{ {-1665, 923, 0}, 0, {-15, 24}, {255, 255, 255, 255} }},
};

Gfx rmaplobby_rmaplobby_mesh_tri_0[] = {
	gsSPVertex(rmaplobby_rmaplobby_mesh_vtx_0 + 0, 10, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 0, 3, 0, 3, 5, 4, 0),
	gsSP2Triangles(3, 6, 5, 0, 3, 7, 6, 0),
	gsSP2Triangles(5, 8, 4, 0, 4, 9, 0, 0),
	gsSPEndDisplayList(),
};

Vtx rmaplobby_rmaplobby_mesh_vtx_1[20] = {
	{{ {923, 267, 0}, 0, {941, 937}, {0, 0, 127, 255} }},
	{{ {973, 317, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {442, 317, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {923, -267, 0}, 0, {950, 64}, {0, 0, 127, 255} }},
	{{ {973, -317, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {392, -267, 0}, 0, {140, 177}, {0, 0, 127, 255} }},
	{{ {442, -317, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {392, -2923, 0}, 0, {-16, -13}, {0, 0, 127, 255} }},
	{{ {442, -2973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-4950, -2923, 0}, 0, {-16, -15}, {0, 0, 127, 255} }},
	{{ {-5000, -2973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-4950, 923, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-5000, 973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-1665, 923, 0}, 0, {-15, 24}, {0, 0, 127, 255} }},
	{{ {-1615, 973, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-1665, 402, 0}, 0, {-9, 764}, {0, 0, 127, 255} }},
	{{ {-1615, 452, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {392, 402, 0}, 0, {-6, 981}, {0, 0, 127, 255} }},
	{{ {442, 452, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {392, 267, 0}, 0, {105, 904}, {0, 0, 127, 255} }},
};

Gfx rmaplobby_rmaplobby_mesh_tri_1[] = {
	gsSPVertex(rmaplobby_rmaplobby_mesh_vtx_1 + 0, 20, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(3, 4, 1, 0, 5, 4, 3, 0),
	gsSP2Triangles(5, 6, 4, 0, 7, 6, 5, 0),
	gsSP2Triangles(7, 8, 6, 0, 9, 8, 7, 0),
	gsSP2Triangles(9, 10, 8, 0, 11, 10, 9, 0),
	gsSP2Triangles(11, 12, 10, 0, 13, 12, 11, 0),
	gsSP2Triangles(13, 14, 12, 0, 15, 14, 13, 0),
	gsSP2Triangles(15, 16, 14, 0, 17, 16, 15, 0),
	gsSP2Triangles(17, 18, 16, 0, 19, 18, 17, 0),
	gsSP2Triangles(19, 2, 18, 0, 0, 2, 19, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmaplobby_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmaplobby_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmaplobby_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmaplobby_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmaplobby_rmaplobby_mesh[] = {
	gsSPDisplayList(mat_rmaplobby_map_mat_1_layer1),
	gsSPDisplayList(rmaplobby_rmaplobby_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmaplobby_map_mat_1_layer1),
	gsSPDisplayList(mat_rmaplobby_map_mat_3_layer1),
	gsSPDisplayList(rmaplobby_rmaplobby_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmaplobby_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

