#include "src/game/envfx_snow.h"

const GeoLayout rfurnace_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rfurnace_rfurnace_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rfurnace_rfurnace_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
