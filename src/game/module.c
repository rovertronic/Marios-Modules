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
#include "area.h"
#include "sram.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include <PR/os_internal_reg.h>
#include "utf8_print.h"
#include "frame_lerp.h"
#include "seq_ids.h"

u8 gModuleTutorialState = TUTORIAL_WAIT_FOR_MODULE_COLLECT;
struct ScreenMessage sScreenMessageList[15];
s8 sScreenMessageCount = -1;
s8 sScreenMessageIndex = -1;

u8 gModuleMenuOpen = FALSE;
u8 gModuleMenuMode = MODULE_MENU_MODE_NORMAL;
s8 gRecycleChestContent = MOD_EMPTY;
s8 gRecycledModule = MOD_EMPTY;
u8 gGameSettings[SETTING_COUNT];
Vec3f gModulePreviewPos;
u8 gModuleUpdateVanity = FALSE;

s8 gMysteryModuleState = 0;
s8 gMysteryModuleSelection = -1;
s8 gMysteryModuleChoice[2];
f32 sMysteryModuleAlpha = 0.0f;

struct module_execution_thread module_execution_threads[MODULE_EXEC_COUNT];

#include "module_data.inc.c"

s8 inventory[INVENTORY_SLOTS_Y][INVENTORY_SLOTS_X];
u8 inventoryParam[INVENTORY_SLOTS_Y][INVENTORY_SLOTS_X];

int inventory_x = 0;
int inventory_y = 0;

f32 inventory_vis_x = 0.0f;
f32 inventory_vis_y = 0.0f;

s8 module_in_hand = MOD_EMPTY;
u8 module_param_in_hand = 0;

struct module_panel * icp = &module_panel_info[PANEL_ACTIONS];
int inventory_panel = PANEL_ACTIONS;

#define DEBUG_LOG_MAX 12
char * sDebugLogStrs[DEBUG_LOG_MAX];
int sDebugLogModuleDisplays[DEBUG_LOG_MAX][2];
int sDebugLogModuleNumber[DEBUG_LOG_MAX];
u8 sDebugLogIndex = 0;

void module_log_message(struct module_execution_thread * met, char * logmsg, int num) {
    if (met->debug_monitor == TRUE) {
        sDebugLogStrs[sDebugLogIndex] = logmsg;
        s8 setModIcon = MOD_PASSIVE;
        if (met == &module_execution_threads[MODULE_EXEC_A] ) {
            setModIcon = MOD_BUTTON_A;
        } else if (met == &module_execution_threads[MODULE_EXEC_B]) {
            setModIcon = MOD_BUTTON_B;
        }
        sDebugLogModuleDisplays[sDebugLogIndex][0] = setModIcon;
        sDebugLogModuleDisplays[sDebugLogIndex][1] = get_inventory(met->x,met->y);
        sDebugLogModuleNumber[sDebugLogIndex] = num;
        sDebugLogIndex=(sDebugLogIndex+1)%DEBUG_LOG_MAX;
    }
}

void module_log_clear(void) {
    for (int i = 0; i < DEBUG_LOG_MAX; i++) {
        sDebugLogStrs[i] = NULL;
    } 
}

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

u8 get_inventory_param(int x, int y) {
    if ((x >= INVENTORY_SLOTS_X)||(x < 0)||(y >= INVENTORY_SLOTS_Y)||(y < 0)) {
        return 0;
    }
    return inventoryParam[y][x];
}

void add_inventory(s8 module) {
    // Logic modules always come packaged with if blocks
    if (module_infos[module].type == MTYPE_LOGIC && module != MOD_IF && module != MOD_ENDBLOCK) {
        add_inventory(MOD_IF);
        add_inventory(MOD_ENDBLOCK);
    }

    // Check for specialized inventory slots first, before
    for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
        for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
            if (inventory[y][x] == MOD_EMPTY && inventory_row_info[y].type == ROW_STORAGE && inventory_row_info[y].mod_type_prio == module_infos[module].type) {
                inventory[y][x] = module;
                inventoryParam[y][x] = 0;
                return;
            }
        }
    }

    for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
        for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
            if (inventory[y][x] == MOD_EMPTY && inventory_row_info[y].type == ROW_STORAGE) {
                inventory[y][x] = module;
                inventoryParam[y][x] = 0;
                return;
            }
        }
    }
}

