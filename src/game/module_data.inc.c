// INVENTORY STRUCTURE DECLARATIONS
s32 creativePanelCondition(void) {
    return TRUE;
}

s32 storagePanelCondition(void) {
    // Storage only activates once your initial inventory overfills
    for (int x = 0; x < INVENTORY_SLOTS_X; x++) {
        for (int y = 5; y < 10; y++) {
            if (get_inventory(x,y) != MOD_EMPTY) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

s32 passivePanelCondition(void) {
    return ( (gMariosModulesSave.file[gMariosModulesSaveIndex].flags & SAVE_FLAG_PASSIVE) != 0 );
}

struct module_panel module_panel_info[] = {
    [PANEL_SETTINGS] = {
        .name = "Settings + Stats",
        .offset = 48,
        .size = 2,
        .unlock = NULL,
    },
    [PANEL_ACTIONS] = {
        .name = "@B@Actions",
        .offset = 0,
        .size = 5,
        .unlock = NULL,
    },
    [PANEL_PASSIVE] = {
        .name = "@Y@Passive",
        .offset = 10,
        .size = 5,
        .unlock = passivePanelCondition,
    },
    [PANEL_STORAGE] = {
        .name = "@1@Storage",
        .offset = 5,
        .size = 5,
        .unlock = storagePanelCondition,
    },
    [PANEL_VANITY] = {
        .name = "@P@Vanity",
        .offset = 44,
        .size = 4,
        .unlock = NULL,
    },
    [PANEL_CREATIVE] = {
        .name = "@G@Creative",
        .offset = 39,
        .size = 5,
        .unlock = creativePanelCondition,
    },
};

struct inventory_row inventory_row_info[INVENTORY_SLOTS_Y] = {
     // Actions
    [0] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_A, .whitelist_flags = WHITELIST_ACTION},
    [1] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_B, .whitelist_flags = WHITELIST_ACTION},
    [2] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [3] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [4] = {.type = ROW_STORAGE, .mod_type_prio = -1},

     // Extra Storage
    [5] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [6] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [7] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [8] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [9] = {.type = ROW_STORAGE, .mod_type_prio = -1},

     // Passive
    [10] = {.type = ROW_SOCKET, .icon = MOD_PASSIVE, .whitelist_flags = WHITELIST_PASSIVE, .wrap = 1},
    [11] = {.type = ROW_SOCKET, .icon = MOD_WRAP, .whitelist_flags = WHITELIST_PASSIVE},
    [12] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [13] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [14] = {.type = ROW_STORAGE, .mod_type_prio = -1},

    // Storage
    //[5] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    //[6] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    //[7] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    //[8] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    //[9] = {.type = ROW_STORAGE, .mod_type_prio = -1},

    // Creative
    [39] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [40] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [41] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [42] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [43] = {.type = ROW_STORAGE, .mod_type_prio = -1},

     // Vanity
    [44] = {.type = ROW_SOCKET, .icon = MOD_VANITY, .whitelist_flags = WHITELIST_VANITY, .wrap = 1},
    [45] = {.type = ROW_SOCKET, .icon = MOD_WRAP,   .whitelist_flags = WHITELIST_VANITY},
    [46] = {.type = ROW_STORAGE, .mod_type_prio = MTYPE_VANITY},
    [47] = {.type = ROW_STORAGE, .mod_type_prio = MTYPE_VANITY},

     // Settings
    [48] = {.type = ROW_SOCKET, .icon = MOD_SETTINGS, .whitelist_flags = (1 << MTYPE_SETTINGS)},
    [49] = {.type = ROW_STORAGE, .mod_type_prio = -1},
};

struct module_type_info module_type_infos[] = {
    [MTYPE_MOVE] = {"B","Action",{0x64, 0x64, 0xF0}},
    [MTYPE_BUFF] = {"O","Upgrade",{200, 0, 0}},
    [MTYPE_COND] = {"G","Sequencing",{0, 170, 0}},
    [MTYPE_INPUT] = {"","Input",{0xC9, 0x82, 0x30}},
    [MTYPE_NONMOD] = {"",NULL,{0x00, 0x00, 0x00}},
    [MTYPE_VANITY] = {"P","Vanity",{0xD3,0x81,0xFC}},
    [MTYPE_SETTINGS] = {"1","Option",{0xAA,0xAA,0xAA}},
    [MTYPE_LOGIC] = {"Y","Logic",{210,176,0}},
    [MTYPE_ELEMENT] = {"E","Element",{0,0x90,0x90}},
    [MTYPE_PASSIVE] = {"E","Passive",{0,0x90,0x90}},
};

#define INVENTORY_PRINT_OFFSET_X 26
#define INVENTORY_PRINT_OFFSET_Y 24

// MODULE FUNCTIONS

void module_jump(struct module_execution_thread * met, u8 call_context) {
    met->doaircooldown = TRUE;

    u8 force = FALSE;
    if (met->element != ELEMENT_NORMAL) {
        Mat4 direction;
        Vec3f origin = {gMarioState->pos[0],gMarioState->pos[1]-100.0f,gMarioState->pos[2]};
        summon_element_projectile(met->element, gMarioState->pos, direction);
        met->element = ELEMENT_NORMAL;
        force = TRUE;
    }

    if ((force)||(!mario_floor_is_steep(gMarioState) && (GROUNDED))) {
        switch(met->mod) {
            case 0:
                module_log_message(met,"UPG is 0, single jump action performed.",0);
                set_mario_action(gMarioState,ACT_JUMP,0);
                break;
            case 1:
                module_log_message(met,"UPG is 1, double jump action performed.",0);
                set_mario_action(gMarioState,ACT_DOUBLE_JUMP,0);
                met->mod--;
                break;
            default:
                module_log_message(met,"UPG is 2+, triple jump action performed.",0);
                set_mario_action(gMarioState,ACT_TRIPLE_JUMP,0);
                if (gMarioState->flags & MARIO_WING_CAP) {
                    set_mario_action(gMarioState,ACT_FLYING_TRIPLE_JUMP,0);
                }
                met->mod-=2;
                break;
        }
    } else {
        module_log_message(met,"Not on valid ground, pressed A instead.",0);
        gMarioState->input |= INPUT_A_PRESSED;
    }

    met->x++;
}

void module_tornado(struct module_execution_thread * met, u8 call_context) {
    if (!(GROUNDED)) {
        met->doaircooldown = TRUE;
        if (met->mod > 0) {
            module_log_message(met,"Twirl action performed with speed @O@UPG.@@",0);
            set_mario_action(gMarioState,ACT_TWIRLING,met->mod+1);
            met->mod=0;
        } else {
            module_log_message(met,"Twirl action performed.",0);
            set_mario_action(gMarioState,ACT_TWIRLING,0);
        }
    } else {
        module_log_message(met,"Failed to initiate twirl.",0);
    }
    met->x++;
}

void module_attack(struct module_execution_thread * met, u8 call_context) {
    module_log_message(met,"Pressed B.",0);

    met->doaircooldown = TRUE;
    gMarioState->input |= INPUT_B_PRESSED;

    met->spd = 0;

    met->x++;
}

void module_zaction(struct module_execution_thread * met, u8 call_context) {
    met->doaircooldown = TRUE;
    if (GROUNDED) {
        module_log_message(met,"On ground, do long jump.",0);
        set_mario_action(gMarioState,ACT_LONG_JUMP,0);
    } else {
        if (gMarioState->action != ACT_GROUND_POUND) {
            module_log_message(met,"In air, do ground pound.",0);
            set_mario_action(gMarioState,ACT_GROUND_POUND,0);
        }
    }
    met->x++;
}


void module_pow(struct module_execution_thread * met, u8 call_context) {
    module_log_message(met,"UPG Increased by %d.",met->extra_data);
    met->mod+=met->extra_data;
    met->x++;
}

void module_spd(struct module_execution_thread * met, u8 call_context) {
    met->spd++;
    met->x++;
}

void module_repeat(struct module_execution_thread * met, u8 call_context) {
    int wrap = (met->y != 0) && (inventory_row_info[met->y-1].wrap) ;

    if (met->used_flags & (1 << (met->x + (wrap * 8)) )) {
        module_log_message(met,"Already repeated, so pass through.",0);

        met->x++;
    } else {
        module_log_message(met,"Repeat from start.",0);

        met->used_flags |= (1 << (met->x + (wrap * 8)) );
        met->x=0;

        if (wrap) {
            met->y--;
        }
    }
}

void module_floor(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            module_log_message(met,"Waiting for floor touch...",0);
            met->halted = TRUE;
            break;
        case MCC_HALTED:
            if (GROUNDED) {
                module_log_message(met,"Floor touched, continue.",0);
                met->halted = FALSE;
                met->x++;
                break;
            }
            break;
    }
}

