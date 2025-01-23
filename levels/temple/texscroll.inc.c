void scroll_temple_dl_Cube_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 16;
	int width = 128 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(temple_dl_Cube_mesh_layer_5_vtx_0);

	deltaX = (int)(3.0 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_temple_dl_Cube_mesh_layer_5_vtx_1() {
	int i = 0;
	int count = 32;
	int width = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(temple_dl_Cube_mesh_layer_5_vtx_1);

	deltaX = (int)(0.20000000298023224 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_temple_dl_Cube_mesh_layer_5_vtx_2() {
	int i = 0;
	int count = 39;
	int width = 16 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(temple_dl_Cube_mesh_layer_5_vtx_2);

	deltaX = (int)(0.10000000149011612 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_gfx_mat_temple_dl_lava_layer1() {
	Gfx *mat = segmented_to_virtual(mat_temple_dl_lava_layer1);


	shift_s(mat, 14, PACK_TILESIZE(0, 1));
	shift_t(mat, 19, PACK_TILESIZE(0, 1));

};

void scroll_temple() {
	scroll_temple_dl_Cube_mesh_layer_5_vtx_0();
	scroll_temple_dl_Cube_mesh_layer_5_vtx_1();
	scroll_temple_dl_Cube_mesh_layer_5_vtx_2();
	scroll_gfx_mat_temple_dl_lava_layer1();
};
