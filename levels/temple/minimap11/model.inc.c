Vtx minimap11_minimap11_mesh_vtx_0[8] = {
	{{ {94, 7775, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {94, 14318, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {-2626, 14318, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {-2626, 7775, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {1094, 9768, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {1094, 9972, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {195, 9972, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
	{{ {195, 9768, 0}, 0, {-16, -16}, {255, 255, 255, 255} }},
};

Gfx minimap11_minimap11_mesh_tri_0[] = {
	gsSPVertex(minimap11_minimap11_mesh_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Vtx minimap11_minimap11_mesh_vtx_1[16] = {
	{{ {94, 14318, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {94, 7775, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {144, 7724, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2677, 7724, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2626, 7775, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2677, 14369, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2626, 14318, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {144, 14369, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1094, 9972, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1094, 9768, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1144, 9718, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {144, 9718, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {195, 9768, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {144, 10023, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {195, 9972, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {1144, 10023, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
};

Gfx minimap11_minimap11_mesh_tri_1[] = {
	gsSPVertex(minimap11_minimap11_mesh_vtx_1 + 0, 16, 0),
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

Gfx mat_minimap11_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_LIGHTING | G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_minimap11_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_LIGHTING | G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_minimap11_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_minimap11_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx minimap11_minimap11_mesh[] = {
	gsSPDisplayList(mat_minimap11_map_mat_1_layer1),
	gsSPDisplayList(minimap11_minimap11_mesh_tri_0),
	gsSPDisplayList(mat_revert_minimap11_map_mat_1_layer1),
	gsSPDisplayList(mat_minimap11_map_mat_3_layer1),
	gsSPDisplayList(minimap11_minimap11_mesh_tri_1),
	gsSPDisplayList(mat_revert_minimap11_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

