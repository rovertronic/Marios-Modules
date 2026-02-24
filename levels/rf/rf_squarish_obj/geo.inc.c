#include "src/game/envfx_snow.h"

const GeoLayout rf_squarish_obj_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_squarish_obj_rf_squarish_obj_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_squarish_obj_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
