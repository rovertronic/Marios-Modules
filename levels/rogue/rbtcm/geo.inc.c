#include "src/game/envfx_snow.h"

const GeoLayout rbtcm_geo[] = {
	GEO_CULLING_RADIUS(10000),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rbtcm_rbtcm_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, rbtcm_rbtcm_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_FORCE, rbtcm_rbtcm_mesh_layer_0),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, rbtcm_rbtcm_mesh_layer_5),
		GEO_DISPLAY_LIST(LAYER_OPAQUE_DECAL, rbtcm_rbtcm_mesh_layer_2),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
