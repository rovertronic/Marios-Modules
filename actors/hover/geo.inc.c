#include "src/game/envfx_snow.h"

const GeoLayout hover_geo[] = {
	GEO_CULLING_RADIUS(500),
	GEO_OPEN_NODE(),
		GEO_ASM(GEO_TRANSPARENCY_MODE_NORMAL, geo_update_layer_transparency),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, hover_hover_mesh_layer_5),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