void drop_inventory(s8 module, u8 dropy) {
    for (int y = dropy; y<INVENTORY_SLOTS_Y; y++) {
        for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
            if (inventory[y][x] == MOD_EMPTY && inventory_row_info[y].type == ROW_STORAGE) {
                inventory[y][x] = module;
                inventoryParam[y][x] = 0;
                return;
            }
        }
    }
}

void init_module_inventory(void) {
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
            inventory[y][x] = MOD_EMPTY;
            inventoryParam[y][x] = 0;
        }
    }

    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        module_execution_threads[i].executing = FALSE;
    }

    // Settings
    inventory[48][0] = MOD_CAMERA_COLLISION;
    inventory[49][0] = MOD_WIDESCREEN;
    inventory[49][1] = MOD_NOMUSIC;
    if (gEmulator & EMU_CONSOLE) {
        // N64 specific configuration
        inventory[49][2] = MOD_60HZ;
    } else {
        inventory[48][1] = MOD_60HZ;
    }

    // Vanity
    inventory[44][0] = MOD_TAN;
    inventory[44][1] = MOD_VAN_SKIN;
    inventory[46][0] = MOD_WOMAN;
    inventory[46][1] = MOD_BROWN;

    // Starter inventory
    inventory[4][7] = MOD_MONITOR;

    tinymt32_init(&gGlobalRandomState,0);

    // Populate creative inventory
    int i2 = 0;
    for (int j = 0; j < MTYPE_COUNT; j++) {
        for (int i = 0; i < MOD_COUNT; i++) {
            if (module_infos[i].creative && module_infos[i].type == j) {
                inventory[39+(i2/8)][i2%8] = i;
                i2++;
            }
        }
    }

    update_settings();
}

void tutorial_handler(void) {
    switch(gModuleTutorialState) {
        case TUTORIAL_MOVE_CURSOR:
            if (inventory_x != 0 || inventory_y != 0) {
                display_tutorial_message("Press @B@A@@ to pick up the jump module.",TUTORIAL_PICK_UP_MOD);
                gModuleTutorialState = TUTORIAL_PICK_UP_MOD;
            }
            break;
        case TUTORIAL_PRESS_START:
            if (gModuleMenuOpen) {
                display_tutorial_message("Use the analog stick to move the cursor.",TUTORIAL_MOVE_CURSOR);
                gModuleTutorialState = TUTORIAL_MOVE_CURSOR;
            }
            break;
        case TUTORIAL_PICK_UP_MOD:
            if (module_in_hand == MOD_JUMP) {
                gModuleTutorialState = TUTORIAL_PLACE_MOD;
                display_tutorial_message("Now place it in @B@Socket A@@.",TUTORIAL_PLACE_MOD);
            }
            break;
        case TUTORIAL_PLACE_MOD:
            if (inventory[0][0] == MOD_JUMP) {
                gModuleTutorialState = TUTORIAL_GET_STAR;
                display_generic_message("Now you can press @B@A@@ to do a single jump!");
                display_tutorial_message("Now collect the @Y@star@@.",TUTORIAL_GET_STAR);
            }
            break;
        case TUTORIAL_GET_STAR:
            if (save_bin_get_flag_total(SAVE_BIN_STARS) > 0) {
                display_generic_message("Collecting @Y@stars@@ unlocks more socket slots.");
                display_generic_message("You can now craft more complex moves.");
                display_generic_message("Keep in mind sockets execute from left to right.");
                display_generic_message("Good luck!");
                gModuleTutorialState = TUTORIAL_DONE;
            }
            break;
    }
}

