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
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include <PR/os_internal_reg.h>
#include "utf8_print.h"

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
    [MOD_BUTTON_A] = {MTYPE_INPUT,"",micons_abtn_rgba16,NULL,NULL,NULL},
    [MOD_BUTTON_B] = {MTYPE_INPUT,"",micons_bbtn_rgba16,NULL,NULL,NULL},
    [MOD_VANITY] = {MTYPE_INPUT,"",micons_vanity_rgba16,NULL,NULL,NULL},
    [MOD_SETTINGS] = {MTYPE_INPUT,"",micons_gear_rgba16,NULL,NULL,NULL},
    [MOD_WRAP] = {MTYPE_INPUT,"",micons_wrap_rgba16,NULL,NULL,NULL},

    // Actions
    [MOD_JUMP] = {
        .name = "Jump",
        .type = MTYPE_MOVE,
        .tex = micons_jump_rgba16,
        .desc = "Makes Mario attempt to jump.",
        .upg_desc = "Jump Tier increases with @O@UPG@@.",
        .func = module_jump,
    },

    [MOD_ATTACK] = {
        .name = "Attack",
        .type = MTYPE_MOVE,
        .tex = micons_pow_rgba16,
        .desc = "Makes Mario attempt to attack.",
        .func = module_attack,
    },

    [MOD_INPUT] = {
        .name = "Input",
        .type = MTYPE_COND,
        .tex = micons_btngen_rgba16,
        .desc = "Checks for a button press for one second, otherwise cancels.",
        .func = module_input,
    },

    [MOD_PLATFORM] = {
        .name = "Hover",
        .type = MTYPE_MOVE,
        .tex = micons_hover_rgba16,
        .desc = "Mario hovers for one second. Can jump.",
        .upg_desc = "Hover time +1/2 per @O@UPG@@.",
        .func = module_platform,
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
        .cooldown = 4.0f,
        .func = module_cap,
    },

    [MOD_GRAPPLE] = {
        .name = "Grapple",
        .type = MTYPE_MOVE,
        .tex = micons_grapple_rgba16,
        .desc = "Launches a grapple hook. Must hit wood.",
    },

    // Modifiers
    [MOD_POW] = {
        .name = "UPG",
        .type = MTYPE_UPGRADE,
        .tex = micons_onepow_rgba16,
        .desc = "+1@O@UPG@@ to the next piece.",
        .cooldown = .5f,
        .func = module_pow,
    },

    [MOD_REPEAT] = {
        .name = "Repeat",
        .type = MTYPE_BUFF,
        .tex = micons_repeat_rgba16,
        .desc = "Repeats from the start.",
        .func = module_repeat,
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
        .func = module_timer,
    },

    // Conditions
    [MOD_HIT_GROUND] = {
        .name = "Ground",
        .type = MTYPE_COND,
        .tex = micons_ground_rgba16,
        .desc = "Continues when Mario touches the ground.",
        .func = module_floor,
    },

    [MOD_HIT_WALL] = {
        .name = "Wall",
        .type = MTYPE_COND,
        .tex = micons_wall_rgba16,
        .desc = "Continues when touching wall, or cancels on floor touch.",
        .func = module_wall,
    },

    [MOD_GRAV] = {
        .name = "Down",
        .type = MTYPE_COND,
        .tex = micons_grav_rgba16,
        .desc = "Continues when Mario has downward velocity.",
        .func = module_grav,
    },

    // Non modifiers
    [MOD_NONMOD_KEY] = {
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
    },

    [MOD_VAN_PANTS] = {
        .name = "Pants Color",
        .type = MTYPE_VANITY,
        .tex = micons_pants_rgba16,
        .desc = "Mixes colors into overalls.",
        .func = module_clothes_color,
        .extra_data = jeanLights,
    },

    [MOD_VAN_HAIR] = {
        .name = "Hair Color",
        .type = MTYPE_VANITY,
        .tex = micons_hair_rgba16,
        .desc = "Mixes colors into hair.",
        .func = module_clothes_color,
        .extra_data = hairLights,
    },

    [MOD_VAN_SKIN] = {
        .name = "Skin Color",
        .type = MTYPE_VANITY,
        .tex = micons_skin_rgba16,
        .desc = "Mixes colors into skin tone.",
        .func = module_clothes_color,
        .extra_data = skinLights,
    },

    [MOD_RED] = {
        .name = "Red",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @R@red@@ into palette.",
        .func = module_color,
        .extra_data = &moduleRed,
    },

    [MOD_BLUE] = {
        .name = "Blue",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @B@blue@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlue,
    },

    [MOD_GREEN] = {
        .name = "Green",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @G@green@@ into palette.",
        .func = module_color,
        .extra_data = &moduleGreen,
    },

    [MOD_YELLOW] = {
        .name = "Yellow",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @Y@yellow@@ into palette.",
        .func = module_color,
        .extra_data = &moduleYellow,
    },

    [MOD_WHITE] = {
        .name = "White",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes white into palette.",
        .func = module_color,
        .extra_data = &moduleWhite,
    },

    [MOD_BLACK] = {
        .name = "Black",
        .type = MTYPE_VANITY,
        .tex = micons_btngen_rgba16,
        .desc = "Mixes @0@black@@ into palette.",
        .func = module_color,
        .extra_data = &moduleBlack,
    },

    [MOD_WOMAN] = {
        .name = "Woman",
        .type = MTYPE_VANITY,
        .tex = micons_woman_rgba16,
        .desc = "Changes Mario's gender to @R@WOMAN@@.",
        .func = module_woman,
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
};