void module_floor_upg(struct module_execution_thread * met, u8 call_context) {
    module_log_message(met,"UPG+%d, since landed %d times.",met->landing_count);

    met->x++;
    met->mod+=met->landing_count;
}

void module_wall(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            met->timer = 0;
            module_log_message(met,"Waiting for wall touch...",0);
            break;
        case MCC_HALTED:
            if (gMarioState->wall != NULL) {
                module_log_message(met,"Wall touched, continue.",0);

                met->halted = FALSE;
                met->x++;
                break;
            }
            if (!(((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING))) {
                met->timer = 0;
                break;
            }
            if (met->timer >= 15) {
                module_log_message(met,"Wall module timed out, cancel sequence.",0);
                met->timer = 0;
                met->cooldown = TRUE;
            }
            break;
    }
}

void module_timer(struct module_execution_thread * met, u8 call_context) {
    u8 time = 0;
    switch(met->option) {
        case 0:
            time = 14;
            break;
        case 1:
            time = 6;
            break;
        case 2:
            time = 9;
            break;
        case 3:
            time = 29;
            break;
        case 4:
            time = 59;
            break;
        case 6:
            time = 1;
            break;
    }
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            met->timer = 0;
            module_log_message(met,"Timer waiting %d frames.", time+1);
            break;
        case MCC_HALTED:
            if (met->timer >= time) {
                module_log_message(met,"Timer finished.",0);

                met->halted = FALSE;
                met->x++;
                met->mod = 0;
                break;
            }
            break;
    }
}

