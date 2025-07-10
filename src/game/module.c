#include "sm64.h"
#include "module.h"
#include "puppyprint.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "level_update.h"
#include "actors/group0.h"
#include "game/segment2.h"
#include "audio/external.h"
#include "engine/math_util.h"
#include "mario.h"
#include "emutest.h"
#include "behavior_data.h"
#include "actors/group0.h"
#include "area.h"
#include "sram.h"
#include <PR/os_internal_reg.h>

u8 gModuleMenuOpen = FALSE;
u8 gGameSettings[SETTING_COUNT];
Vec3f gModulePreviewPos;
u8 gModuleUpdateVanity = FALSE;

struct module_execution_thread module_execution_threads[MODULE_EXEC_COUNT];

void module_jump(struct module_execution_thread * met, u8 call_context) {
    gMarioState->input |= INPUT_A_PRESSED;
    gMarioState->actionMod = 0;
    
    met->jump_tier = (met->jump_tier+met->mod)%3;
    switch(met->jump_tier) {
        case 0:
            break;
        case 1:
            gMarioState->actionMod = ACT_DOUBLE_JUMP;
            break;
        default:
            gMarioState->actionMod = ACT_TRIPLE_JUMP;
            if (gMarioState->flags & MARIO_WING_CAP) {
                gMarioState->actionMod = ACT_FLYING_TRIPLE_JUMP;
            }
            break;
    }
    met->jump_tier = (met->jump_tier+1)%3;
    met->mod = 0;

    gMarioState->forwardVel += 10.0f*met->spd;
    met->spd = 0;

    met->x++;
}

