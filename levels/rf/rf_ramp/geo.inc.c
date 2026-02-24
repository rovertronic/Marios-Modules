#include "src/game/envfx_snow.h"

const GeoLayout rf_ramp_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_ramp_rf_ramp_mesh_layer_1_with_revert),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
