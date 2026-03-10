void scroll_gravity_crystal_bill_mesh_layer_5_vtx_1() {
	int i = 0;
	int count = 13;
	int width = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int timeX;
	float amplitudeX = 0.5;
	float frequencyX = 0.5;
	float offsetX = 0.0;
	Vtx *vertices = segmented_to_virtual(gravity_crystal_bill_mesh_layer_5_vtx_1);

	deltaX = (int)(amplitudeX * frequencyX * coss((frequencyX * timeX + offsetX) * (1024 * 16 - 1) / 6.28318530718) * 0x20);

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;	timeX += 1;
}

void scroll_actor_geo_gravity_crystal() {
	scroll_gravity_crystal_bill_mesh_layer_5_vtx_1();
};
