#include "levels/rogue/header.h"
#include "levels/rogue/rfnab/texscroll.inc.c"
#include "levels/rogue/rbtcm/texscroll.inc.c"
#include "levels/rogue/vanishhop/texscroll.inc.c"
void scroll_textures_rogue() {
	scroll_rogue_level_geo_rfnab();
	scroll_rogue_level_geo_rbtcm();
	scroll_rogue_level_geo_vanishhop();
	}
