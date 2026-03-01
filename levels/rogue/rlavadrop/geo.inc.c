#include "src/game/envfx_snow.h"

const GeoLayout rlavadrop_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rlavadrop_rlavadrop_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rlavadrop_rlavadrop_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