void module_attack(struct module_execution_thread * met, u8 call_context) {

    gMarioState->input |= INPUT_B_PRESSED;

    gMarioState->forwardVel += 10.0f*met->spd;
    met->spd = 0;

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
            if (((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING)) {
                met->halted = FALSE;
                met->x++;
                break;
            }
            break;
    }
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
            if ((gMarioState->vel[1] < 0.0f)||(((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING))) {
                met->halted = FALSE;
                met->x++;
                break;
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
    [MOD_BUTTON_A] = {MTYPE_INPUT,micons_abtn_rgba16,NULL,NULL,NULL},
    [MOD_BUTTON_B] = {MTYPE_INPUT,micons_bbtn_rgba16,NULL,NULL,NULL},
    [MOD_VANITY] = {MTYPE_INPUT,micons_vanity_rgba16,NULL,NULL,NULL},
    [MOD_SETTINGS] = {MTYPE_INPUT,micons_gear_rgba16,NULL,NULL,NULL},
    [MOD_WRAP] = {MTYPE_INPUT,micons_wrap_rgba16,NULL,NULL,NULL},

    // Actions
    [MOD_JUMP] = {MTYPE_MOVE,micons_jump_rgba16,"Makes Mario attempt to jump.","Increases jump tier per MOD.",module_jump},
    [MOD_ATTACK] = {MTYPE_MOVE,micons_pow_rgba16,"Makes Mario attempt to attack.",NULL,module_attack},
    [MOD_INPUT] = {MTYPE_COND,micons_btngen_rgba16,"Checks for a button press for one second.",NULL,module_input},
    [MOD_PLATFORM] = {MTYPE_MOVE,micons_hover_rgba16,"Mario hovers for one second. Can jump.","Extend hover time by 1/2.",module_platform},
    [MOD_SWAP] = {MTYPE_MOVE,micons_swap_rgba16,"Toggles swap platforms.",NULL,NULL},
    [MOD_CAP] = {MTYPE_MOVE,micons_cap_rgba16,"Enables cap power for one second.","0:Vanish, 1:Metal, 2:Wing.",module_cap},
    [MOD_GRAPPLE] = {MTYPE_MOVE,micons_grapple_rgba16,"Launches a grapple hook. Must hit wood.",NULL,NULL},

    // Modifiers
    [MOD_POW] = {MTYPE_BUFF,micons_onepow_rgba16,"Adds 1 to the MOD of the next piece.",NULL,module_pow},
    [MOD_REPEAT] = {MTYPE_BUFF,micons_repeat_rgba16,"Repeats from the start.",NULL,module_repeat},
    [MOD_SPD] = {MTYPE_BUFF,micons_spd_rgba16,"Adds speed to next action block.",NULL,module_spd},
    [MOD_TIMER] = {MTYPE_BUFF,micons_clock_rgba16,"Continues after a 1/2 second.","Adds 1/3 a second per MOD.",module_timer},

    // Conditions
    [MOD_HIT_GROUND] = {MTYPE_COND,micons_ground_rgba16,"Continues when Mario touches the ground.",NULL,module_floor},
    [MOD_HIT_WALL] = {MTYPE_COND,micons_wall_rgba16,"Continues when Mario touches a wall.",NULL,module_wall},
    [MOD_GRAV] = {MTYPE_COND,micons_grav_rgba16,"Continues when Mario has downward velocity.",NULL,module_grav},

    // Non modifiers
    [MOD_NONMOD_KEY] = {MTYPE_NONMOD, micons_key_rgba16,NULL,NULL,NULL},

    // Vanity
    [MOD_VAN_CAP] = {MTYPE_VANITY,micons_cap_rgba16,"Mixes colors into cap & shirt.",NULL,module_clothes_color,capLights},
    [MOD_VAN_PANTS] = {MTYPE_VANITY,micons_pants_rgba16,"Mixes colors into overalls.",NULL,module_clothes_color,jeanLights},
    [MOD_VAN_HAIR] = {MTYPE_VANITY,micons_hair_rgba16,"Mixes colors into hair.",NULL,module_clothes_color,hairLights},
    [MOD_VAN_SKIN] = {MTYPE_VANITY,micons_skin_rgba16,"Mixes colors into skin tone.",NULL,module_clothes_color,skinLights},

    [MOD_RED] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes red into palette.",NULL,module_color,&moduleRed},
    [MOD_BLUE] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes blue into palette.",NULL,module_color,&moduleBlue},
    [MOD_GREEN] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes green into palette.",NULL,module_color,&moduleGreen},
    [MOD_YELLOW] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes yellow into palette.",NULL,module_color,&moduleYellow},
    [MOD_WHITE] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes white into palette.",NULL,module_color,&moduleWhite},
    [MOD_BLACK] = {MTYPE_VANITY,micons_btngen_rgba16,"Mixes black into palette.",NULL,module_color,&moduleBlack},
    [MOD_WOMAN] = {MTYPE_VANITY,micons_woman_rgba16,"Changes Mario's gender to woman.",NULL,module_woman,NULL},
    
    // Settings
    [MOD_60HZ] = {MTYPE_SETTINGS,micons_sixty_rgba16,"Sets maximum framerate to 60.",NULL,module_settings,&gGameSettings[SETTING_60HZ]},
    [MOD_WIDESCREEN] = {MTYPE_SETTINGS,micons_wide_rgba16,"Changes viewing resolution to 16:9.",NULL,module_settings,&gGameSettings[SETTING_WIDE]},
    [MOD_CAMERA_COLLISION] = {MTYPE_SETTINGS,micons_camcol_rgba16,"Camera collides with walls.",NULL,module_settings,&gGameSettings[SETTING_CAMERA_COLLISION]},
    [MOD_AA] = {MTYPE_SETTINGS,micons_aa_rgba16,"Enables anti-aliasing (Smooth triangles).",NULL,module_settings,&gGameSettings[SETTING_AA]},
};

struct module_type_info module_type_infos[] = {
    [MTYPE_MOVE] = {"<COL_6464F0FF>Action",{0x64, 0x64, 0xF0}},
    [MTYPE_BUFF] = {"<COL_C80000FF>Modifier",{200, 0, 0}},
    [MTYPE_COND] = {"<COL_00AA00FF>Condition",{0, 170, 0}},
    [MTYPE_INPUT] = {"Input",{0xC9, 0x82, 0x30}},
    [MTYPE_NONMOD] = {NULL,{0x00, 0x00, 0x00}},
    [MTYPE_VANITY] = {"<COL_D381FCFF>Vanity",{0xD3,0x81,0xFC}},
    [MTYPE_SETTINGS] = {"<COL_AAAAAAFF>Option",{0xAA,0xAA,0xAA}},
};

#define INVENTORY_PRINT_OFFSET_X 26
#define INVENTORY_PRINT_OFFSET_Y 24


// INVENTORY STRUCTURE DECLARATIONS
#define INVENTORY_PANEL_CT 3
struct module_panel module_panel_info[] = {
    [PANEL_SETTINGS] = {
        .name = "Settings",
        .offset = 48,
        .size = 2,
        .unlock_flag = -1,
    },
    [PANEL_ACTIONS] = {
        .name = "Actions",
        .offset = 0,
        .size = 5,
        .unlock_flag = -1,
    },
    [PANEL_VANITY] = {
        .name = "Vanity",
        .offset = 44,
        .size = 4,
        .unlock_flag = -1,
    },
};

struct inventory_row inventory_row_info[INVENTORY_SLOTS_Y] = {
     // Actions
    [0] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_A, .whitelist_flags = WHITELIST_ACTION},
    [1] = {.type = ROW_SOCKET, .icon = MOD_BUTTON_B, .whitelist_flags = WHITELIST_ACTION},
    [2] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [3] = {.type = ROW_STORAGE, .mod_type_prio = -1},
    [4] = {.type = ROW_STORAGE, .mod_type_prio = -1},

     // Vanity
    [44] = {.type = ROW_SOCKET, .icon = MOD_VANITY, .whitelist_flags = WHITELIST_VANITY, .wrap = 1},
    [45] = {.type = ROW_SOCKET, .icon = MOD_WRAP,   .whitelist_flags = WHITELIST_VANITY},
    [46] = {.type = ROW_STORAGE, .mod_type_prio = MTYPE_VANITY},
    [47] = {.type = ROW_STORAGE, .mod_type_prio = MTYPE_VANITY},

     // Settings
    [48] = {.type = ROW_SOCKET, .icon = MOD_SETTINGS, .whitelist_flags = (1 << MTYPE_SETTINGS)},
    [49] = {.type = ROW_STORAGE, .mod_type_prio = -1},
};

s8 inventory[INVENTORY_SLOTS_Y][INVENTORY_SLOTS_X];

int inventory_x = 0;
int inventory_y = 0;

f32 inventory_vis_x = 0.0f;
f32 inventory_vis_y = 0.0f;

s8 module_in_hand = MOD_EMPTY;

struct module_panel * icp = &module_panel_info[PANEL_ACTIONS];
int inventory_panel = PANEL_ACTIONS;

s32 is_inventory_slot_locked(int x, int y) {
    return FALSE;
    switch(y) {
        case 0://a
            if (x >= 1+(gMarioState->numStars*2)) {return TRUE;}
            break;
        case 1://b
            if (x >= -1+gMarioState->numStars) {return TRUE;}
            break;
    }
    return FALSE;
}

s8 get_inventory(int x, int y) {
    if ((x >= INVENTORY_SLOTS_X)||(x < 0)||(y >= INVENTORY_SLOTS_Y)||(y < 0)) {
        return MOD_EMPTY;
    }
    return inventory[y][x];
}

void add_inventory(s8 module) {
    // Check for specialized inventory slots first, before
    for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
        for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
            if (inventory[y][x] == MOD_EMPTY && inventory_row_info[y].type == ROW_STORAGE && inventory_row_info[y].mod_type_prio == module_infos[module].type) {
                inventory[y][x] = module;
                return;
            }
        }
    }

    for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
        for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
            if (inventory[y][x] == MOD_EMPTY && inventory_row_info[y].type == ROW_STORAGE) {
                inventory[y][x] = module;
                return;
            }
        }
    }
}

