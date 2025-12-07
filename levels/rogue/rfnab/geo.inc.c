#include "src/game/envfx_snow.h"

const GeoLayout rfnab_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rfnab_rfnab_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, rfnab_rfnab_mesh_layer_5),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
