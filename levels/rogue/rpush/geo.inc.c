#include "src/game/envfx_snow.h"

const GeoLayout rpush_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rpush_rpush_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rpush_rpush_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, rpush_rpush_mesh_layer_6),
		GEO_DISPLAY_LIST(LAYER_FORCE, rpush_rpush_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