void init_module_inventory(void) {
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
            inventory[y][x] = MOD_EMPTY;
        }
    }

    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        module_execution_threads[i].executing = FALSE;
    }

    // Settings
    inventory[48][0] = MOD_CAMERA_COLLISION;
    inventory[49][0] = MOD_WIDESCREEN;
    if (gEmulator & EMU_CONSOLE) {
        // N64 specific configuration
        inventory[49][1] = MOD_60HZ;
    } else {
        inventory[48][1] = MOD_60HZ;
    }

    inventory[4][0] = MOD_REPEAT;
    inventory[3][0] = MOD_REPEAT;
    inventory[2][0] = MOD_JUMP;
    inventory[2][1] = MOD_JUMP;
    inventory[2][2] = MOD_PLATFORM;
    inventory[2][3] = MOD_TIMER;
    inventory[2][4] = MOD_TIMER;
    inventory[2][5] = MOD_INPUT;
    inventory[2][6] = MOD_INPUT;
    inventory[2][7] = MOD_VAN_CAP;
    inventory[4][7] = MOD_VAN_PANTS;
    inventory[4][6] = MOD_VAN_PANTS;

    inventory[4][5] = MOD_RED;
    inventory[4][4] = MOD_BLUE;
    inventory[4][3] = MOD_GREEN;
    inventory[4][2] = MOD_RED;
    inventory[4][1] = MOD_GREEN;
    inventory[4][0] = MOD_BLUE;
    inventory[3][1] = MOD_YELLOW;
    inventory[3][2] = MOD_BLACK;
    inventory[3][3] = MOD_WHITE;
    inventory[3][4] = MOD_VAN_HAIR;
    inventory[3][5] = MOD_VAN_SKIN;
    inventory[3][6] = MOD_WOMAN;

    load_marios_modules();
    update_settings();
}

