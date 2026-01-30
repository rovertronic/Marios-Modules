Vtx rmappush_rmappush_mesh_vtx_0[11] = {
	{{ {-2180, 3797, 0}, 0, {-16, 996}, {255, 255, 255, 255} }},
	{{ {-2230, -298, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {923, -298, 0}, 0, {992, -4}, {255, 255, 255, 255} }},
	{{ {923, 3797, 0}, 0, {992, 996}, {255, 255, 255, 255} }},
	{{ {-3257, -298, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {-2230, -298, 0}, 0, {-16, -4}, {255, 255, 255, 255} }},
	{{ {-3307, 3797, 0}, 0, {-16, 996}, {255, 255, 255, 255} }},
	{{ {-3507, -298, 0}, 0, {-16, -4}, {255, 255, 255, 255} }},
	{{ {-3507, 3797, 0}, 0, {-16, 996}, {255, 255, 255, 255} }},
	{{ {-3257, -926, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {-2230, -926, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
};

Gfx rmappush_rmappush_mesh_tri_0[] = {
	gsSPVertex(rmappush_rmappush_mesh_vtx_0 + 0, 11, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(0, 4, 5, 0, 0, 6, 4, 0),
	gsSP2Triangles(6, 7, 4, 0, 6, 8, 7, 0),
	gsSP2Triangles(4, 9, 10, 0, 4, 10, 1, 0),
	gsSPEndDisplayList(),
};

Vtx rmappush_rmappush_mesh_vtx_1[20] = {
	{{ {923, 3797, 0}, 0, {992, 996}, {0, 0, 127, 255} }},
	{{ {973, 3847, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {-2180, 3847, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {923, -298, 0}, 0, {992, -4}, {0, 0, 127, 255} }},
	{{ {973, -348, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-2230, -298, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2180, -348, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2230, -926, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2180, -976, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3257, -926, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3307, -976, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3257, -298, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3307, -348, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3507, -298, 0}, 0, {-16, -4}, {0, 0, 127, 255} }},
	{{ {-3557, -348, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-3507, 3797, 0}, 0, {-16, 996}, {0, 0, 127, 255} }},
	{{ {-3557, 3847, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-3307, 3797, 0}, 0, {-16, 996}, {0, 0, 127, 255} }},
	{{ {-3307, 3847, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-2180, 3797, 0}, 0, {-16, 996}, {0, 0, 127, 255} }},
};

Gfx rmappush_rmappush_mesh_tri_1[] = {
	gsSPVertex(rmappush_rmappush_mesh_vtx_1 + 0, 20, 0),
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

Gfx mat_rmappush_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_ZBUFFER | G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmappush_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_ZBUFFER | G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmappush_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmappush_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmappush_rmappush_mesh[] = {
	gsSPDisplayList(mat_rmappush_map_mat_1_layer1),
	gsSPDisplayList(rmappush_rmappush_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmappush_map_mat_1_layer1),
	gsSPDisplayList(mat_rmappush_map_mat_3_layer1),
	gsSPDisplayList(rmappush_rmappush_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmappush_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

