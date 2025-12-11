#include "src/game/envfx_snow.h"

const GeoLayout vanishhop_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, vanishhop_vanishhop_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, vanishhop_vanishhop_mesh_layer_4),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 557, 0, -2037),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