void module_update(void) {
    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->cooldown) {
            if (met->timer >= 5) {
                if (met->manual) {
                    play_sound(SOUND_MENU_MESSAGE_DISAPPEAR,gGlobalSoundSource);
                }
                met->executing = FALSE;
                met->cooldown = FALSE;
            }
            if (
                (((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING))
                && (count_objects_with_behavior(bhvHover) == 0) 
                && (gMarioState->capTimer == 0)
            ) {
                met->timer++;
            }
        } else if (met->executing) {
            s8 read_mod = get_inventory(met->x,met->y);
            if (!met->halted) {
                while(read_mod != MOD_EMPTY) {
                    if (1 << module_infos[read_mod].type & inventory_row_info[met->y].whitelist_flags) {
                        met->extra_data = module_infos[read_mod].extra_data;
                        module_infos[read_mod].func(met,MCC_INVOKE);



                        if (inventory_row_info[met->y].wrap && met->x == 8) {
                            met->x = 0;
                            met->y ++;
                        }

                        read_mod = get_inventory(met->x,met->y);
                        if (met->halted) {
                            return;
                        }
                    } else {
                        // module incompatible with row, NOP
                        met->x++;
                        read_mod = get_inventory(met->x,met->y);
                    }
                }
                if (met->manual) {
                    met->cooldown = TRUE;
                } else {
                    met->executing = FALSE;
                }
                met->timer = 0;

                if (gModuleUpdateVanity && i != MODULE_EXEC_VANITY) {
                    gModuleUpdateVanity = FALSE;
                    update_vanity();
                }
            } else {
                module_infos[read_mod].func(met,MCC_HALTED);
                met->timer++;
            }
        }
    }
}

void execute_module_in_inventory(struct module_execution_thread * met, u32 input, int x, int y, int manual) {
    if (!met->executing) {
        met->mod = 0;
        met->spd = 0;
        met->x = x;
        met->y = y;
        met->timer = 0;
        met->executing = TRUE;
        met->halted = FALSE;
        met->input = input;
        met->cooldown = FALSE;
        met->jump_tier = 0;
        met->input_notify = FALSE;
        met->used_flags = 0;
        met->extra_data = NULL;
        met->manual = manual;
        colorBlendCount = 0;

        if (manual) {
            play_sound(SOUND_MENU_MESSAGE_APPEAR,gGlobalSoundSource);
        }
    }
}

s32 handle_module_inputs(void) {
    if (!gModuleMenuOpen) {
        if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_A],A_BUTTON,0,0,TRUE);
        }
        if (gPlayer1Controller->buttonPressed & B_BUTTON) {
            execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_B],B_BUTTON,0,1,TRUE);
        }
    }
    return FALSE;
}

void update_vanity(void) {
    gMarioState->marioObj->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_MARIO];
    execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_VANITY],0,0,44,FALSE);
}

void update_settings(void) {
    for (int i = 0; i < SETTING_COUNT; i++) {
        gGameSettings[i] = 0;
    }
    execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_SETTINGS],0,0,48,FALSE);
}

