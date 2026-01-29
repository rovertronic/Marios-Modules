#include "src/game/envfx_snow.h"

const GeoLayout rthwomps_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rthwomps_rthwomps_mesh_layer_1_with_revert),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rthwomps_rthwomps_mesh_layer_4_with_revert),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, rthwomps_rthwomps_mesh_layer_6_with_revert),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
