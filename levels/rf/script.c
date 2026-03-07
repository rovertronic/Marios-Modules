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
#include "levels/rf/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_rf_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _rf_segment_7SegmentRomStart, _rf_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _bits_skybox_yay0SegmentRomStart, _bits_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group1_yay0SegmentRomStart, _group1_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group1_geoSegmentRomStart, _group1_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group17_yay0SegmentRomStart, _group17_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group17_geoSegmentRomStart, _group17_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_2), 
	JUMP_LINK(script_func_global_18), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_STRAIGHT, rf_straight_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_RIGHT, rf_right_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_LEFT, rf_left_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_RAMP, rf_ramp_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_TILTING, rf_tilting_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_SQUARISH, rf_squarish_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_AMPLECTED, rf_amplected_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_RINO, rf_rino_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_SLOPE, rf_slope_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_BLANKJUMP, rf_blankjump_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_VERTICAL, rf_vertical_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_ROOM_RF_END, rf_end_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_RF_TILTING, rf_tilting_obj_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_RF_SPINNER, rf_spinner_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_RF_SQUARISH, rf_squarish_obj_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_RF_RINO, rf_rino_obj_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_RF_RINO_PLAT, rf_rino_plat_geo), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, rf_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B /* fall recover */, LEVEL_RF, 0x01, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1 /* death */, LEVEL_PITSTOP, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0 /*final boss*/, LEVEL_FINALBOSS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, -297, 0, 0, 0, 0, (0x0B << 16), bhvFadingWarp),
		OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, (0x0A << 16), bhvInstantActiveWarp),
		TERRAIN(rf_area_1_collision),
		ROOMS(rf_area_1_collision_rooms),
		MACRO_OBJECTS(rf_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_KOOPA_ROAD),
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