void module_update(void) {
    tutorial_handler();

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
                (GROUNDED)
                && (count_objects_with_behavior(bhvHover) == 0) 
                && (gMarioState->capTimer == 0)
            ) {
                met->timer++;
            }
        } else if (met->executing) {
            s8 read_mod = get_inventory(met->x,met->y);
            if (met->begin) {
                met->begin = FALSE;
                if (met == &module_execution_threads[MODULE_EXEC_PASSIVE]) {
                    gMarioState->passiveFlag = 0;
                }
                if (met == &module_execution_threads[MODULE_EXEC_VANITY]) {
                    gMarioState->marioObj->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_MARIO];
                }
            }
            if (!met->halted) {
                while(read_mod != MOD_EMPTY) {                    
                    if (1 << module_infos[read_mod].type & inventory_row_info[met->y].whitelist_flags) {
                        met->extra_data = module_infos[read_mod].extra_data;
                        met->option = inventoryParam[met->y][met->x];
                        if (module_infos[read_mod].func != NULL) {
                            module_infos[read_mod].func(met,MCC_INVOKE);
                        } else {
                            // No function = passthrough
                            met->x++;
                        }
                        met->cooltime += module_infos[read_mod].cooldown*30.0f;

                        if (inventory_row_info[met->y].wrap && met->x == INVENTORY_SLOTS_X) {
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
                met->cooltime = MAX(met->cooltime,1);
                if (met->doaircooldown || met->cooltime > 1) {
                    met->cooldown = TRUE;
                    if (met == &module_execution_threads[MODULE_EXEC_PASSIVE]) {
                        gMarioState->passiveFlag = 0;
                    }
                } else {
                    met->executing = FALSE;
                }
                met->timer = 0;
            } else {
                if (!met->mario_ground_listener) {
                    if (GROUNDED) {
                        met->mario_ground_listener = TRUE;
                        met->landing_count ++;
                    }
                } else {
                    if (!GROUNDED) {
                        met->mario_ground_listener = FALSE;
                    }
                }

                module_infos[read_mod].func(met,MCC_HALTED);
                met->timer++;
            }
        }
    }
}

void add_met_condition(struct module_execution_thread * met, s32 condition) {
    if (condition) {
        met->condition_flags |= (1<<met->condition_count);
    }
    met->condition_count++;
}

void execute_module_in_inventory(struct module_execution_thread * met, u32 input, int x, int y, int manual) {
    if (!met->executing) {
        met->begin = TRUE;
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
        met->cooltime = 1;
        met->element = ELEMENT_NORMAL;
        met->landing_count = 0;
        met->mario_ground_listener = TRUE;
        met->ifbool = FALSE;
        met->doaircooldown = FALSE;
        met->debug_monitor = FALSE;

        met->condition_flags = 0;
        met->condition_count = 0;
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
        execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_PASSIVE],Z_TRIG,0,5,FALSE);
        update_vanity();
    }
    return FALSE;
}

