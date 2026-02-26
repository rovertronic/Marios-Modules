#include "src/game/envfx_snow.h"

const GeoLayout rf_rino_obj_geo[] = {
	GEO_CULLING_RADIUS(500),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_rino_obj_rf_rino_obj_mesh_layer_1_with_revert),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
