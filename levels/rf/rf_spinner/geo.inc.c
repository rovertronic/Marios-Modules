#include "src/game/envfx_snow.h"

const GeoLayout rf_spinner_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_spinner_rf_spinner_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_spinner_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