struct module_type_info module_type_infos[] = {
    [MTYPE_MOVE] = {"B","Action",{0x64, 0x64, 0xF0}},
    [MTYPE_BUFF] = {"R","Logistics",{200, 0, 0}},
    [MTYPE_COND] = {"G","Sequencing",{0, 170, 0}},
    [MTYPE_INPUT] = {"","Input",{0xC9, 0x82, 0x30}},
    [MTYPE_NONMOD] = {"",NULL,{0x00, 0x00, 0x00}},
    [MTYPE_VANITY] = {"P","Vanity",{0xD3,0x81,0xFC}},
    [MTYPE_SETTINGS] = {"1","Option",{0xAA,0xAA,0xAA}},
    [MTYPE_UPGRADE] = {"O","Upgrade",{210,176,0}},
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

    int i2 = 0;
    for (int i = 0; i < MOD_COUNT; i++) {
        if (module_infos[i].type != MTYPE_INPUT && module_infos[i].type != MTYPE_NONMOD) {
            inventory[39+(i2/8)][i2%8] = i;
            i2++;
        }
    }

    load_marios_modules();
    update_settings();
}

void module_update(void) {
    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->cooldown) {
            if (met->timer >= met->cooltime) {
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
                        met->cooltime += module_infos[read_mod].cooldown*30.0f;

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
        met->cooltime = 5;
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
            if (inventory_panel != PANEL_CREATIVE) {
                inventory[true_inventory_y][inventory_x] = module_in_hand;
            }
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

char * module_is_invalid(int x, int y) {
    s8 mod = get_inventory(x,y);

    // Blank spaces are never invalid
    if (mod == -1) {
        return NULL;
    }

    // Missing a module to the left? Is invalid!
    if (x != 0 && get_inventory(x-1,y) == -1 && inventory_row_info[y].type == ROW_SOCKET) {
        return "@R@Unconnected";
    }

    // Not on the socket whitelist? Is invalid!
    if (inventory_row_info[y].type == ROW_SOCKET && !(1 << module_infos[mod].type & inventory_row_info[y].whitelist_flags)) {
        return "@R@Incompatbile";
    }

    return NULL;
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

            char * invalid = module_is_invalid(x,true_y);
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

    char * errmsg = module_is_invalid(inventory_x,inventory_y+icp->offset);
    if (errmsg) {
        //print reason
        utf8_print_reset();
        print_utf8(errmsg, 15+inventory_vis_x, 230-inventory_vis_y);
    }

    // PRINT PANEL INFO
    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160);
    render_4slice(25,122,33+162,122-16);

    utf8_print_reset();
    int sx;
    int sy;
    utf8_size(icp->name,&sx,&sy);
    print_utf8(icp->name, 30+(81-(sx/2)), 122-16);

    print_utf8("@<@←@@𝐋", 32, 122-16);
    print_utf8("𝐑@>@→", 33+162-16, 122-16);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);

    //PRINT MOD INFO
    s8 mod_inf_to_disp = MOD_EMPTY;

    if (module_in_hand != MOD_EMPTY) {
        mod_inf_to_disp = module_in_hand;
    } else {
        mod_inf_to_disp = inventory[inventory_y + icp->offset][inventory_x];
    }

    if (mod_inf_to_disp != MOD_EMPTY) {
        print_set_envcolour(255, 255, 255, 255);
        sprintf(print_buffer, "@%s@%s (%s):@@ %s\n",
            module_type_infos[module_infos[mod_inf_to_disp].type].text_color,
            module_infos[mod_inf_to_disp].name,
            module_type_infos[module_infos[mod_inf_to_disp].type].name,
            module_infos[mod_inf_to_disp].desc);
        if (module_infos[mod_inf_to_disp].upg_desc != NULL) {
            sprintf(print_buffer, "%s@O@UPG: @@%s ",print_buffer,module_infos[mod_inf_to_disp].upg_desc);
        }
        if (module_infos[mod_inf_to_disp].cooldown != 0.0f) {
            sprintf(print_buffer, "%s@1@Cooldown: %.1fs@@",print_buffer,module_infos[mod_inf_to_disp].cooldown);
        }

        gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
        gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160);
        render_4slice(25,82,33+260,25);

        utf8_print_reset();
        print_utf8(utf8_autonewline(print_buffer,260), 30, 64);
        gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
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

Gfx *geo_module_material(s32 callContext, struct GraphNode *node, void *context) {
    Gfx *dlStart, *dlHead;
    struct Object *obj;
    struct GraphNodeGenerated *currentGraphNode;

    currentGraphNode = node;

    if (callContext == GEO_CONTEXT_RENDER) {
        obj = (struct Object *) gCurGraphNodeObject;

        dlHead = alloc_display_list(sizeof(Gfx) * (11));
        dlStart = dlHead;

        gDPPipeSync(dlHead++);
        gDPSetCombineLERP(dlHead++,0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0);
        gSPTexture(dlHead++,65535, 65535, 0, 0, 1);
        gDPSetTextureImage(dlHead++,G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, module_infos[obj->oBehParams2ndByte].tex);
        gDPSetTile(dlHead++,G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0);
        gDPLoadBlock(dlHead++,7, 0, 0, 255, 512);
        gDPSetTile(dlHead++,G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0);
        gDPSetTileSize(dlHead++,0, 0, 0, 60, 60);
        gSPEndDisplayList(dlHead++);

        geo_append_display_list(dlStart, LAYER_TRANSPARENT);

        dlHead = alloc_display_list(sizeof(Gfx) * (5));
        dlStart = dlHead;

        int id = obj->oBehParams2ndByte;
        u8 r = module_type_infos[module_infos[id].type].color[0];
        u8 g = module_type_infos[module_infos[id].type].color[1];
        u8 b = module_type_infos[module_infos[id].type].color[2];
        gDPSetEnvColor(dlHead++, r,g,b, 255);
        gSPEndDisplayList(dlHead++);

        geo_append_display_list(dlStart, LAYER_ALPHA);
    }
    return NULL;
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
* Cap + Shirt (Vanity)\n\
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
* Re-organized module classifications\n\
* Modules in chests are now 3D\n\
\n\
Rebalances:\n\
* Putting jumps together no longer increases jump tier\n\
* Cap module incurs 4 second cooldown";

struct mariosModulesSave sMariosModulesSave;

void save_marios_modules(Vec3f pos) {
    int size = sizeof(struct mariosModulesSave);

    if (gSramProbe != 0) {
        sMariosModulesSave.version = MARIOS_MODULES_GAME_VERSION;
        for (int i = 0; i < 3; i++) {
            sMariosModulesSave.pos[i] = pos[i];
        }
        sMariosModulesSave.save_magic = SAVE_MAGIC;
        sMariosModulesSave.keys = gMarioState->numKeys;
        sMariosModulesSave.coins = gMarioState->numCoins;
        bcopy(&inventory,&sMariosModulesSave.inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        nuPiWriteSram(0, &sMariosModulesSave, ALIGN8(size));
    }

}

void load_marios_modules(void) {
    int size = sizeof(struct mariosModulesSave);

    if (gSramProbe != 0) {
        nuPiReadSram(0, &sMariosModulesSave, ALIGN8(size));
        if (sMariosModulesSave.save_magic == SAVE_MAGIC) {
            bcopy(&sMariosModulesSave.inventory,&inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        } else {
            bzero(&sMariosModulesSave,size);
        }
        gMarioState->numKeys = sMariosModulesSave.keys;
        gMarioState->numCoins = sMariosModulesSave.coins;
    }
}

int saveBinTotal[SAVE_BIN_COUNT];

void save_bin_reset(void) {
    for (int i = 0; i < SAVE_BIN_COUNT; i++) {
        saveBinTotal[i] = 0;
    }
}

void obj_save_bin_count(int type) {
    o->saveBinId = saveBinTotal[type];
    o->saveBinType = type;
    saveBinTotal[type]++;
}

u32 obj_save_bin_read(void) {
    return (sMariosModulesSave.bin[o->saveBinType] & (1 << o->saveBinId));
}

void obj_save_bin_write(void) {
    sMariosModulesSave.bin[o->saveBinType] |= (1 << o->saveBinId);
}

s32 save_bin_get_flag_total(int type) {
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if (sMariosModulesSave.bin[type] & (1 << i)) {
            count++;
        }
    }
    return count;
}

void marios_modules_savefile_load_position(void) {
    if (sMariosModulesSave.save_magic == SAVE_MAGIC) {
        for (int i = 0; i < 3; i++) {
            gMarioState->pos[i] = sMariosModulesSave.pos[i];
        }
    }
}