void module_grav(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            module_log_message(met,"Waiting for Mario to fall...",0);
            break;
        case MCC_HALTED:
            if (gMarioState->vel[1] < 0.0) {
                module_log_message(met,"Mario fell, continue.",0);
                met->halted = FALSE;
                met->x++;
                break;
            }
            if (!GROUNDED) {
                met->timer = 0;
            }
            if (met->timer >= 15) {
                met->timer = 0;
                met->cooldown = TRUE;

                module_log_message(met,"Down module timed out, cancel sequence.",0);
            }
            break;
    }
}

void module_input(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            play_sound(SOUND_GENERAL_BOWSER_KEY_LAND, gGlobalSoundSource);
            module_log_message(met,"Polling for player input.",0);
            if (gPlayer1Controller->buttonPressed & met->input) {
                module_log_message(met,"Player input accepted, continue.",0);

                met->halted = FALSE;
                met->input_notify = FALSE;
                met->x++;
                break;
            }
            met->halted = TRUE;
            met->timer = 0;
            met->input_notify = TRUE;
            break;
        case MCC_HALTED:
            if (gPlayer1Controller->buttonPressed & met->input) {
                module_log_message(met,"Player input accepted, continue.",0);

                met->halted = FALSE;
                met->input_notify = FALSE;
                met->x++;
                break;
            }
            if (met->timer >= 30) {
                module_log_message(met,"Player input rejected, cancel.",0);

                met->input_notify = FALSE;
                met->cooldown = TRUE;
                met->timer = 0;
            }
            break;
    }
}

void module_platform(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            module_log_message(met,"Spawned air platform.",0);

            met->doaircooldown = TRUE;
            met->halted = TRUE;
            met->timer = 0;

            play_sound(SOUND_ACTION_TELEPORT, gGlobalSoundSource);
            struct Object * hover = spawn_object(gMarioState->marioObj,MODEL_HOVER,bhvHover);
            hover->oBehParams2ndByte = met->mod;
            if (gMarioState->vel[1] < 0.0f) {
                gMarioState->vel[1] = 0.0f;
            }
            break;
        case MCC_HALTED:
            if (met->timer >= 1) {
                met->halted = FALSE;
                met->x++;
                met->mod = 0;
                break;
            }
            break;
    }
}

void module_cap(struct module_execution_thread * met, u8 call_context) {
    gMarioState->capTimer += 60;
    switch(met->mod) {
        case 0:
            module_log_message(met,"UPG is 0, enable vanish cap.",0);
            gMarioState->flags |= MARIO_VANISH_CAP;
            break;
        case 1:
            module_log_message(met,"UPG is 1, enable metal cap.",0);
            gMarioState->flags |= MARIO_METAL_CAP;
            break;
        default:
            module_log_message(met,"UPG is 2+, enable wing cap.",0);
            gMarioState->flags |= MARIO_WING_CAP;
            break;
    }

    met->mod = 0;
    met->x++;
}

void module_element(struct module_execution_thread * met, u8 call_context) {
    met->element = met->extra_data;
    met->x++;
}

Vec3f colorBlendStack[10];
int colorBlendCount = 0;

void module_clothes_color(struct module_execution_thread * met, u8 call_context) {
    // Somewhat hacky, inject mario's material dls with new color
    // won't crash N64 i think and that's all that matters
    if (colorBlendCount == 0) {
        module_log_message(met,"No colors to apply, do nothing.",0);
        met->x++;
        return;
    }


    Gfx ** lightList = met->extra_data;

    while (*lightList != NULL) {
        Gfx * dlhead = segmented_to_virtual(*lightList);

        Vec3f final = {0.0f,0.0f,0.0f};
        for (int i = 0; i < colorBlendCount; i++) {
            for (int j = 0; j < 3; j++) {
                final[j] += colorBlendStack[i][j] * (1.0f/colorBlendCount);
            }
        }

        u8 r = final[0]*255.0f;
        u8 g = final[1]*255.0f;
        u8 b = final[2]*255.0f;
        
        gSPLightColor(dlhead++,LIGHT_1, (r<<24) | (g<<16) | (b<<8) | 0xFF);
        gSPLightColor(dlhead++,LIGHT_2, (r/2<<24) | (g/2<<16) | (b/2<<8) | 0xFF);

        lightList++;
    }

    colorBlendCount = 0;

    module_log_message(met,"Applying colors to clothes.",0);

    gModuleUpdateVanity = TRUE;
    met->x++;
}

