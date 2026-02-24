#include "src/game/envfx_snow.h"

const GeoLayout rf_right_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_right_rf_right_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
