// breakable_box.inc.c

struct ObjectHitbox sBreakableBoxHitbox = {
    /* interactType:      */ INTERACT_BREAKABLE,
    /* downOffset:        */  20,
    /* damageOrCoinValue: */   0,
    /* health:            */   1,
    /* numLootCoins:      */   0,
    /* radius:            */ 150,
    /* height:            */ 200,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 200,
};

struct ObjectHitbox sBreakableBoxWallHitbox = {
    /* interactType:      */ INTERACT_BREAKABLE,
    /* downOffset:        */  0,
    /* damageOrCoinValue: */   0,
    /* health:            */   1,
    /* numLootCoins:      */   0,
    /* radius:            */ 180,
    /* height:            */ 200,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 200,
};

// yeah... i lieded about these being static vars... who cares
extern int sSilverStarCt;
extern struct DungeonRoomVariant * sRoomWood;

void breakable_box_init(void) {
    o->oHiddenObjectSwitchObj = NULL;
    o->oAnimState = BREAKABLE_BOX_ANIM_STATE_CORK_BOX;
    switch (o->oBehParams2ndByte) {
        case BREAKABLE_BOX_BP_NO_COINS: o->oNumLootCoins = 0; break;
        case BREAKABLE_BOX_BP_3_COINS:  o->oNumLootCoins = 3; break;
        case BREAKABLE_BOX_BP_5_COINS:  o->oNumLootCoins = 5; break;
        case BREAKABLE_BOX_BP_LARGE:    cur_obj_scale(1.5f);  break;
    }
}

void hidden_breakable_box_actions(void) {
    struct Object *switchObj;
    cur_obj_set_model(MODEL_BREAKABLE_BOX);
    o->oFlags &= ~OBJ_FLAG_DUNGEON_CULL;

    if (o->oBehParams2ndByte == 2) {
        obj_set_hitbox(o, &sBreakableBoxWallHitbox);
    } else {
        obj_set_hitbox(o, &sBreakableBoxHitbox);
    }

    switch (o->oAction) {
        case BREAKABLE_BOX_ACT_HIDDEN:
            cur_obj_disable_rendering();
            cur_obj_become_intangible();
            if (o->oTimer == 0) {
                breakable_box_init();
            }
            if (o->oHiddenObjectSwitchObj == NULL) {
                o->oHiddenObjectSwitchObj = cur_obj_nearest_object_with_behavior(bhvFloorSwitchHiddenObjects);
            }
            switchObj = o->oHiddenObjectSwitchObj;
            if ((switchObj != NULL) && (switchObj->oAction == PURPLE_SWITCH_ACT_TICKING)) {
                o->oAction = BREAKABLE_BOX_ACT_ACTIVE;
                cur_obj_enable_rendering();
                cur_obj_unhide();
            }
            break;
        case BREAKABLE_BOX_ACT_ACTIVE:
            cur_obj_become_tangible();
            if (!gMarioState->bonkSignal && o->oBehParams2ndByte == 2) {
                cur_obj_become_intangible();
            }
            if (cur_obj_wait_then_blink(360 + GET_BPARAM3(o->oHiddenObjectSwitchObj->oBehParams)*2, 20)) o->oAction = BREAKABLE_BOX_ACT_HIDDEN;
            if (cur_obj_was_attacked_or_ground_pounded()) {
                spawn_mist_particles();
                spawn_triangle_break_particles(30, MODEL_DIRT_ANIMATION, 3.0f, TINY_DIRT_PARTICLE_ANIM_STATE_YELLOW);
                o->oAction = BREAKABLE_BOX_ACT_BROKEN;
                cur_obj_play_sound_2(SOUND_GENERAL_BREAK_BOX);

                if (o->oBehParams2ndByte == 2) {
                    sSilverStarCt++;
                    play_sound(SOUND_MENU_COLLECT_SECRET + (((u8) sSilverStarCt-1) << 16), gGlobalSoundSource);
                    spawn_orange_number(sSilverStarCt, 0, 0, 0);
                }
            }
            load_object_collision_model();
            break;
        case BREAKABLE_BOX_ACT_BROKEN:
            cur_obj_become_intangible();
            cur_obj_disable_rendering();
            o->oInteractStatus = INT_STATUS_NONE;
            switchObj = o->oHiddenObjectSwitchObj;
            if ((switchObj != NULL) && (switchObj->oAction == PURPLE_SWITCH_ACT_IDLE)) {
                o->oAction = BREAKABLE_BOX_ACT_HIDDEN;
            }
            break;
    }
}

void hidden_unbreakable_box_actions(void) {
    struct Object *switchObj;
    obj_set_collision_data(o, wdw_seg7_collision_hidden_platform);
    switch (o->oAction) {
        case BREAKABLE_BOX_ACT_HIDDEN:
            cur_obj_disable_rendering();
            cur_obj_become_intangible();
            if (o->oHiddenObjectSwitchObj == NULL) {
                o->oHiddenObjectSwitchObj = cur_obj_nearest_object_with_behavior(bhvFloorSwitchHiddenObjects);
            }
            switchObj = o->oHiddenObjectSwitchObj;
            if ((switchObj != NULL) && (switchObj->oAction == PURPLE_SWITCH_ACT_TICKING)) {
                o->oAction = BREAKABLE_BOX_ACT_ACTIVE;
                cur_obj_enable_rendering();
                cur_obj_unhide();
            }
            break;
        case BREAKABLE_BOX_ACT_ACTIVE:
            cur_obj_become_tangible();
            if (cur_obj_wait_then_blink(360, 20)) o->oAction = BREAKABLE_BOX_ACT_HIDDEN;
            load_object_collision_model();
            break;
    }
}

void bhv_hidden_object_loop(void) {
    if (o->oBehParams2ndByte == 1) {
        hidden_unbreakable_box_actions();
    } else {
        hidden_breakable_box_actions();
    }
}

void random_spawn_coins_or_amp(void) {
    if (random_u16()%2==0) {
        struct Object * amp = spawn_object(o,MODEL_AMP,bhvHomingAmp);
        amp->oPosY += 20.0f;
    } else {
        o->oNumLootCoins = 2;
        obj_explode_and_spawn_coins(46.0f, COIN_TYPE_YELLOW);
    }
}

void spawn_dungeon_woodroom_junk(void) {
    if (o->dungeonRoom[0]->variant == &sRoomWood) {
        struct Object * nearestTreasure;
        int hasTreasure = FALSE;
        nearestTreasure = cur_obj_nearest_object_with_behavior(bhvChest);
        if (nearestTreasure && dist_between_objects(o,nearestTreasure) < 10.0f) {
            hasTreasure = TRUE;
        } else {
            nearestTreasure = cur_obj_nearest_object_with_behavior(bhvStar);
            if (nearestTreasure && dist_between_objects(o,nearestTreasure) < 150.0f) {
                hasTreasure = TRUE;
            }
        }
        if (!hasTreasure) {
            random_spawn_coins_or_amp();
        }
    }
}

void bhv_breakable_box_loop(void) {
    obj_set_hitbox(o, &sBreakableBoxHitbox);
    cur_obj_set_model(MODEL_BREAKABLE_BOX);
    if (o->oTimer == 0) breakable_box_init();
    if (cur_obj_was_attacked_or_ground_pounded()) {
        obj_explode_and_spawn_coins(46.0f, COIN_TYPE_YELLOW);
        create_sound_spawner(SOUND_GENERAL_BREAK_BOX);
        spawn_dungeon_woodroom_junk();
    }
}
