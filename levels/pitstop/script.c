#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "make_const_nonconst.h"
#include "levels/pitstop/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_pitstop_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _pitstop_segment_7SegmentRomStart, _pitstop_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _ssl_skybox_yay0SegmentRomStart, _ssl_skybox_yay0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, pitstop_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_ROGUE, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, (0x0A << 16), bhvInstantActiveWarp),
		OBJECT(MODEL_CHEST, -1000, 66, 8302, 0, 90, 0, (60 << 24) | (0 << 16) | (2 << 8) | (3), bhvChest),
		OBJECT(MODEL_CHEST, -1000, 66, 9302, 0, 90, 0, (60 << 24) | (0 << 16) | (2 << 8) | (1), bhvChest),
		OBJECT(MODEL_CHEST, 1000, 66, 8802, 0, -90, 0, (25 << 24) | (0 << 16) | (1 << 8) | (5), bhvChest),
		OBJECT(MODEL_CHEST, 1000, 66, 9302, 0, -90, 0, (25 << 24) | (0 << 16) | (1 << 8) | (6), bhvChest),
		OBJECT(MODEL_CHEST, 1000, 66, 8302, 0, -90, 0, (25 << 24) | (0 << 16) | (1 << 8) | (4), bhvChest),
		OBJECT(MODEL_CHEST, -1000, 66, 8802, 0, 90, 0, (60 << 24) | (0 << 16) | (2 << 8) | (2), bhvChest),
		OBJECT(MODEL_MODULE, -1100, 358, 8302, 0, 90, 0, (3), bhvModuleSign),
		OBJECT(MODEL_MODULE, -1100, 358, 9302, 0, 90, 0, (1), bhvModuleSign),
		OBJECT(MODEL_MODULE, -1100, 358, 8802, 0, 90, 0, (2), bhvModuleSign),
		OBJECT(MODEL_MODULE, 1100, 358, 8802, 0, -90, 0, (5), bhvModuleSign),
		OBJECT(MODEL_MODULE, 1100, 358, 9302, 0, -90, 0, (6), bhvModuleSign),
		OBJECT(MODEL_MODULE, 1100, 358, 8302, 0, -90, 0, (4), bhvModuleSign),
		OBJECT(MODEL_STAR, 0, 0, -1071, 0, 0, 0, (1 << 16), bhvWarp),
		TERRAIN(pitstop_area_1_collision),
		MACRO_OBJECTS(pitstop_area_1_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_STONE),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};