void module_color(struct module_execution_thread * met, u8 call_context) {
    vec3f_copy(colorBlendStack[colorBlendCount],*((Vec3f *)met->extra_data));
    colorBlendCount++;

    module_log_message(met,"Mixing color into pallete.",0);

    gModuleUpdateVanity = TRUE;
    met->x++;
}

void module_settings(struct module_execution_thread * met, u8 call_context) {
    *((u8 *)met->extra_data) = 1;

    gModuleUpdateVanity = TRUE;
    met->x++;
}

void module_woman(struct module_execution_thread * met, u8 call_context) {
    gMarioState->marioObj->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_WOMAN];

    gModuleUpdateVanity = TRUE;
    met->x++;
}

void module_flip(struct module_execution_thread * met, u8 call_context) {
    module_log_message(met,"Y velocity set to %d.",-gMarioState->vel[1]);

    met->doaircooldown = TRUE;
    gMarioState->vel[1] = -gMarioState->vel[1];
    met->x++;
}

void module_if(struct module_execution_thread * met, u8 call_context) {
    s32 condition = FALSE;
    switch(met->option) {
        case 0:
            module_log_message(met,"AND Gate, %d conditions",met->condition_count);
            condition = TRUE;
            for (int i = 0; i < met->condition_count; i++) {
                if (!(met->condition_flags & (1 << i))) {
                    condition = FALSE;
                }
            }
            break;
        case 1:
            module_log_message(met,"NOT Gate, %d conditions",met->condition_count);
            condition = (met->condition_flags == 0);
            break;
        case 2:
            module_log_message(met,"OR Gate, %d conditions",met->condition_count);
            condition = (met->condition_flags != 0);
            break;
    }

    if (condition) {
        module_log_message(met,"@G@Condition@@ is @B@TRUE@@, continue.",0);
        met->x++;
    } else {
        u8 revertX = met->x;
        s8 curModId = get_inventory(met->x,met->y);
        s8 stackLevel = 0;

        module_log_message(met,"@G@Condition@@ is @R@FALSE@@, go to end block.",0);
        while(!(curModId == MOD_ENDBLOCK && stackLevel == 0)) {

            switch(curModId) {
                case MOD_IF:
                    stackLevel++;
                    break;
                case MOD_ENDBLOCK:
                    stackLevel--;
                    if (stackLevel == 0) {
                        continue;
                    }
                    break;
            }

            met->x++;
            if (inventory_row_info[met->y].wrap && met->x == INVENTORY_SLOTS_X) {
                met->x = 0;
                met->y ++;
            }
            curModId = get_inventory(met->x,met->y);

            // Escape sequence, ie no valid endblock
            if (met->x >= INVENTORY_SLOTS_X) {
                met->x = revertX;
                module_log_message(met,"No end block found, ignore.",0);
                met->x++;
                break;
            }
        }
    }
    met->condition_flags = 0;
    met->condition_count = 0;
}

void module_if_floor(struct module_execution_thread * met, u8 call_context) {
    if (GROUNDED) {
        module_log_message(met,"On floor, condition set to @B@TRUE@@.",0);
        add_met_condition(met,TRUE);
    } else {
        module_log_message(met,"Off floor, condition set to @R@FALSE@@.",0);
        add_met_condition(met,FALSE);
    }
    met->x++;
}

void module_if_down(struct module_execution_thread * met, u8 call_context) {
    if (gMarioState->vel[1] < 0.0f) {
        module_log_message(met,"Falling, condition set to @B@TRUE@@.",0);
        add_met_condition(met,TRUE);
    } else {
        module_log_message(met,"Not falling, condition set to @R@FALSE@@.",0);
        add_met_condition(met,FALSE);
    }
    met->x++;
}

void module_if_input(struct module_execution_thread * met, u8 call_context) {
    u32 inpFlag = A_BUTTON;
    switch(met->option) {
        case 2:
        case 3:
            inpFlag = B_BUTTON;
            break;
        case 4:
        case 5:
            inpFlag = Z_TRIG;
    }

    u32 cond = (gPlayer1Controller->buttonDown & inpFlag);
    if (met->option % 2 == 0) {
        cond = (gPlayer1Controller->buttonPressed & inpFlag);
    }
    if (cond) {
        module_log_message(met,"Input met, condition set to @B@TRUE@@.",0);
        add_met_condition(met,TRUE);
    } else {
        module_log_message(met,"No input, condition set to @R@FALSE@@.",0);
        add_met_condition(met,FALSE);
    }
    met->x++;
}

