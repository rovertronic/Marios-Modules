void scroll_gfx_mat_rfnab_static() {
	Gfx *mat = segmented_to_virtual(mat_rfnab_static);

	shift_s(mat, 9, PACK_TILESIZE(0, 73));
	shift_t(mat, 9, PACK_TILESIZE(0, 21));

};

void scroll_rogue_level_geo_rfnab() {
	scroll_gfx_mat_rfnab_static();
};
