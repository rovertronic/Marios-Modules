Vtx minimap6_minimap6_mesh_vtx_0[8] = {
	{{ {4277, -457, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {4277, 961, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {1387, 961, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {1387, -457, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {7084, -3091, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {7084, 1061, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {4433, 1061, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {4433, -3091, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
};

Gfx minimap6_minimap6_mesh_tri_0[] = {
	gsSPVertex(minimap6_minimap6_mesh_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Vtx minimap6_minimap6_mesh_vtx_1[4] = {
	{{ {4328, 505, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4328, 105, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4382, 105, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4382, 505, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
};

Gfx minimap6_minimap6_mesh_tri_1[] = {
	gsSPVertex(minimap6_minimap6_mesh_vtx_1 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx minimap6_minimap6_mesh_vtx_2[16] = {
	{{ {1387, 961, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4277, 961, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4328, 1012, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4328, -508, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4277, -457, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1336, -508, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1387, -457, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1336, 1012, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4433, 1061, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {7084, 1061, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {7135, 1112, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {7135, -3142, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {7084, -3091, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4382, -3142, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4433, -3091, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {4382, 1112, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
};

Gfx minimap6_minimap6_mesh_tri_2[] = {
	gsSPVertex(minimap6_minimap6_mesh_vtx_2 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
	gsSP2Triangles(1, 4, 3, 0, 4, 5, 3, 0),
	gsSP2Triangles(4, 6, 5, 0, 6, 7, 5, 0),
	gsSP2Triangles(6, 0, 7, 0, 0, 2, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
	gsSP2Triangles(9, 12, 11, 0, 12, 13, 11, 0),
	gsSP2Triangles(12, 14, 13, 0, 14, 15, 13, 0),
	gsSP2Triangles(14, 8, 15, 0, 8, 10, 15, 0),
	gsSPEndDisplayList(),
};

Gfx mat_minimap6_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_LIGHTING | G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_minimap6_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_LIGHTING | G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_minimap6_map_mat_2_layer1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 0, 12, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_minimap6_map_mat_2_layer1[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_minimap6_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_minimap6_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx minimap6_minimap6_mesh[] = {
	gsSPDisplayList(mat_minimap6_map_mat_1_layer1),
	gsSPDisplayList(minimap6_minimap6_mesh_tri_0),
	gsSPDisplayList(mat_revert_minimap6_map_mat_1_layer1),
	gsSPDisplayList(mat_minimap6_map_mat_2_layer1),
	gsSPDisplayList(minimap6_minimap6_mesh_tri_1),
	gsSPDisplayList(mat_revert_minimap6_map_mat_2_layer1),
	gsSPDisplayList(mat_minimap6_map_mat_3_layer1),
	gsSPDisplayList(minimap6_minimap6_mesh_tri_2),
	gsSPDisplayList(mat_revert_minimap6_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

