#include "src/game/envfx_snow.h"

const GeoLayout utilityFlame_geo[] = {
	GEO_CULLING_RADIUS(500),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, utilityFlame_utilityFlame_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, utilityFlame_utilityFlame_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
