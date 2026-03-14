Vtx rmap_maze_rmap_maze_mesh_vtx_0[4] = {
	{{ {-2950, -4950, 0}, 0, {25840, 15600}, {255, 255, 255, 255} }},
	{{ {2923, -4950, 0}, 0, {25840, -14471}, {255, 255, 255, 255} }},
	{{ {2923, 923, 0}, 0, {-4231, -14471}, {255, 255, 255, 255} }},
	{{ {-2950, 923, 0}, 0, {-4231, 15600}, {255, 255, 255, 255} }},
};

Gfx rmap_maze_rmap_maze_mesh_tri_0[] = {
	gsSPVertex(rmap_maze_rmap_maze_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx rmap_maze_rmap_maze_mesh_vtx_1[16] = {
	{{ {-2950, -4950, 0}, 0, {25840, 15600}, {0, 0, 127, 255} }},
	{{ {-3000, -5000, 0}, 0, {26096, 15856}, {0, 0, 127, 255} }},
	{{ {2973, -5000, 0}, 0, {26096, -14727}, {0, 0, 127, 255} }},
	{{ {2923, -4950, 0}, 0, {25840, -14471}, {0, 0, 127, 255} }},
	{{ {-2950, 923, 0}, 0, {-4231, 15600}, {0, 0, 127, 255} }},
	{{ {-3000, 973, 0}, 0, {-4487, 15856}, {0, 0, 127, 255} }},
	{{ {-3000, -5000, 0}, 0, {26096, 15856}, {0, 0, 127, 255} }},
	{{ {-2950, -4950, 0}, 0, {25840, 15600}, {0, 0, 127, 255} }},
	{{ {2923, 923, 0}, 0, {-4231, -14471}, {0, 0, 127, 255} }},
	{{ {2973, 973, 0}, 0, {-4487, -14727}, {0, 0, 127, 255} }},
	{{ {-3000, 973, 0}, 0, {-4487, 15856}, {0, 0, 127, 255} }},
	{{ {-2950, 923, 0}, 0, {-4231, 15600}, {0, 0, 127, 255} }},
	{{ {2923, -4950, 0}, 0, {25840, -14471}, {0, 0, 127, 255} }},
	{{ {2973, -5000, 0}, 0, {26096, -14727}, {0, 0, 127, 255} }},
	{{ {2973, 973, 0}, 0, {-4487, -14727}, {0, 0, 127, 255} }},
	{{ {2923, 923, 0}, 0, {-4231, -14471}, {0, 0, 127, 255} }},
};

Gfx rmap_maze_rmap_maze_mesh_tri_1[] = {
	gsSPVertex(rmap_maze_rmap_maze_mesh_vtx_1 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_maze_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_maze_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_maze_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_maze_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmap_maze_rmap_maze_mesh[] = {
	gsSPDisplayList(mat_rmap_maze_map_mat_1_layer1),
	gsSPDisplayList(rmap_maze_rmap_maze_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmap_maze_map_mat_1_layer1),
	gsSPDisplayList(mat_rmap_maze_map_mat_3_layer1),
	gsSPDisplayList(rmap_maze_rmap_maze_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmap_maze_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

