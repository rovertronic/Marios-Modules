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
#include "levels/temple/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_temple_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x7, _temple_segment_7SegmentRomStart, _temple_segment_7SegmentRomEnd), 
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
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, temple_area_1),
		MARIO_POS(0x01, 0, 0, 674, 0),
		OBJECT(MODEL_CHEST, 4520, 199, -195, 0, -180, 0, (MOD_JUMP << 16), bhvChest),
		OBJECT(MODEL_CHEST, -900, 279, -6868, 0, 90, 0, (MOD_HIT_GROUND << 16), bhvChest),
		OBJECT(MODEL_CHEST, -100, 279, -6868, 0, -90, 0, (5 << 24) | (MOD_JUMP << 16), bhvChest),
		OBJECT(MODEL_CHEST, 105, 1080, -5283, 0, -90, 0, (MOD_POW << 16), bhvChest),
		OBJECT(MODEL_NONE, -363, 1080, -5283, 0, 90, 0, (0 << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 186, 166, -5348, 0, 0, 0, (2 << 16), bhvCoinFormation),
		OBJECT(MODEL_GOOMBA, 4332, -57, -3851, 0, 149, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 4732, -57, -4835, 0, -148, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_NONE, 4040, 633, -287, 0, 0, 0, (0 << 24), bhvStar),
		TERRAIN(temple_area_1_collision),
		MACRO_OBJECTS(temple_area_1_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_STONE),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 0, 674, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};