void update_vanity(void) {
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
u8 double_tap_return = FALSE;

void joystick_to_dpad(void) {
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
        double_tap_return = FALSE;
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
}

void control_module_menu(void) {
    // Always turn off passive effects when in menu
    gMarioState->passiveFlag = 0;

    // handle panel changing
    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        inventory_panel = (INVENTORY_PANEL_CT+inventory_panel+1)%INVENTORY_PANEL_CT;
        while ((module_panel_info[inventory_panel].unlock != NULL)
        && !module_panel_info[inventory_panel].unlock()) {
            inventory_panel = (INVENTORY_PANEL_CT+inventory_panel+1)%INVENTORY_PANEL_CT;
        }
    }
    if (gPlayer1Controller->buttonPressed & L_TRIG) {
        inventory_panel = (INVENTORY_PANEL_CT+inventory_panel-1)%INVENTORY_PANEL_CT;
        while ((module_panel_info[inventory_panel].unlock != NULL)
        && !module_panel_info[inventory_panel].unlock()) {
            inventory_panel = (INVENTORY_PANEL_CT+inventory_panel-1)%INVENTORY_PANEL_CT;
        }
    }

    icp = &module_panel_info[inventory_panel];

    joystick_to_dpad();

    if (gPlayer1Controller->buttonPressed & L_JPAD) {
        inventory_x --;
        double_tap_return = FALSE;
    } else if (gPlayer1Controller->buttonPressed & R_JPAD) {
        inventory_x ++;
        double_tap_return = FALSE;
    } else if (gPlayer1Controller->buttonPressed & D_JPAD) {
        inventory_y ++;
        double_tap_return = FALSE;
    } else if (gPlayer1Controller->buttonPressed & U_JPAD) {
        inventory_y --;
        double_tap_return = FALSE;
    }

    inventory_x = (INVENTORY_SLOTS_X+inventory_x)%INVENTORY_SLOTS_X;
    inventory_y = (icp->size+inventory_y)%icp->size;

    int true_inventory_y = inventory_y + icp->offset;

    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->y == true_inventory_y && met->executing && !met->cooldown) {
            //no editing while running
            return;
        }
    }

    int modified_inventory = FALSE;
    if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        if (gModuleMenuMode == MODULE_MENU_MODE_NORMAL) {
            if (is_inventory_slot_locked(inventory_x,inventory_y)) {
                play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundSource);
            } else {
                if (!(module_in_hand == MOD_EMPTY && inventory[true_inventory_y][inventory_x] == MOD_EMPTY)) {
                    play_sound(SOUND_MENU_CLICK_FILE_SELECT, gGlobalSoundSource);
                }

                s8 module_to_pick_up = inventory[true_inventory_y][inventory_x];
                u8 module_param_to_pick = inventoryParam[true_inventory_y][inventory_x];
                if (inventory_panel != PANEL_CREATIVE) {
                    inventory[true_inventory_y][inventory_x] = module_in_hand;
                    inventoryParam[true_inventory_y][inventory_x] = module_param_in_hand;
                }
                module_in_hand = module_to_pick_up;
                module_param_in_hand = module_param_to_pick;
            }
            modified_inventory = TRUE;
        } else {
            s8 recycledModule = inventory[true_inventory_y][inventory_x];

            int price = 4;
            int canSimilarRoll = TRUE;
            switch(module_infos[recycledModule].loot_tier) {
                case LOOT_TIER_2:
                    price = 10;
                    break;
                case LOOT_VANITY:
                    price = 1;
                    canSimilarRoll = FALSE;
                    break;
            }

            if (gMarioState->numCoins >= price && module_infos[recycledModule].loot_tier != LOOT_NONE) {
                gPlayer1Controller->buttonPressed = 0;

                gMarioState->numCoins-=price;
                gHudDisplay.coins = gMarioState->numCoins;

                gRecycledModule = inventory[true_inventory_y][inventory_x];
                inventory[true_inventory_y][inventory_x] = MOD_EMPTY;

                int checkSimilar = FALSE;
                if (canSimilarRoll) {
                    checkSimilar = random_u16()%2;
                }

                s8 rerolledModule = random_u16()%MOD_COUNT;
                int i = 0;
                while(
                    (module_infos[rerolledModule].loot_tier != module_infos[recycledModule].loot_tier)
                    || (module_infos[rerolledModule].func == module_infos[recycledModule].func)
                    || (rerolledModule == recycledModule)
                    || (checkSimilar && module_infos[rerolledModule].type != module_infos[recycledModule].type) ) {
                    rerolledModule = random_u16()%MOD_COUNT;
                    i++;
                    if (i > 10) {
                        //only 1 module of this type in the loot table
                        checkSimilar = FALSE;
                    }
                }

                gRecycleChestContent = rerolledModule;
                gModuleMenuOpen = FALSE;
            }
        }
    }

    if (module_in_hand != MOD_EMPTY && module_infos[module_in_hand].options && (gPlayer1Controller->buttonPressed & B_BUTTON)) {
        play_sound(SOUND_GENERAL_BIG_CLOCK, gGlobalSoundSource);
        module_param_in_hand++;
        if (module_infos[module_in_hand].options[module_param_in_hand] == NULL) {
            module_param_in_hand = 0;
        }
    }

    // SHORTCUTS (Not allowed in creative menu)
    if (inventory_panel != PANEL_CREATIVE) {
        if (gPlayer1Controller->buttonPressed & R_CBUTTONS) {
            inventory_vis_x += 5.0f;
            for (int i = INVENTORY_SLOTS_X-2; i >= inventory_x; i--) {
                s8 mod = get_inventory(i,true_inventory_y);
                u8 param = get_inventory_param(i,true_inventory_y);

                if (get_inventory(i+1,true_inventory_y) == MOD_EMPTY) {
                    inventory[true_inventory_y][i+1] = mod;
                    inventory[true_inventory_y][i] = MOD_EMPTY;

                    inventoryParam[true_inventory_y][i+1] = param;
                    inventoryParam[true_inventory_y][i] = 0;
                }
            }
            modified_inventory = TRUE;
        }

        if (gPlayer1Controller->buttonPressed & L_CBUTTONS) {
            inventory_vis_x -= 5.0f;
            for (int i = 1; i <= inventory_x; i++) {
                s8 mod = get_inventory(i,true_inventory_y);
                u8 param = get_inventory_param(i,true_inventory_y);

                if (get_inventory(i-1,true_inventory_y) == MOD_EMPTY) {
                    inventory[true_inventory_y][i-1] = mod;
                    inventory[true_inventory_y][i] = MOD_EMPTY;

                    inventoryParam[true_inventory_y][i-1] = param;
                    inventoryParam[true_inventory_y][i] = 0;
                }
            }
            modified_inventory = TRUE;
        }

        if (gPlayer1Controller->buttonPressed & D_CBUTTONS) {
            if (!double_tap_return) {
                inventory_vis_y += 10.0f;
                s8 mod = inventory[true_inventory_y][inventory_x];
                drop_inventory(mod,true_inventory_y);
                inventory[true_inventory_y][inventory_x] = MOD_EMPTY;
                double_tap_return = TRUE;
            } else {
                for (int i = 0; i < INVENTORY_SLOTS_X; i++) {
                    s8 mod = inventory[true_inventory_y][i];
                    drop_inventory(mod,true_inventory_y);
                    inventory[true_inventory_y][i] = MOD_EMPTY;
                }
            }
            modified_inventory = TRUE;
        }

        if (gPlayer1Controller->buttonPressed & Z_TRIG) {
            inventory_vis_x -= 10.0f;
            for (int j = 0; j < INVENTORY_SLOTS_X; j++) {
                for (int i = 1; i < INVENTORY_SLOTS_X; i++) {
                    s8 mod = get_inventory(i,true_inventory_y);

                    if (get_inventory(i-1,true_inventory_y) == MOD_EMPTY) {
                        inventory[true_inventory_y][i-1] = mod;
                        inventory[true_inventory_y][i] = MOD_EMPTY;
                    }
                }
            }
            modified_inventory = TRUE;
        }
    }

    if (modified_inventory) {
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

    // Stored modules should never have errors
    if (inventory_row_info[y].type != ROW_SOCKET) {
        return NULL;
    }

    // Missing a module to the left? Is invalid!
    if (x != 0 && get_inventory(x-1,y) == -1) {
        return "@R@Unconnected";
    }

    // Not on the socket whitelist? Is invalid!
    if (inventory_row_info[y].type == ROW_SOCKET && !(1 << module_infos[mod].type & inventory_row_info[y].whitelist_flags)) {
        return "@R@Incompatbile with socket";
    }

    if (get_inventory(x-1,y) == mod && module_infos[mod].unchainable) {
        return "@R@Module not chainable";
    }

    if (get_inventory(x-1,y) == MOD_GRAV && mod == MOD_FLIP_VEL) {
        return "@R@Negative 0 is still 0.";
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
            if (gModuleMenuMode == MODULE_MENU_MODE_RECYCLE && module_infos[inventory[true_y][x]].loot_tier == LOOT_NONE) {
                gPrintModuleDarken=2;
            }
            if (invalid) {
                gPrintModuleDarken=2;
            }
            print_module(inventory[true_y][x],inv_slot_printx(x,y), inv_slot_printy(x,y));
            if (invalid) {
                print_texture(micons_warn_rgba16,16,inv_slot_printx(x,y), inv_slot_printy(x,y));
                gPrintModuleDarken=1;
            }
            if (gModuleMenuMode == MODULE_MENU_MODE_RECYCLE && module_infos[inventory[true_y][x]].loot_tier == LOOT_NONE) {
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
        print_utf8_boxed(errmsg, 15+inventory_vis_x, 230-inventory_vis_y, 1.0f, FALSE);
    }

    if (icp == &module_panel_info[PANEL_SETTINGS]) {
        gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

        gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
        gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160);
        render_4slice(25,170,195,125);

        utf8_print_reset();
        gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);

        sprintf(print_buffer,"Chests: %d/%d\nStars: %d/%d",
        save_bin_get_flag_total(SAVE_BIN_CHESTS),save_bin_get_max_total(SAVE_BIN_CHESTS),
        save_bin_get_flag_total(SAVE_BIN_STARS), save_bin_get_max_total(SAVE_BIN_STARS));
        print_utf8(print_buffer,30,145);
    }

    // PRINT PANEL INFO
    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160);
    render_4slice(25,122,33+162,122-16);

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
    int sx;
    int sy;
    utf8_size(icp->name,&sx,&sy);

    print_utf8(icp->name, 30+(81-(sx/2)), 122-16);
    print_utf8("@<@←@@𝐋", 32, 122-16);
    print_utf8("𝐑@>@→", 33+162-16, 122-16);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);

    //PRINT MOD INFO
    s8 mod_inf_to_disp = MOD_EMPTY;
    u8 mod_param_inf = 0;

    if (module_in_hand != MOD_EMPTY) {
        mod_inf_to_disp = module_in_hand;
        mod_param_inf = module_param_in_hand;
    } else {
        mod_inf_to_disp = inventory[inventory_y + icp->offset][inventory_x];
        mod_param_inf = inventoryParam[inventory_y + icp->offset][inventory_x];
    }

    if (mod_inf_to_disp != MOD_EMPTY) {
        print_set_envcolour(255, 255, 255, 255);
        int charCt = sprintf(print_buffer, "@%s@%s (%s):@@ %s\n",
            module_type_infos[module_infos[mod_inf_to_disp].type].text_color,
            module_infos[mod_inf_to_disp].name,
            module_type_infos[module_infos[mod_inf_to_disp].type].name,
            module_infos[mod_inf_to_disp].desc);

        if (module_infos[mod_inf_to_disp].options != NULL) {
            char * paramStr = "(@G@B@@ to change: %s) ";
            if (module_in_hand == MOD_EMPTY) {
                paramStr = "(%s) ";
            }
            charCt += sprintf(print_buffer+charCt, paramStr, module_infos[mod_inf_to_disp].options[mod_param_inf]);
        }

        if (module_infos[mod_inf_to_disp].upg_desc != NULL) {
            charCt += sprintf(print_buffer+charCt, "@O@UPG: @@%s ",module_infos[mod_inf_to_disp].upg_desc);
        }
        if (module_infos[mod_inf_to_disp].cooldown != 0.0f) {
            charCt += sprintf(print_buffer+charCt, "@1@(Cooldown: %.1fs)@@ ",module_infos[mod_inf_to_disp].cooldown);
        }

        if (gModuleMenuMode == MODULE_MENU_MODE_RECYCLE) {
            int recyclePrice = 4;
            int canRecycle = TRUE;
            switch(module_infos[mod_inf_to_disp].loot_tier) {
                case LOOT_NONE:
                    canRecycle = FALSE;
                    break;
                case LOOT_TIER_2:
                    recyclePrice = 10;
                    break;
                case LOOT_VANITY:
                    recyclePrice = 1;
                    break;
            }

            if (canRecycle) {
                char * str = "Recycle %s for @Y@%d coins.@@ (You currently have @Y@%d@@)";
                if (gMarioState->numCoins < recyclePrice) {
                    str = "Recycle %s for @Y@%d coins.@@ (You currently have @R@%d@@)";
                }
                sprintf(print_buffer, str,module_infos[mod_inf_to_disp].name,recyclePrice,gMarioState->numCoins);
            } else {
                sprintf(print_buffer, "@R@Can't recycle.");
            }
        }
        /*
        if (module_infos[mod_inf_to_disp].elementable == TRUE) {
            sprintf(print_buffer, "%s@E@(Imbuable)@@ ", print_buffer);
        }
        */

        gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
        gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160);
        render_4slice(25,82,33+260,25);

        utf8_print_reset();
        gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
        print_utf8(utf8_autonewline(print_buffer,260), 30, 64);
        gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
    }
}

