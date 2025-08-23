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
                set_mario_action(gMarioState,ACT_JUMP,0);
                break;
            case 1:
                set_mario_action(gMarioState,ACT_DOUBLE_JUMP,0);
                break;
            default:
                set_mario_action(gMarioState,ACT_TRIPLE_JUMP,0);
                if (gMarioState->flags & MARIO_WING_CAP) {
                    set_mario_action(gMarioState,ACT_FLYING_TRIPLE_JUMP,0);
                }
                break;
        }
        met->mod = 0;
    } else {
        gMarioState->input |= INPUT_A_PRESSED;
    }

    met->x++;
}

void module_tornado(struct module_execution_thread * met, u8 call_context) {
    if (!(GROUNDED)) {
        met->doaircooldown = TRUE;
        set_mario_action(gMarioState,ACT_TWIRLING,0);
    }
    met->x++;
}

void module_attack(struct module_execution_thread * met, u8 call_context) {
    met->doaircooldown = TRUE;
    gMarioState->input |= INPUT_B_PRESSED;

    gMarioState->forwardVel += 10.0f*met->spd;
    met->spd = 0;

    met->x++;
}

void module_zaction(struct module_execution_thread * met, u8 call_context) {
    met->doaircooldown = TRUE;
    if (GROUNDED) {
        set_mario_action(gMarioState,ACT_LONG_JUMP,0);
    } else {
        if (gMarioState->action != ACT_GROUND_POUND) {
            set_mario_action(gMarioState,ACT_GROUND_POUND,0);
        }
    }
    met->x++;
}


void module_pow(struct module_execution_thread * met, u8 call_context) {
    met->mod++;
    met->x++;
}

void module_spd(struct module_execution_thread * met, u8 call_context) {
    met->spd++;
    met->x++;
}

void module_repeat(struct module_execution_thread * met, u8 call_context) {
    if (met->used_flags & (1 << met->x)) {
        met->x++;
    } else {
        met->used_flags |= (1 << met->x);
        met->x=0;
    }
}

void module_floor(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            break;
        case MCC_HALTED:
            if (GROUNDED) {
                met->halted = FALSE;
                met->x++;
                break;
            }
            break;
    }
}

void module_floor_upg(struct module_execution_thread * met, u8 call_context) {
    met->x++;
    met->mod+=met->landing_count;
}

void module_wall(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            met->timer = 0;
            break;
        case MCC_HALTED:
            if (gMarioState->wall != NULL) {
                met->halted = FALSE;
                met->x++;
                break;
            }
            if (!(((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING))) {
                met->timer = 0;
                break;
            }
            if (met->timer >= 15) {
                met->timer = 0;
                met->cooldown = TRUE;
            }
            break;
    }
}

void module_timer(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            met->halted = TRUE;
            met->timer = 0;
            break;
        case MCC_HALTED:
            if (met->timer >= 15 + (10*met->mod)) {
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
            break;
        case MCC_HALTED:
            if (gMarioState->vel[1] < 0.0) {
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
            }
            break;
    }
}

