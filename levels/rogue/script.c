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
#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/rogue/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_rogue_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _rogue_segment_7SegmentRomStart, _rogue_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x5, _group1_yay0SegmentRomStart, _group1_yay0SegmentRomEnd), 
	LOAD_RAW(0xc, _group1_geoSegmentRomStart, _group1_geoSegmentRomEnd), 
	LOAD_YAY0(0x6, _group17_yay0SegmentRomStart, _group17_yay0SegmentRomEnd), 
	LOAD_RAW(0xd, _group17_geoSegmentRomStart, _group17_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_2), 
	JUMP_LINK(script_func_global_18), 
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
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_FNAB, rfnab_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_BTCM, rbtcm_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_BALDI, baldi_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_BALDI_DOOR, baldidoor_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_DOOR, dungeondoor_geo),
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_CLOCKHAND, dungeonclockhand_geo),

	LOAD_MODEL_FROM_GEO(MODEL_ROOM_WOOD, rboxes_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_SPACEWORLD, spaceworld_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_FACADE1, facade1_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_GARDENHALL, rgardenhall_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_FURNACE, rfurnace_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_GRINDR, rgrindr_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_CAVEJUMP, rcavejump_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_SPLITHALL, rsplithall_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_AUTOMAZE, rmaze_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_SUPER_TREASURE, rsupertreasure_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_SILVER_PILLAR, rsilverpillar_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_BNS, rbns_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_CLOCK, rclock_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RED_COIN, rredcoin_geo),
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_BOSS, rboss_geo),

	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_TREE_1, bubbly_tree_geo),
	LOAD_MODEL_FROM_GEO(MODEL_DUNGEON_TREE_2, spiky_tree_geo),
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