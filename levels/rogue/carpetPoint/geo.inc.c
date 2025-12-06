#include "src/game/envfx_snow.h"

const GeoLayout carpetPoint_geo[] = {
	GEO_CULLING_RADIUS(1000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE_DECAL, carpetPoint_carpetPoint_mesh_layer_2),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
