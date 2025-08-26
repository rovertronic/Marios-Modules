void scroll_gfx_mat_mchest_lites() {
	Gfx *mat = segmented_to_virtual(mat_mchest_lites);
	static int interval_tex_mchest_lites = 4;
	static int cur_interval_tex_mchest_lites = 4;

	if (--cur_interval_tex_mchest_lites <= 0) {
		shift_s(mat, 12, PACK_TILESIZE(0, 64));
		cur_interval_tex_mchest_lites = interval_tex_mchest_lites;
	}

};

void scroll_actor_geo_mchest() {
	scroll_gfx_mat_mchest_lites();
};