#define ANALOG_MENU_THRESH 30
u16 joystick_hold_timer = 0;
void control_module_menu(void) {
    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->executing && met->manual) {
            //no editing while running
            return;
        }
    }

    //handle joystick
    if (
        (gPlayer1Controller->rawStickY < ANALOG_MENU_THRESH) &&
        (gPlayer1Controller->rawStickY > -ANALOG_MENU_THRESH) &&
        (gPlayer1Controller->rawStickX < ANALOG_MENU_THRESH) &&
        (gPlayer1Controller->rawStickX > -ANALOG_MENU_THRESH)
    ) {
        joystick_hold_timer = 0;
    } else {
        joystick_hold_timer++;
    }
    if (joystick_hold_timer==1 || (joystick_hold_timer>15&&(gGlobalTimer%4==0))) {
        if (gPlayer1Controller->rawStickY > ANALOG_MENU_THRESH) {
            gPlayer1Controller->buttonPressed |= U_JPAD;
        }
        if (gPlayer1Controller->rawStickY < -ANALOG_MENU_THRESH) {
            gPlayer1Controller->buttonPressed |= D_JPAD;
        }
        if (gPlayer1Controller->rawStickX > ANALOG_MENU_THRESH) {
            gPlayer1Controller->buttonPressed |= R_JPAD;
        }
        if (gPlayer1Controller->rawStickX < -ANALOG_MENU_THRESH) {
            gPlayer1Controller->buttonPressed |= L_JPAD;
        }
    }



    if (gPlayer1Controller->buttonPressed & L_JPAD) {
        inventory_x --;
    }
    else if (gPlayer1Controller->buttonPressed & R_JPAD) {
        inventory_x ++;
    }
    else if (gPlayer1Controller->buttonPressed & D_JPAD) {
        inventory_y ++;
    } else if (gPlayer1Controller->buttonPressed & U_JPAD) {
        inventory_y --;
    }

    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        inventory_panel++;
    }
    if (gPlayer1Controller->buttonPressed & L_TRIG) {
        inventory_panel--;
    }
    inventory_panel = (INVENTORY_PANEL_CT+inventory_panel)%INVENTORY_PANEL_CT;

    icp = &module_panel_info[inventory_panel];

    inventory_x = (INVENTORY_SLOTS_X+inventory_x)%INVENTORY_SLOTS_X;
    inventory_y = (icp->size+inventory_y)%icp->size;

    int true_inventory_y = inventory_y + icp->offset;

    if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        if (is_inventory_slot_locked(inventory_x,inventory_y)) {
            play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundSource);
        } else {
            if (!(module_in_hand == MOD_EMPTY && inventory[true_inventory_y][inventory_x] == MOD_EMPTY)) {
                play_sound(SOUND_MENU_CLICK_FILE_SELECT, gGlobalSoundSource);
            }

            s16 module_to_pick_up = inventory[true_inventory_y][inventory_x];
            inventory[true_inventory_y][inventory_x] = module_in_hand;
            module_in_hand = module_to_pick_up;
        }
        if (true_inventory_y == 44 || true_inventory_y == 45) {
            update_vanity();
        }
        if (true_inventory_y == 48) {
            update_settings();
        }
    }
}

void print_texture(void * tex, int size, int x, int y) {
    gDPPipeSync(gDisplayListHead++);
    gDPLoadTextureBlock(gDisplayListHead++, tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, size,size, 0, 0, 0, 0, 0, 0, 0);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + (size)) << 2,
                        (y + (size)) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
}

u8 gPrintModuleDarken=1;
void print_module(int id, int x, int y) {
    if (id == MOD_EMPTY) return;
    u8 r = module_type_infos[module_infos[id].type].color[0]/gPrintModuleDarken;
    u8 g = module_type_infos[module_infos[id].type].color[1]/gPrintModuleDarken;
    u8 b = module_type_infos[module_infos[id].type].color[2]/gPrintModuleDarken;
    gDPSetEnvColor(gDisplayListHead++, r,g,b, 255);
    if (module_infos[id].type != MTYPE_NONMOD) {
        print_texture(micons_piece_rgba16,32,x,y);
    }
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    if (module_infos[id].func == module_color) {
        // Terry davis would have a select few words for this
        gDPSetEnvColor(gDisplayListHead++,
            ((f32 *)module_infos[id].extra_data)[0]*255.0f,
            ((f32 *)module_infos[id].extra_data)[1]*255.0f,
            ((f32 *)module_infos[id].extra_data)[2]*255.0f, 255);
    }
    print_texture(module_infos[id].tex,16,x,y);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
}

int inv_slot_printx(int x, int y) {
    if (inventory_row_info[y+icp->offset].type == ROW_SOCKET) {
        return x*16+INVENTORY_PRINT_OFFSET_X+22;
    }
    return x*21+INVENTORY_PRINT_OFFSET_X;
}
int inv_slot_printx_w(int x, int y) {
    if (inventory_row_info[y+icp->offset].type == ROW_SOCKET) {
        return inv_slot_printx(x,y)+16;
    }
    return inv_slot_printx(x,y)+20;
}
int inv_slot_printy(int x, int y) {
    if (y > 1) {
        return y*18+INVENTORY_PRINT_OFFSET_Y;
    }
    return y*17+INVENTORY_PRINT_OFFSET_Y;
}

