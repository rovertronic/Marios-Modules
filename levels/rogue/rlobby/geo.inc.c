#include "src/game/envfx_snow.h"

const GeoLayout rlobby_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rlobby_rlobby_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rlobby_rlobby_mesh_layer_4),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -3885, -212, -1133),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
