#include "src/game/envfx_snow.h"

const GeoLayout rfacade2_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rfacade2_rfacade2_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, rfacade2_rfacade2_mesh_layer_5),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rfacade2_rfacade2_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_FORCE, rfacade2_rfacade2_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
