#include "src/game/envfx_snow.h"

const GeoLayout minijunc_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, minijunc_minijunc_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
