#include "src/game/envfx_snow.h"

const GeoLayout rf_tilting_obj_geo[] = {
	GEO_CULLING_RADIUS(1000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rf_tilting_obj_rf_tilting_obj_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
