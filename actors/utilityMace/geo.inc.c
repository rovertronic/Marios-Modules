#include "src/game/envfx_snow.h"

const GeoLayout utilityMace_geo[] = {
	GEO_CULLING_RADIUS(1000),
	GEO_OPEN_NODE(),
		GEO_SHADOW(1, 204, 200),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, utilityMace_utilityMace_mesh_layer_1),
			GEO_DISPLAY_LIST(LAYER_ALPHA, utilityMace_utilityMace_mesh_layer_4),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
