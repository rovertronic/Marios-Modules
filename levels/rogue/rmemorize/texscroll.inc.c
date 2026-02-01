void scroll_rmemorize_rmemorize_mesh_layer_1_vtx_2() {
	int i = 0;
	int count = 415;
	int width = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(rmemorize_rmemorize_mesh_layer_1_vtx_2);

	deltaX = (int)(0.20000000298023224 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_rogue_level_geo_rmemorize() {
	scroll_rmemorize_rmemorize_mesh_layer_1_vtx_2();
};
