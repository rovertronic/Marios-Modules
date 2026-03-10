#include "actors/group0.h"
#include "actors/hover/texscroll.inc.c"
#include "actors/save/texscroll.inc.c"
#include "actors/mchest/texscroll.inc.c"
#include "actors/gravity_crystal/texscroll.inc.c"
void scroll_textures_group0() {
	scroll_actor_geo_hover();
		scroll_actor_geo_save();
	scroll_actor_geo_mchest();
	scroll_actor_geo_gravity_crystal();
}
