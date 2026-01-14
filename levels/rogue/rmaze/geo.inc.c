#include "src/game/envfx_snow.h"

const GeoLayout rmaze_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rmaze_rmaze_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, rmaze_rmaze_mesh_layer_5),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