char * get_screen_message_buffer(void) {
    return &sScreenMessageList[sScreenMessageCount+1];
}

void add_screen_message(f32 time) {
    // Assume usage of sprintf and get_screen_message_buffer before this
    sScreenMessageCount++;
    sScreenMessageList[sScreenMessageCount].time = time;
    sScreenMessageList[sScreenMessageCount].tutorialHoldId = -1;
}

f32 gMessageDisplayTimer = 0.0f;
f32 gMessageDisplayNextTimer = 0.0f;
f32 messageDisplayAlpha = 0.0f;
char * messageDisplayPtr = NULL;
char * messageDisplayQueuePtr = NULL;

char print_buffer_t5[100];
char print_buffer_t5_2[100];
void display_module_message(s8 id) {
    char * usebuff = get_screen_message_buffer();
    if (module_infos[id].type != MTYPE_NONMOD && id != MOD_PASSIVE) {
        sprintf(usebuff,"Obtained @%s@%s@@ module.",module_type_infos[module_infos[id].type].text_color,module_infos[id].name);
    } else {
        sprintf(usebuff,"Obtained %s.",module_infos[id].name);
    }
    add_screen_message(120.f);
}

void display_generic_message(char * str) {
    sprintf(get_screen_message_buffer(),"%s",str);
    add_screen_message(120.f);
}

