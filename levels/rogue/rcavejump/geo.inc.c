#include "src/game/envfx_snow.h"

const GeoLayout rcavejump_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rcavejump_rcavejump_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, rcavejump_rcavejump_mesh_layer_6),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, rcavejump_rcavejump_mesh_layer_5),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
