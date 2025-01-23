#include "src/game/envfx_snow.h"

const GeoLayout bdoorl_geo[] = {
	GEO_CULLING_RADIUS(1000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, bdoorl_bdoorl_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, bdoorl_bdoorl_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
