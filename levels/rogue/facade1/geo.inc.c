#include "src/game/envfx_snow.h"

const GeoLayout facade1_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, facade1_facade1_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, facade1_facade1_mesh_layer_5),
		GEO_DISPLAY_LIST(LAYER_FORCE, facade1_facade1_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
