#include "src/game/envfx_snow.h"

const GeoLayout rpush_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rpush_rpush_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