void module_if_wall(struct module_execution_thread * met, u8 call_context) {
    if (gMarioState->wall != NULL) {
        module_log_message(met,"Wall hit, condition set to @B@TRUE@@.",0);
        add_met_condition(met,TRUE);
    } else {
        module_log_message(met,"Wall not hit, condition set to @R@FALSE@@.",0);
        add_met_condition(met,FALSE);
    }

    met->x++;
}

void module_stop(struct module_execution_thread * met, u8 call_context) {
    module_log_message(met,"Sequence stopped prematurely.",0);
    met->x = INVENTORY_SLOTS_X+1; // OOB = MOD_EMPTY
}

void module_monitor(struct module_execution_thread * met, u8 call_context) {
    if (!met->debug_monitor) {
        met->debug_monitor = TRUE;
        module_log_message(met,"Begin monitoring socket.",0);
    }
    met->x++;
}

void module_passive_effect(struct module_execution_thread * met, u8 call_context) {
    gMarioState->passiveFlag |= (1 << (u32)(met->extra_data));
    met->x++;
}

void module_rotate(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            met->timer = 0;

            switch(met->option) {
                case 0:
                    met->currentAngle = -0x4000;
                    break;
                case 1:
                    met->currentAngle = 0x8000;
                    break;
                case 2:
                    met->currentAngle = 0x4000;
                    break;
                case 3:
                    met->currentAngle = -(gMarioState->faceAngle[1] -gMarioState->intendedYaw);
                    break;
            }
            break;
        case MCC_HALTED:;
            s16 rotateInc = met->currentAngle/4;

            gMarioState->faceAngle[1] += rotateInc;
            if (met->timer >= 3) {
                met->halted = FALSE;
                met->x++;
            }
            break;
    }
}

Vec3f moduleRed = {1.0f,0.0f,0.0f};
Vec3f moduleBlue = {0.0f,0.0f,1.0f};
Vec3f moduleGreen = {0.0f,1.0f,0.0f};
Vec3f moduleYellow = {1.0f,1.0f,0.0f};
Vec3f moduleWhite = {1.0f,1.0f,1.0f};
Vec3f moduleBlack = {0.02f,0.02f,0.02f};
Vec3f moduleTan = {.996f,.756f,.474f};
Vec3f moduleBrown = {.6f,.3f,.1f};

Gfx * capLights[] = {
    &mat_mario_cap_v3,
    &mat_woman_cap_v3,
    NULL,
};

Gfx * jeanLights[] = {
    &mat_mario_body_v3,
    &mat_woman_body_v3,
    NULL,
};

Gfx * hairLights[] = {
    &mat_mario_sideburns_v3_001,
    &mat_mario_hair_v3_001,
    &mat_woman_hair_v3_001,
    NULL,
};

Gfx * skinLights[] = {
    &mat_mario_face_0___eye_open_v3_001,
    &mat_mario_face_1___eye_half_v3_001,
    &mat_mario_face_2___eye_closed_v3_001,
    &mat_mario_mustache_v3_001,

    &mat_woman_womanEye1,
    &mat_woman_womanEye2,
    &mat_woman_womanEye3,
    &mat_woman_mouth,
    &mat_woman_face_7___eye_X_v3_001,
    NULL,
};

char * ifInputOptions[] = {
    "@B@A@@ Pressed",
    "@B@A@@ Held",
    "@G@B@@ Pressed",
    "@G@B@@ Held",
    "@1@Z@@ Pressed",
    "@1@Z@@ Held",
    NULL,
};

char * timerOptions[] = {
    "1/2 Second",
    "1/4 Second",
    "1/3 Second",
    "1 Second",
    "2 Seconds",
    "1 Frame",
    "2 Frames",
    NULL,
};

char * ifOptions[] = {
    "All conditions are @B@TRUE@@.",
    "All conditions are @R@FALSE@@.",
    "Any condition is @B@TRUE@@.",
    NULL,
};

char * rotateOptions[] = {
    "90 degrees clockwise",
    "180 degrees",
    "90 degrees counter-clockwise",
    "To analog stick direction",
    NULL,
};

