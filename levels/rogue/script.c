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
#include "levels/rogue/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_rogue_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _rogue_segment_7SegmentRomStart, _rogue_segment_7SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_MINIJUNC, minijunc_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_SHORT_HALL, smallhall_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_LOBBY, rlobby_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_DOORHOLE, doorhole_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_DOORHOLE_COVERED, doorhole_covered_geo),
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_CARPET_POINT, carpetPoint_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_TREASURE, rtresure_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_WALLJUMP, rwalljump_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_LONGJUMP, rlongjump_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_VANISHHOP, vanishhop_geo),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, rogue_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		MARIO_POS(0x01, 0, 0, 0, 0),
		OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, (0x0A << 16), bhvAirborneWarp),
		TERRAIN(rogue_area_1_collision),
		ROOMS(rogue_area_1_collision_rooms),
		MACRO_OBJECTS(rogue_area_1_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};