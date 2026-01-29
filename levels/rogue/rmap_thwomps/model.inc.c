Vtx rmap_thwomps_rmap_thwomps_mesh_vtx_0[8] = {
	{{ {923, -2618, 0}, 0, {5223, 13901}, {255, 255, 255, 255} }},
	{{ {-4618, -2923, 0}, 0, {-23149, 15463}, {255, 255, 255, 255} }},
	{{ {618, -2923, 0}, 0, {3661, 15463}, {255, 255, 255, 255} }},
	{{ {-4923, -2618, 0}, 0, {-24711, 13901}, {255, 255, 255, 255} }},
	{{ {923, 2618, 0}, 0, {5223, -12909}, {255, 255, 255, 255} }},
	{{ {-4923, 2618, 0}, 0, {-24711, -12909}, {255, 255, 255, 255} }},
	{{ {618, 2923, 0}, 0, {3661, -14471}, {255, 255, 255, 255} }},
	{{ {-4618, 2923, 0}, 0, {-23149, -14471}, {255, 255, 255, 255} }},
};

Gfx rmap_thwomps_rmap_thwomps_mesh_tri_0[] = {
	gsSPVertex(rmap_thwomps_rmap_thwomps_mesh_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsSP2Triangles(3, 0, 4, 0, 3, 4, 5, 0),
	gsSP2Triangles(5, 4, 6, 0, 5, 6, 7, 0),
	gsSPEndDisplayList(),
};

Vtx rmap_thwomps_rmap_thwomps_mesh_vtx_1[16] = {
	{{ {-4923, -2618, 0}, 0, {-24711, 13901}, {0, 0, 127, 255} }},
	{{ {-4923, 2618, 0}, 0, {-24711, -12909}, {0, 0, 127, 255} }},
	{{ {-4973, 2639, 0}, 0, {-24967, -13015}, {0, 0, 127, 255} }},
	{{ {-4639, 2973, 0}, 0, {-23255, -14727}, {0, 0, 127, 255} }},
	{{ {-4618, 2923, 0}, 0, {-23149, -14471}, {0, 0, 127, 255} }},
	{{ {639, 2973, 0}, 0, {3767, -14727}, {0, 0, 127, 255} }},
	{{ {618, 2923, 0}, 0, {3661, -14471}, {0, 0, 127, 255} }},
	{{ {973, 2639, 0}, 0, {5479, -13015}, {0, 0, 127, 255} }},
	{{ {923, 2618, 0}, 0, {5223, -12909}, {0, 0, 127, 255} }},
	{{ {973, -2639, 0}, 0, {5479, 14007}, {0, 0, 127, 255} }},
	{{ {923, -2618, 0}, 0, {5223, 13901}, {0, 0, 127, 255} }},
	{{ {639, -2973, 0}, 0, {3767, 15719}, {0, 0, 127, 255} }},
	{{ {618, -2923, 0}, 0, {3661, 15463}, {0, 0, 127, 255} }},
	{{ {-4639, -2973, 0}, 0, {-23255, 15719}, {0, 0, 127, 255} }},
	{{ {-4618, -2923, 0}, 0, {-23149, 15463}, {0, 0, 127, 255} }},
	{{ {-4973, -2639, 0}, 0, {-24967, 14007}, {0, 0, 127, 255} }},
};

Gfx rmap_thwomps_rmap_thwomps_mesh_tri_1[] = {
	gsSPVertex(rmap_thwomps_rmap_thwomps_mesh_vtx_1 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
	gsSP2Triangles(1, 4, 3, 0, 4, 5, 3, 0),
	gsSP2Triangles(4, 6, 5, 0, 6, 7, 5, 0),
	gsSP2Triangles(6, 8, 7, 0, 8, 9, 7, 0),
	gsSP2Triangles(8, 10, 9, 0, 10, 11, 9, 0),
	gsSP2Triangles(10, 12, 11, 0, 12, 13, 11, 0),
	gsSP2Triangles(12, 14, 13, 0, 14, 15, 13, 0),
	gsSP2Triangles(14, 0, 15, 0, 0, 2, 15, 0),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_thwomps_map_mat_1_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_LIGHTING | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 67, 149, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_thwomps_map_mat_1_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_LIGHTING | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx mat_rmap_thwomps_map_mat_3_layer1[] = {
	gsSPClearGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 146, 197, 255, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_rmap_thwomps_map_mat_3_layer1[] = {
	gsSPSetGeometryMode(G_SHADE | G_ZBUFFER),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx rmap_thwomps_rmap_thwomps_mesh[] = {
	gsSPDisplayList(mat_rmap_thwomps_map_mat_1_layer1),
	gsSPDisplayList(rmap_thwomps_rmap_thwomps_mesh_tri_0),
	gsSPDisplayList(mat_revert_rmap_thwomps_map_mat_1_layer1),
	gsSPDisplayList(mat_rmap_thwomps_map_mat_3_layer1),
	gsSPDisplayList(rmap_thwomps_rmap_thwomps_mesh_tri_1),
	gsSPDisplayList(mat_revert_rmap_thwomps_map_mat_3_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

