#include "src/game/envfx_snow.h"

const GeoLayout rgrindr_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rgrindr_rgrindr_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rgrindr_rgrindr_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, rgrindr_rgrindr_mesh_layer_6),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
