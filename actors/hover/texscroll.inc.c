void scroll_hover_hover_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 17;
	int width = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(hover_hover_mesh_layer_5_vtx_0);

	deltaX = (int)(-0.5 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_actor_geo_hover() {
	scroll_hover_hover_mesh_layer_5_vtx_0();
};