int module_is_invalid(int x, int y, s8 mod) {
    // Blank spaces are never invalid
    if (mod == -1) {
        return FALSE;
    }

    // Missing a module to the left? Is invalid!
    if (x != 0 && get_inventory(x-1,y) == -1 && inventory_row_info[y].type == ROW_SOCKET) {
        return TRUE;
    }

    // Not on the socket whitelist? Is invalid!
    if (inventory_row_info[y].type == ROW_SOCKET && !(1 << module_infos[mod].type & inventory_row_info[y].whitelist_flags)) {
        return TRUE;
    }

    return FALSE;
}


char print_buffer[500];
void print_module_menu(void) {
    gSPDisplayList(gDisplayListHead++,ui_ui_mesh);

    gPrintModuleDarken=1;
    inventory_vis_x = approach_f32_asymptotic(inventory_vis_x,inv_slot_printx(inventory_x,inventory_y),.3f);
    inventory_vis_y = approach_f32_asymptotic(inventory_vis_y,inv_slot_printy(inventory_x,inventory_y),.3f);

    prepare_blank_box();
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 0; y<icp->size; y++) {
            u8 brightness = 10;
            u8 alpha = 150;
            if (x == inventory_x && y == inventory_y) {
                brightness = 200;
                alpha = 150;
            }
            render_blank_box_rounded(inv_slot_printx(x,y), inv_slot_printy(x,y),
            inv_slot_printx_w(x,y), inv_slot_printy(x,y)+16,
            brightness, brightness, brightness, alpha);
        }
    }
    finish_blank_box();

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 0; y<icp->size; y++) {
            int true_y = y + icp->offset;

            int invalid = module_is_invalid(x,true_y,get_inventory(x,true_y));
            if (invalid) {
                gPrintModuleDarken=2;
            }
            print_module(inventory[true_y][x],inv_slot_printx(x,y), inv_slot_printy(x,y));
            if (invalid) {
                print_texture(micons_warn_rgba16,16,inv_slot_printx(x,y), inv_slot_printy(x,y));
                gPrintModuleDarken=1;
            }

            if (is_inventory_slot_locked(x,y)) {
                gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
                print_texture(micons_lock_rgba16,16,inv_slot_printx(x,y), inv_slot_printy(x,y));
                gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
            }

            if (inventory_row_info[true_y].type == ROW_SOCKET) {
                print_module(inventory_row_info[true_y].icon,inv_slot_printx(-1,y), inv_slot_printy(-1,y));
                if (inventory_row_info[true_y].wrap) {
                    print_module(MOD_WRAP,inv_slot_printx(8,y), inv_slot_printy(-1,y));
                }
            }
        }
    }

    //PRINT HAND and GRAB
    print_module(module_in_hand,inventory_vis_x,inventory_vis_y);
    void * hand_tex = micons_small_hand_1_rgba16;
    if (module_in_hand != MOD_EMPTY) {
        hand_tex = micons_small_hand_2_rgba16;
    }
    print_texture(hand_tex,16,inventory_vis_x+8, inventory_vis_y+8);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    //PRINT MOD INFO
    s8 mod_inf_to_disp = MOD_EMPTY;

    if (module_in_hand != MOD_EMPTY) {
        mod_inf_to_disp = module_in_hand;
    } else {
        mod_inf_to_disp = inventory[inventory_y + icp->offset][inventory_x];
    }

    if (mod_inf_to_disp != MOD_EMPTY) {
        print_set_envcolour(255, 255, 255, 255);
        sprintf(print_buffer, "%s",module_type_infos[module_infos[mod_inf_to_disp].type].name);
        sprintf(print_buffer, "%s:<COL_FFFFFFFF> %s\n",print_buffer,module_infos[mod_inf_to_disp].desc);
        if (module_infos[mod_inf_to_disp].mod_desc != NULL) {
            sprintf(print_buffer, "%s<COL_FF0000FF>MOD Bonus: <COL_FFFFFFFF>%s",print_buffer,module_infos[mod_inf_to_disp].mod_desc);
        }

        f32 tooltip_x = 15;
        f32 tooltip_y = 170;

        /*
        if (tooltip_x + get_text_width(print_buffer,FONT_VANILLA) > 320) {
            tooltip_x = 320 - get_text_width(print_buffer,FONT_VANILLA);
        }
        */

        prepare_blank_box();
        render_blank_box_rounded(tooltip_x,tooltip_y,
        //tooltip_x+get_text_width(print_buffer,FONT_VANILLA),inventory_vis_y+16+get_text_height(print_buffer),
        tooltip_x + 295, tooltip_y + 30,
        0,0,0,200);
        finish_blank_box();

        print_small_text(tooltip_x+4, tooltip_y+4, print_buffer, PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
    }
}