struct module_info module_infos[] = {
    // Sockets
    [MOD_BUTTON_A] = {MTYPE_INPUT,0,0,0,0,"",micons_abtn_rgba16,NULL,NULL,NULL},
    [MOD_BUTTON_B] = {MTYPE_INPUT,0,0,0,0,"",micons_bbtn_rgba16,NULL,NULL,NULL},
    [MOD_VANITY] = {MTYPE_INPUT,0,0,0,0,"",micons_vanity_rgba16,NULL,NULL,NULL},
    [MOD_SETTINGS] = {MTYPE_INPUT,0,0,0,0,"",micons_gear_rgba16,NULL,NULL,NULL},
    [MOD_WRAP] = {MTYPE_INPUT,0,0,0,0,"",micons_wrap_rgba16,NULL,NULL,NULL},
    [MOD_PASSIVE] = {MTYPE_INPUT,0,0,0,0,"Passive Socket",micons_p__rgba16,NULL,NULL,NULL},

    // Actions
    [MOD_JUMP] = {
        .name = "Jump",
        .type = MTYPE_MOVE,
        .tex = micons_jump_rgba16,
        .desc = "Makes Mario jump if possible.",
        .upg_desc = "Jump Tier increases per @O@UPG@@.",
        .unchainable = TRUE,
        .elementable = TRUE,
        .func = module_jump,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_TORNADO] = {
        .name = "Tornado",
        .type = MTYPE_MOVE,
        .tex = micons_tornado_rgba16,
        .desc = "If airborne, makes Mario spin.",
        .upg_desc = "Twirl speed increases per @O@UPG@@.",
        .unchainable = TRUE,
        .elementable = TRUE,
        .func = module_tornado,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_ATTACK] = {
        .name = "Attack",
        .type = MTYPE_MOVE,
        .tex = micons_pow_rgba16,
        .desc = "Makes Mario punch, kick, or dive, if possible.",
        .unchainable = TRUE,
        .func = module_attack,
        .elementable = TRUE,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_INPUT] = {
        .name = "Input",
        .type = MTYPE_COND,
        .tex = micons_btngen_rgba16,
        .desc = "Checks for a button press for one second, otherwise cancels.",
        .func = module_input,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_PLATFORM] = {
        .name = "Air Platform",
        .type = MTYPE_MOVE,
        .tex = micons_hover_rgba16,
        .desc = "Spawns a temporary air platform for one second.",
        .upg_desc = "Size +50* per @O@UPG@@.",
        .unchainable = TRUE,
        .func = module_platform,
        .cooldown = .5f,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_SWAP] = {
        .name = "Swap",
        .type = MTYPE_MOVE,
        .tex = micons_swap_rgba16,
        .desc = "Toggles swap platforms.",
    },

    [MOD_CAP] = {
        .name = "Cap",
        .type = MTYPE_MOVE,
        .tex = micons_cap_rgba16,
        .desc = "Enables cap power for two seconds. Can stack caps and time.",
        .upg_desc = "0:Vanish, 1:Metal, 2:Wing.",
        .cooldown = 3.0f,
        .func = module_cap,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_MONITOR] = {
        .name = "Debug Monitor",
        .type = MTYPE_MOVE,
        .tex = micons_script_rgba16,
        .desc = "Displays a verbose breakdown of the actions of all following modules. Press START to clear log.",
        .func = module_monitor,
        .creative = FALSE,
    },

    [MOD_GRAPPLE] = {
        .name = "Grapple",
        .type = MTYPE_MOVE,
        .tex = micons_grapple_rgba16,
        .desc = "Launches a grapple hook. Must hit wood.",
    },