void display_tutorial_message(char * str, u8 tutorialId) {
    sprintf(get_screen_message_buffer(),"%s",str);
    add_screen_message(99999.f);
    sScreenMessageList[sScreenMessageCount].tutorialHoldId = tutorialId;
}

void print_module_generic_message(void) {
    if (messageDisplayAlpha == 0.0f && sScreenMessageCount != sScreenMessageIndex) {
        sScreenMessageIndex++;
    }

    if (sScreenMessageIndex == -1) {return;}

    if (sScreenMessageList[sScreenMessageIndex].time > 0) {
        sScreenMessageList[sScreenMessageIndex].time -= gFrameLerpDeltaTime;

        if (sScreenMessageList[sScreenMessageIndex].tutorialHoldId != gModuleTutorialState &&
            sScreenMessageList[sScreenMessageIndex].tutorialHoldId != -1) {
            sScreenMessageList[sScreenMessageIndex].time = 0;
        }
        messageDisplayAlpha += gFrameLerpDeltaTime*.1f;
    } else {
        messageDisplayAlpha -= gFrameLerpDeltaTime*.1f;
    }
    messageDisplayAlpha = CLAMP(messageDisplayAlpha,0.0f,1.0f);

    if (messageDisplayAlpha > 0.0f) {
        print_utf8_boxed(sScreenMessageList[sScreenMessageIndex].text ,160,10,messageDisplayAlpha,TRUE);
    }

    if (messageDisplayAlpha == 0.0f && sScreenMessageIndex == sScreenMessageCount) {
        sScreenMessageIndex = -1;
        sScreenMessageCount = -1;
    }

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#define MODULE_HUD_STATUS_Y 205
void print_execution_status(int x, int y, int execthread, int module) {
    u8 dotShowCondition = (module_execution_threads[execthread].executing);
    if (execthread == MODULE_EXEC_PASSIVE) {
        dotShowCondition = (module_execution_threads[execthread].cooldown);
    }

    print_module(module,x,y);
    if (dotShowCondition) {
        print_texture(micons_executing_rgba16,16 ,x,y);
    }
    if (module_execution_threads[execthread].input_notify) {
        print_texture(micons_inpnotif_rgba16,16 ,x,y);
    }
}

char sDebugLogStringBuffer[100];
void print_module_hud_status(void) {

    if (gMysteryModuleState % 2 == 1) {
        sMysteryModuleAlpha+=gFrameLerpDeltaTime*.1f;
        sMysteryModuleAlpha = CLAMP(sMysteryModuleAlpha,0.0f,1.0f);
    } else {
        sMysteryModuleAlpha-=gFrameLerpDeltaTime*.1f;
        sMysteryModuleAlpha = CLAMP(sMysteryModuleAlpha,0.0f,1.0f);
    }
    if (sMysteryModuleAlpha > 0.0f) {
        u8 noThanks = (gMysteryModuleState >= 3);
        gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
        gDPSetEnvColor(gDisplayListHead++, 0,0,0, 160 * sMysteryModuleAlpha);
        render_4slice(160-40,120+30,160+40,120-30+(noThanks*-20));

        utf8_print_reset();
        gDPSetEnvColor(gDisplayListHead++, 255,255,255, 255 * sMysteryModuleAlpha);
        int offset; int y;
        utf8_size("Pick a module.",&offset,&y);
        offset/=2;
        print_utf8("Pick a module.",160 - offset ,125);
        if (noThanks) {
            utf8_size("B: No Thanks.",&offset,&y);
            offset/=2;
            print_utf8("@G@B@@: No Thanks.",160 - offset ,75);
        }

        gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
        
        gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
        print_module(gMysteryModuleChoice[0],140-8,120);
        print_module(gMysteryModuleChoice[1],180-8,120);

        s16 x = 160;
        switch(gMysteryModuleSelection) {
            case 0:
                x = 140;
                break;
            case 1:
                x = 180;
                break;
        }
        print_texture(micons_small_hand_1_rgba16,16,x, 130);
    }

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    print_execution_status(22,MODULE_HUD_STATUS_Y,MODULE_EXEC_A,MOD_BUTTON_A);
    print_execution_status(42,MODULE_HUD_STATUS_Y,MODULE_EXEC_B,MOD_BUTTON_B);
    if (passivePanelCondition()) {
        print_execution_status(62,MODULE_HUD_STATUS_Y,MODULE_EXEC_PASSIVE,MOD_PASSIVE);
    }

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    int y = 0;
    for (int i = 0; i < DEBUG_LOG_MAX; i++) {
        int index = (sDebugLogIndex - 1 - i + DEBUG_LOG_MAX) % DEBUG_LOG_MAX;
        if (sDebugLogStrs[index] != NULL) {
            y+=16;

            sprintf(sDebugLogStringBuffer, sDebugLogStrs[index], sDebugLogModuleNumber[index], sDebugLogModuleNumber[index]);

            utf8_print_reset();
            gDPSetEnvColor(gDisplayListHead++, 255,255,255, 200);
            print_utf8(sDebugLogStringBuffer,64,23+y);
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
            print_module(sDebugLogModuleDisplays[index][0],20,200-y);
            print_module(sDebugLogModuleDisplays[index][1],36,200-y);
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
        }
    }
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

#include "main_menu.inc.c"