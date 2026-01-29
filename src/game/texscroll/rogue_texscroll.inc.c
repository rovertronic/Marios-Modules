#include "levels/rogue/header.h"
#include "levels/rogue/rfnab/texscroll.inc.c"
#include "levels/rogue/rbtcm/texscroll.inc.c"
#include "levels/rogue/vanishhop/texscroll.inc.c"
#include "levels/rogue/facade1/texscroll.inc.c"
#include "levels/rogue/rgardenhall/texscroll.inc.c"
#include "levels/rogue/rfurnace/texscroll.inc.c"
#include "levels/rogue/rcavejump/texscroll.inc.c"
#include "levels/rogue/rboss/texscroll.inc.c"
#include "levels/rogue/rfacade2/texscroll.inc.c"
#include "levels/rogue/rthwomps/texscroll.inc.c"
void scroll_textures_rogue() {
	scroll_rogue_level_geo_rfnab();
	scroll_rogue_level_geo_rbtcm();
	scroll_rogue_level_geo_vanishhop();
		scroll_rogue_level_geo_facade1();
	scroll_rogue_level_geo_rgardenhall();
	scroll_rogue_level_geo_rfurnace();
	scroll_rogue_level_geo_rcavejump();
	scroll_rogue_level_geo_rboss();
	scroll_rogue_level_geo_rfacade2();
	scroll_rogue_level_geo_rthwomps();
}