    [MOD_FLIP_VEL] = {
        .name = "Velocity Flip",
        .type = MTYPE_MOVE,
        .tex = micons_swapule_rgba16,
        .desc = "Inverts Mario's Y velocity.",
        .unchainable = TRUE,
        .func = module_flip,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_ZACTION] = {
        .name = "Crouch Move",
        .type = MTYPE_MOVE,
        .tex = micons_ground_rgba16,
        .desc = "Ground pound in air, long jump on floor.",
        .unchainable = TRUE,
        .func = module_zaction,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    // Modifiers
    [MOD_POW] = {
        .name = "+1 Upgrade",
        .type = MTYPE_BUFF,
        .tex = micons_onepow_rgba16,
        .desc = "+1@O@UPG@@ to the next piece.",
        .cooldown = .5f,
        .func = module_pow,
        .extra_data = 1,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_POW2] = {
        .name = "+2 Upgrade",
        .type = MTYPE_BUFF,
        .tex = micons_twopow_rgba16,
        .desc = "+2@O@UPG@@ to the next piece.",
        .cooldown = .7f,
        .func = module_pow,
        .extra_data = 2,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_GROUND_UPG] = {
        .name = "Ground Upgrade",
        .type = MTYPE_BUFF,
        .tex = micons_ground_rgba16,
        .desc = "@O@UPG@@+ times landed during sequence.",
        .func = module_floor_upg,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_COOL] = {
        .name = "Heat Sink",
        .type = MTYPE_BUFF,
        .tex = micons_heatsink_rgba16,
        .desc = "Decreases cooldown time.",
        .func = NULL,
        .cooldown = -1.f,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_REPEAT] = {
        .name = "Repeat",
        .type = MTYPE_COND,
        .tex = micons_repeat_rgba16,
        .desc = "Repeats from the start once.",
        .func = module_repeat,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },

    [MOD_TIMER] = {
        .name = "Timer",
        .type = MTYPE_COND,
        .tex = micons_clock_rgba16,
        .desc = "Continues after set time has passed.",
        .func = module_timer,
        .options = timerOptions,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    // Conditions
    [MOD_HIT_WALL] = {
        .name = "Wall",
        .type = MTYPE_COND,
        .tex = micons_wall_rgba16,
        .desc = "Continues when touching wall, or cancels on floor touch.",
        .unchainable = TRUE,
        .func = module_wall,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_GRAV] = {
        .name = "Down",
        .type = MTYPE_COND,
        .tex = micons_grav_rgba16,
        .desc = "Continues when Mario has downward velocity, cancels on floor.",
        .unchainable = TRUE,
        .func = module_grav,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_HIT_GROUND] = {
        .name = "Ground",
        .type = MTYPE_COND,
        .tex = micons_ground_rgba16,
        .desc = "Continues when Mario touches the ground.",
        .unchainable = TRUE,
        .func = module_floor,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    // Non modifiers
    [MOD_NONMOD_KEY] = {
        .name = "Key",
        .type = MTYPE_NONMOD,
        .tex = micons_key_rgba16,
    },

    // Vanity
    [MOD_VAN_CAP] = {
        .name = "Cap Color",
        .type = MTYPE_VANITY,
        .tex = micons_cap_rgba16,
        .desc = "Mixes colors into cap + shirt.",
        .func = module_clothes_color,
        .extra_data = capLights,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_VAN_PANTS] = {
        .name = "Pants Color",
        .type = MTYPE_VANITY,
        .tex = micons_pants_rgba16,
        .desc = "Mixes colors into overalls.",
        .func = module_clothes_color,
        .extra_data = jeanLights,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_VAN_HAIR] = {
        .name = "Hair Color",
        .type = MTYPE_VANITY,
        .tex = micons_hair_rgba16,
        .desc = "Mixes colors into hair.",
        .func = module_clothes_color,
        .extra_data = hairLights,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_VAN_SKIN] = {
        .name = "Skin Color",
        .type = MTYPE_VANITY,
        .tex = micons_skin_rgba16,
        .desc = "Mixes colors into skin tone.",
        .func = module_clothes_color,
        .extra_data = skinLights,
        .creative = TRUE,
    },

    [MOD_RED] = {
        .name = "Red",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @R@red@@ into palette.",
        .func = module_color,
        .extra_data = &moduleRed,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_BLUE] = {
        .name = "Blue",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @B@blue@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlue,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_GREEN] = {
        .name = "Green",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @G@green@@ into palette.",
        .func = module_color,
        .extra_data = &moduleGreen,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_YELLOW] = {
        .name = "Yellow",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @Y@yellow@@ into palette.",
        .func = module_color,
        .extra_data = &moduleYellow,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_WHITE] = {
        .name = "White",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes white into palette.",
        .func = module_color,
        .extra_data = &moduleWhite,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_BLACK] = {
        .name = "Black",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @0@black@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlack,
        .creative = TRUE,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_TAN] = {
        .name = "Tan",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes tan into palette.",
        .func = module_color,
        .extra_data = &moduleTan,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_BROWN] = {
        .name = "Brown",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes brown into palette.",
        .func = module_color,
        .extra_data = &moduleBrown,
        .loot_tier = LOOT_VANITY,
    },

    [MOD_WOMAN] = {
        .name = "Woman",
        .type = MTYPE_VANITY,
        .tex = micons_woman_rgba16,
        .desc = "Swaps to alternative hardware + voicebox.",
        //Ryan left twitter... rip
        //edit: he's BACK
        //.desc = "Changes Mario's gender to @R@WOMAN@@.",
        .func = module_woman,
        .creative = TRUE,
    },

    // Settings
    [MOD_60HZ] = {
        .name = "60Hz",
        .type = MTYPE_SETTINGS,
        .tex = micons_sixty_rgba16,
        .desc = "Sets maximum framerate to 60.",
        .upg_desc = NULL,
        .func = module_settings,
        .extra_data = &gGameSettings[SETTING_60HZ],
    },

    [MOD_WIDESCREEN] = {
        .name = "Widescreen",
        .type = MTYPE_SETTINGS,
        .tex = micons_wide_rgba16,
        .desc = "Changes viewing resolution to 16:9.",
        .upg_desc = NULL,
        .func = module_settings,
        .extra_data = &gGameSettings[SETTING_WIDE],
    },

    [MOD_CAMERA_COLLISION] = {
        .name = "Camera Collision",
        .type = MTYPE_SETTINGS,
        .tex = micons_camcol_rgba16,
        .desc = "Camera collides with walls.",
        .upg_desc = NULL,
        .func = module_settings,
        .extra_data = &gGameSettings[SETTING_CAMERA_COLLISION],
    },

    [MOD_AA] = {
        .name = "Anti-Aliasing",
        .type = MTYPE_SETTINGS,
        .tex = micons_aa_rgba16,
        .desc = "Enables anti-aliasing (Smooth triangles).",
        .upg_desc = NULL,
        .func = module_settings,
        .extra_data = &gGameSettings[SETTING_AA],
    },

    [MOD_NOMUSIC] = {
        .name = "Disable Music",
        .type = MTYPE_SETTINGS,
        .tex = micons_nosound_rgba16,
        .desc = "Disables in-game music, but not sound FX.",
        .upg_desc = NULL,
        .func = module_settings,
        .extra_data = &gGameSettings[SETTING_NOMUSIC],
    },

    [MOD_ICE] = {
        .name = "Ice",
        .type = MTYPE_ELEMENT,
        .tex = micons_ice_rgba16,
        .desc = "Next applicable module gets imbued with ice.",
        .cooldown = .3f,
        .func = module_element,
        .extra_data = ELEMENT_ICE,
    },
    [MOD_FLAME] = {
        .name = "Flame",
        .type = MTYPE_ELEMENT,
        .tex = micons_pow_rgba16,
        .desc = "Next applicable module gets imbued with flame.",
        .cooldown = .3f,
        .func = module_element,
        .extra_data = ELEMENT_FLAME,
    },

    [MOD_STOP] = {
        .name = "Stop",
        .type = MTYPE_LOGIC,
        .tex = micons_stop_rgba16,
        .desc = "Stops the sequence prematurely.",
        .func = module_stop,
        .creative = TRUE,
    },
    [MOD_IF] = {
        .name = "Start If Block",
        .type = MTYPE_LOGIC,
        .tex = micons_if_rgba16,
        .desc = "Executes block when...",
        .func = module_if,
        .options = ifOptions,
        .creative = TRUE,
    },
    [MOD_ENDBLOCK] = {
        .name = "End Block",
        .type = MTYPE_LOGIC,
        .tex = micons_endblock_rgba16,
        .desc = "Marks end of a block.",
        .func = NULL,
        .creative = TRUE,
    },
    [MOD_IF_FLOOR] = {
        .name = "If Grounded",
        .type = MTYPE_LOGIC,
        .tex = micons_ground_rgba16,
        .desc = "Checks if Mario is touching floor.",
        .func = module_if_floor,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },
    [MOD_IF_DOWN] = {
        .name = "If Falling",
        .type = MTYPE_LOGIC,
        .tex = micons_grav_rgba16,
        .desc = "Checks if Mario is falling.",
        .func = module_if_down,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    [MOD_IF_INPUT] = {
        .name = "If Input",
        .type = MTYPE_LOGIC,
        .tex = micons_btngen_rgba16,
        .desc = "Checks for input.",
        .func = module_if_input,
        .options = ifInputOptions,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },
    [MOD_IF_WALL] = {
        .name = "If Touching Wall",
        .type = MTYPE_LOGIC,
        .tex = micons_wall_rgba16,
        .desc = "Checks for wall touch.",
        .func = module_if_wall,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },

    //
    [MOD_SPD] = {
        .name = "Move",
        .type = MTYPE_PASSIVE,
        .tex = micons_spd_rgba16,
        .desc = "Makes Mario walk forward.",
        .func = module_passive_effect,
        .extra_data = PASSIVE_FLAG_RUN,
        .creative = TRUE,
    },
    [MOD_HOLD] = {
        .name = "Hold",
        .type = MTYPE_PASSIVE,
        .tex = micons_btngen_rgba16,
        .desc = "Always makes Mario jump as high as possible.",
        .func = module_passive_effect,
        .extra_data = PASSIVE_FLAG_HOLD,
        .creative = TRUE,
    },
    [MOD_DEFENSE] = {
        .name = "Defense",
        .type = MTYPE_PASSIVE,
        .tex = micons_def_rgba16,
        .desc = "Reduces damage taken by 25*.",
        .func = module_passive_effect,
        .extra_data = PASSIVE_FLAG_DEFENSE,
        .cooldown = .5f,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_2,
    },
    [MOD_ROTATE] = {
        .name = "Rotate",
        .type = MTYPE_MOVE,
        .tex = micons_repeat_rgba16,
        .desc = "Rotates Mario.",
        //.upg_desc = "Increases rotate speed.",
        .func = module_rotate,
        .options = rotateOptions,
        .creative = TRUE,
        .loot_tier = LOOT_TIER_1,
    },
};