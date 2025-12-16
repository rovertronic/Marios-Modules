#include "src/game/envfx_snow.h"

const GeoLayout title2_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, title2_title2_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, title2_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