void module_input(struct module_execution_thread * met, u8 call_context) {
    switch(call_context) {
        case MCC_INVOKE:
            play_sound(SOUND_GENERAL_BOWSER_KEY_LAND, gGlobalSoundSource);
            if (gPlayer1Controller->buttonPressed & met->input) {
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
                met->halted = FALSE;
                met->input_notify = FALSE;
                met->x++;
                break;
            }
            if (met->timer >= 30) {
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
    gMarioState->capTimer += 30;
    switch(met->mod) {
        case 0:
            gMarioState->flags |= MARIO_VANISH_CAP;
            break;
        case 1:
            gMarioState->flags |= MARIO_METAL_CAP;
            break;
        default:
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

    gModuleUpdateVanity = TRUE;
    met->x++;
}

void module_color(struct module_execution_thread * met, u8 call_context) {
    vec3f_copy(colorBlendStack[colorBlendCount],*((Vec3f *)met->extra_data));
    colorBlendCount++;

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
    met->doaircooldown = TRUE;
    gMarioState->vel[1] = -gMarioState->vel[1];
    met->x++;
}

void module_if(struct module_execution_thread * met, u8 call_context) {
    if (met->ifbool) {
        met->x++;
        met->ifbool = FALSE;
    } else {
        u8 revertX = met->x+1;
        while(get_inventory(met->x,met->y) != MOD_ENDBLOCK) {
            met->x++;
            if (met->x >= INVENTORY_SLOTS_X) {
                met->x = revertX;
                break;
            }
        }
    }
}

void module_if_floor(struct module_execution_thread * met, u8 call_context) {
    if (GROUNDED) {
        met->ifbool = TRUE;
    } else {
        met->ifbool = FALSE;
    }
    met->x++;
}

void module_if_down(struct module_execution_thread * met, u8 call_context) {
    if (gMarioState->vel[1] < 0.0f) {
        met->ifbool = TRUE;
    } else {
        met->ifbool = FALSE;
    }
    met->x++;
}

void module_stop(struct module_execution_thread * met, u8 call_context) {
    met->x = INVENTORY_SLOTS_X; // OOB = MOD_EMPTY
}

Vec3f moduleRed = {1.0f,0.0f,0.0f};
Vec3f moduleBlue = {0.0f,0.0f,1.0f};
Vec3f moduleGreen = {0.0f,1.0f,0.0f};
Vec3f moduleYellow = {1.0f,1.0f,0.0f};
Vec3f moduleWhite = {1.0f,1.0f,1.0f};
Vec3f moduleBlack = {0.02f,0.02f,0.02f};

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
    NULL,
};

struct module_info module_infos[] = {
    // Sockets
    [MOD_BUTTON_A] = {MTYPE_INPUT,0,0,0,"",micons_abtn_rgba16,NULL,NULL,NULL},
    [MOD_BUTTON_B] = {MTYPE_INPUT,0,0,0,"",micons_bbtn_rgba16,NULL,NULL,NULL},
    [MOD_VANITY] = {MTYPE_INPUT,0,0,0,"",micons_vanity_rgba16,NULL,NULL,NULL},
    [MOD_SETTINGS] = {MTYPE_INPUT,0,0,0,"",micons_gear_rgba16,NULL,NULL,NULL},
    [MOD_WRAP] = {MTYPE_INPUT,0,0,0,"",micons_wrap_rgba16,NULL,NULL,NULL},
    [MOD_PASSIVE] = {MTYPE_INPUT,0,0,0,"",micons_p__rgba16,NULL,NULL,NULL},

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
    },

    [MOD_TORNADO] = {
        .name = "Tornado",
        .type = MTYPE_MOVE,
        .tex = micons_tornado_rgba16,
        .desc = "If airborne, makes Mario spin. Rises when above heat.",
        .unchainable = TRUE,
        .elementable = TRUE,
        .func = module_tornado,
        .creative = TRUE,
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
    },

    [MOD_INPUT] = {
        .name = "Input",
        .type = MTYPE_COND,
        .tex = micons_btngen_rgba16,
        .desc = "Checks for a button press for one second, otherwise cancels.",
        .func = module_input,
        .creative = TRUE,
    },

    [MOD_PLATFORM] = {
        .name = "Air Platform",
        .type = MTYPE_MOVE,
        .tex = micons_hover_rgba16,
        .desc = "Spawns a temporary air platform for one second.",
        .upg_desc = "Time +1/2s per @O@UPG@@.",
        .unchainable = TRUE,
        .func = module_platform,
        .creative = TRUE,
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
        .desc = "Enables cap power for one second.",
        .upg_desc = "0:Vanish, 1:Metal, 2:Wing.",
        .cooldown = 3.0f,
        .func = module_cap,
        .creative = TRUE,
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
    },

    [MOD_ZACTION] = {
        .name = "Crouch Move",
        .type = MTYPE_MOVE,
        .tex = micons_ground_rgba16,
        .desc = "Ground pound in air, long jump on floor.",
        .unchainable = TRUE,
        .func = module_zaction,
        .creative = TRUE,
    },

    // Modifiers
    [MOD_POW] = {
        .name = "+1 Upgrade",
        .type = MTYPE_BUFF,
        .tex = micons_onepow_rgba16,
        .desc = "+1@O@UPG@@ to the next piece.",
        .cooldown = .5f,
        .func = module_pow,
        .creative = TRUE,
    },

    [MOD_GROUND_UPG] = {
        .name = "Ground Upgrade",
        .type = MTYPE_BUFF,
        .tex = micons_ground_rgba16,
        .desc = "@O@UPG@@+ times landed during sequence.",
        .unchainable = TRUE,
        .func = module_floor_upg,
        .creative = TRUE,
    },


    [MOD_REPEAT] = {
        .name = "Repeat",
        .type = MTYPE_COND,
        .tex = micons_repeat_rgba16,
        .desc = "Repeats from the start.",
        .func = module_repeat,
        .creative = TRUE,
    },

    [MOD_SPD] = {
        .name = "Speed",
        .type = MTYPE_BUFF,
        .tex = micons_spd_rgba16,
        .desc = "Adds speed to next action block.",
        .func = module_spd,
    },

    [MOD_TIMER] = {
        .name = "Timer",
        .type = MTYPE_COND,
        .tex = micons_clock_rgba16,
        .desc = "Continues after a 1/2 second.",
        .upg_desc = "+1/3 a second per @O@UPG@@.",
        .cooldown = -.5f,
        .func = module_timer,
        .creative = TRUE,
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
    },

    [MOD_GRAV] = {
        .name = "Down",
        .type = MTYPE_COND,
        .tex = micons_grav_rgba16,
        .desc = "Continues when Mario has downward velocity, cancels on floor.",
        .unchainable = TRUE,
        .func = module_grav,
        .creative = TRUE,
    },

    [MOD_HIT_GROUND] = {
        .name = "Ground",
        .type = MTYPE_COND,
        .tex = micons_ground_rgba16,
        .desc = "Continues when Mario touches the ground.",
        .unchainable = TRUE,
        .func = module_floor,
        .creative = TRUE,
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
    },

    [MOD_VAN_PANTS] = {
        .name = "Pants Color",
        .type = MTYPE_VANITY,
        .tex = micons_pants_rgba16,
        .desc = "Mixes colors into overalls.",
        .func = module_clothes_color,
        .extra_data = jeanLights,
        .creative = TRUE,
    },

    [MOD_VAN_HAIR] = {
        .name = "Hair Color",
        .type = MTYPE_VANITY,
        .tex = micons_hair_rgba16,
        .desc = "Mixes colors into hair.",
        .func = module_clothes_color,
        .extra_data = hairLights,
        .creative = TRUE,
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
    },

    [MOD_BLUE] = {
        .name = "Blue",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @B@blue@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlue,
        .creative = TRUE,
    },

    [MOD_GREEN] = {
        .name = "Green",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @G@green@@ into palette.",
        .func = module_color,
        .extra_data = &moduleGreen,
        .creative = TRUE,
    },

    [MOD_YELLOW] = {
        .name = "Yellow",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @Y@yellow@@ into palette.",
        .func = module_color,
        .extra_data = &moduleYellow,
        .creative = TRUE,
    },

    [MOD_WHITE] = {
        .name = "White",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes white into palette.",
        .func = module_color,
        .extra_data = &moduleWhite,
        .creative = TRUE,
    },

    [MOD_BLACK] = {
        .name = "Black",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @0@black@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlack,
        .creative = TRUE,
    },

    [MOD_WOMAN] = {
        .name = "Woman",
        .type = MTYPE_VANITY,
        .tex = micons_woman_rgba16,
        .desc = "Changes Mario's gender to @R@WOMAN@@.",
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

    [MOD_ICE] = {
        .name = "Ice",
        .type = MTYPE_ELEMENT,
        .tex = micons_ice_rgba16,
        .desc = "Next applicable module gets imbued with ice.",
        .cooldown = .3f,
        .func = module_element,
        .creative = TRUE,
        .extra_data = ELEMENT_ICE,
    },
    [MOD_FLAME] = {
        .name = "Flame",
        .type = MTYPE_ELEMENT,
        .tex = micons_pow_rgba16,
        .desc = "Next applicable module gets imbued with flame.",
        .cooldown = .3f,
        .func = module_element,
        .creative = TRUE,
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
        .desc = "Executes block if @G@condition@@ is @B@TRUE@@.",
        .func = module_if,
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
        .desc = "Sets @G@condition@@ to @B@TRUE@@ if Mario is touching floor.",
        .func = module_if_floor,
        .creative = TRUE,
    },
    [MOD_IF_DOWN] = {
        .name = "If Falling",
        .type = MTYPE_LOGIC,
        .tex = micons_grav_rgba16,
        .desc = "Sets @G@condition@@ to @B@TRUE@@ if Mario is falling.",
        .func = module_if_down,
        .creative = TRUE,
    },
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
};

#define INVENTORY_PRINT_OFFSET_X 26
#define INVENTORY_PRINT_OFFSET_Y 24


// INVENTORY STRUCTURE DECLARATIONS
struct module_panel module_panel_info[] = {
    [PANEL_SETTINGS] = {
        .name = "Settings",
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
        .offset = 5,
        .size = 5,
        .unlock = NULL,
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
        .unlock = NULL,
    },
};

struct inventory_row inventory_row_info[INVENTORY_SLOTS_Y] = {
     // Actions
    [0] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_A, .whitelist_flags = WHITELIST_ACTION},
    [1] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_B, .whitelist_flags = WHITELIST_ACTION},
    [2] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [3] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [4] = {.type = ROW_STORAGE, .mod_type_prio = -1},

     // Actions
    [5] = {.type = ROW_SOCKET, .icon = MOD_PASSIVE, .whitelist_flags = WHITELIST_ACTION, .wrap = 1},
    [6] = {.type = ROW_SOCKET, .icon = MOD_WRAP, .whitelist_flags = WHITELIST_ACTION},
    [7] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [8] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [9] = {.type = ROW_STORAGE, .mod_type_prio = -1},

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