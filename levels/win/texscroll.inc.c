void scroll_win_dl_rf_end_mesh_layer_1_vtx_6() {
	int i = 0;
	int count = 36;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(win_dl_rf_end_mesh_layer_1_vtx_6);

	deltaY = (int)(-1.0 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_win() {
	scroll_win_dl_rf_end_mesh_layer_1_vtx_6();
};
