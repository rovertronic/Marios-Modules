#include "src/game/envfx_snow.h"

const GeoLayout module3d_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_module_material),
		GEO_DISPLAY_LIST(LAYER_ALPHA, module3d_module3d_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, module3d_module3d_mesh_layer_5),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
