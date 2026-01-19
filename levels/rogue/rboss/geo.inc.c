#include "src/game/envfx_snow.h"

const GeoLayout rboss_geo[] = {
	GEO_CULLING_RADIUS(15000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rboss_rboss_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rboss_rboss_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
