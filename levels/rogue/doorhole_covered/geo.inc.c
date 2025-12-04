#include "src/game/envfx_snow.h"

const GeoLayout doorhole_covered_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, doorhole_covered_doorhole_covered_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, doorhole_covered_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
