void scroll_rboss_rboss_mesh_layer_1_vtx_7() {
	int i = 0;
	int count = 388;
	int width = 128 * 0x20;
	int height = 128 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(rboss_rboss_mesh_layer_1_vtx_7);

	deltaX = (int)(1.0 * 0x20) % width;
	deltaY = (int)(0.0 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_gfx_mat_rboss_gravity_layer1() {
	Gfx *mat = segmented_to_virtual(mat_rboss_gravity_layer1);

	shift_s(mat, 15, PACK_TILESIZE(0, 1));

};

void scroll_rogue_level_geo_rboss() {
	scroll_rboss_rboss_mesh_layer_1_vtx_7();
	scroll_gfx_mat_rboss_gravity_layer1();
};
