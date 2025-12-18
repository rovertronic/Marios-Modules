#include "src/game/envfx_snow.h"

const GeoLayout dungeondoor_geo[] = {
	GEO_CULLING_RADIUS(1000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, dungeondoor_dungeondoor_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