u8 world_module_timer = 0;
Vec3f world_module_pos;
s8 world_module_id = -1;

#define MODULE_HUD_STATUS_Y 205
void print_module_hud_status(void) {
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    print_module(MOD_BUTTON_A,22,MODULE_HUD_STATUS_Y);
    if (module_execution_threads[MODULE_EXEC_A].executing) {
        print_texture(micons_executing_rgba16,16 ,22,MODULE_HUD_STATUS_Y);
    }
    if (module_execution_threads[MODULE_EXEC_A].input_notify) {
        print_texture(micons_inpnotif_rgba16,16 ,22,MODULE_HUD_STATUS_Y);
    }
    print_module(MOD_BUTTON_B,42,MODULE_HUD_STATUS_Y);
    if (module_execution_threads[MODULE_EXEC_B].executing) {
        print_texture(micons_executing_rgba16,16 ,42,MODULE_HUD_STATUS_Y);
    }
    if (module_execution_threads[MODULE_EXEC_B].input_notify) {
        print_texture(micons_inpnotif_rgba16,16 ,42,MODULE_HUD_STATUS_Y);
    }

    if (world_module_id != -1) {
        s32 x;
        s32 y;

        world_pos_to_screen_pos(&world_module_pos,&x,&y);
        print_module(world_module_id,x-8,y-world_module_timer);

        if (world_module_timer > 30) {
            world_module_id = -1;
        }
        world_module_timer++;
    }


    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

char * changelog = "\
Major Changes:\n\
* Increased max framerate to 60\n\
* Overhauled and refined module menu\n\
* Added vanity, settings, and progress panels\n\
* Added game saving via save blocks\n\
\n\
New Module Additions:\n\
* Red Dye (Vanity)\n\
* Green Dye (Vanity)\n\
* Blue Dye (Vanity)\n\
* Yellow Dye (Vanity)\n\
* Black Dye (Vanity)\n\
* White Dye (Vanity)\n\
* Pants (Vanity)\n\
* Cap & Shirt (Vanity)\n\
* Skin (Vanity)\n\
* Hair (Vanity)\n\
* Woman (Vanity)\n\
\n\
* 60 FPS (Option)\n\
* Camera Collision (Option)\n\
* Widescreen (Option)\n\
\n\
Minor Changes:\n\
* Polished level visuals\n\
* Added module warnings\n\
* Fixed thwomp death softlock\n\
* Hold to navigate menus added\n\
* Shortened module cooldown\n\
* Timer module now classified as modifier rather than condition";

struct mariosModulesSave sMariosModulesSave;

void save_marios_modules(Vec3f pos) {
    int size = sizeof(struct mariosModulesSave);

    if (gSramProbe != 0) {
        sMariosModulesSave.version = MARIOS_MODULES_GAME_VERSION;
        for (int i = 0; i < 3; i++) {
            sMariosModulesSave.pos[i] = pos[i];
        }
        sMariosModulesSave.save_magic = SAVE_MAGIC;
        bcopy(&inventory,&sMariosModulesSave.inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        s32 status = nuPiWriteSram(0, &sMariosModulesSave, ALIGN8(size));
    }

}

void load_marios_modules(void) {
    int size = sizeof(struct mariosModulesSave);

    if (gSramProbe != 0) {
        s32 status = nuPiReadSram(0, &sMariosModulesSave, ALIGN8(size));
        if (sMariosModulesSave.save_magic == SAVE_MAGIC) {
            bcopy(&sMariosModulesSave.inventory,&inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        }
    }
}

void marios_modules_savefile_load_position(void) {
    if (sMariosModulesSave.save_magic == SAVE_MAGIC) {
        for (int i = 0; i < 3; i++) {
            gMarioState->pos[i] = sMariosModulesSave.pos[i];
        }
    }
}