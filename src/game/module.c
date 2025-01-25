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
#include "behavior_data.h"

u8 gModuleMenuOpen = FALSE;

struct module_execution_thread module_execution_threads[MODULE_EXEC_COUNT];

void module_jump(struct module_execution_thread * met, u8 call_context) {
    gMarioState->input |= INPUT_A_PRESSED;
    
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
    if (met->repeat) {
        met->x++;
    } else {
        met->x=0;
        met->repeat=TRUE;
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

struct module_info module_infos[] = {
    [MOD_BUTTON_A] = {MTYPE_INPUT,micons_abtn_rgba16,NULL,NULL,NULL},
    [MOD_BUTTON_B] = {MTYPE_INPUT,micons_bbtn_rgba16,NULL,NULL,NULL},
    [MOD_JUMP] = {MTYPE_MOVE,micons_jump_rgba16,"Makes Mario attempt to jump.","Increases jump tier per MOD.",module_jump},
    [MOD_POW] = {MTYPE_BUFF,micons_onepow_rgba16,"Adds 1 to the MOD of the next piece.",NULL,module_pow},
    [MOD_HIT_GROUND] = {MTYPE_COND,micons_ground_rgba16,"Continues when Mario touches the ground.",NULL,module_floor},
    [MOD_HIT_WALL] = {MTYPE_COND,micons_wall_rgba16,"Continues when Mario touches a wall.",NULL,module_wall},
    [MOD_TIMER] = {MTYPE_COND,micons_clock_rgba16,"Continues after a 1/2 second.","Adds 1/3 a second per MOD.",module_timer},
    [MOD_ATTACK] = {MTYPE_MOVE,micons_pow_rgba16,"Makes Mario attempt to attack.",NULL,module_attack},
    [MOD_INPUT] = {MTYPE_COND,micons_btngen_rgba16,"Checks for a button press for one second.",NULL,module_input},
    [MOD_SPD] = {MTYPE_BUFF,micons_spd_rgba16,"Adds speed to next action block.",NULL,module_spd},
    [MOD_PLATFORM] = {MTYPE_MOVE,micons_hover_rgba16,"Mario hovers for one second. Can jump.","Extend hover time by 1/2.",module_platform},
    [MOD_REPEAT] = {MTYPE_BUFF,micons_repeat_rgba16,"Repeats from the start.",NULL,module_repeat},
    [MOD_SWAP] = {MTYPE_MOVE,micons_swap_rgba16,"Toggles swap platforms.",NULL,NULL},
    [MOD_CAP] = {MTYPE_MOVE,micons_cap_rgba16,"Enables cap power for one second.","0:Vanish, 1:Metal, 2:Wing.",module_cap},
    [MOD_GRAPPLE] = {MTYPE_MOVE,micons_grapple_rgba16,"Launches a grapple hook. Must hit wood.",NULL,NULL},
    [MOD_GRAV] = {MTYPE_COND,micons_grav_rgba16,"Continues when Mario has downward velocity.",NULL,module_grav},
    [MOD_NONMOD_KEY] = {MTYPE_NONMOD, micons_key_rgba16,NULL,NULL,NULL},
};

struct module_type_info module_type_infos[] = {
    [MTYPE_MOVE] = {"<COL_6464F0FF>Action",{0x64, 0x64, 0xF0}},
    [MTYPE_BUFF] = {"<COL_C80000FF>Modifier",{200, 0, 0}},
    [MTYPE_COND] = {"<COL_00AA00FF>Condition",{0, 170, 0}},
    [MTYPE_INPUT] = {"Input",{0xC9, 0x82, 0x30}},
    [MTYPE_NONMOD] = {NULL,{0x00, 0x00, 0x00}}
};

#define INVENTORY_PRINT_OFFSET_X 80
#define INVENTORY_PRINT_OFFSET_Y 70

#define INVENTORY_SLOTS_Y 5
#define INVENTORY_SLOTS_X 8
s8 inventory[INVENTORY_SLOTS_Y][INVENTORY_SLOTS_X];

int inventory_x = 0;
int inventory_y = 0;

f32 inventory_vis_x = 0.0f;
f32 inventory_vis_y = 0.0f;

s8 module_in_hand = MOD_EMPTY;

s32 is_inventory_slot_locked(int x, int y) {
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
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 2; y<INVENTORY_SLOTS_Y; y++) {
            if (inventory[y][x] == MOD_EMPTY) {
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

    module_execution_threads[MODULE_EXEC_A].executing = FALSE;
    module_execution_threads[MODULE_EXEC_B].executing = FALSE;
}

void module_update(void) {
    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->cooldown) {
            if (met->timer >= 15) {
                play_sound(SOUND_MENU_MESSAGE_DISAPPEAR,gGlobalSoundSource);
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
                    module_infos[read_mod].func(met,MCC_INVOKE);
                    read_mod = get_inventory(met->x,met->y);
                    if (met->halted) {
                        return;
                    }
                }
                met->cooldown = TRUE;
                met->timer = 0;
            } else {
                module_infos[read_mod].func(met,MCC_HALTED);
                met->timer++;
            }
        }
    }
}

void execute_module_in_inventory(struct module_execution_thread * met, u32 input, int x, int y) {
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
        met->repeat = FALSE;

        gMarioState->actionMod = 0;

        play_sound(SOUND_MENU_MESSAGE_APPEAR,gGlobalSoundSource);
    }
}

s32 handle_module_inputs(void) {
    if (!gModuleMenuOpen) {
        if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_A],A_BUTTON,0,0);
        }
        if (gPlayer1Controller->buttonPressed & B_BUTTON) {
            execute_module_in_inventory(&module_execution_threads[MODULE_EXEC_B],B_BUTTON,0,1);
        }
    }
    return FALSE;
}

u8 control_neutral = TRUE;
void control_module_menu(void) {
    for (int i = 0; i < MODULE_EXEC_COUNT; i++) {
        struct module_execution_thread * met = &module_execution_threads[i];
        if (met->executing) {
            //no editing while running
            return;
        }
    }

    //handle joystick
    if (
        (gPlayer1Controller->rawStickY < 60) &&
        (gPlayer1Controller->rawStickY > -60) &&
        (gPlayer1Controller->rawStickX < 60) &&
        (gPlayer1Controller->rawStickX > -60)
    ) {
        control_neutral = TRUE;
    }
    if (control_neutral) {
        if (gPlayer1Controller->rawStickY > 60) {
            gPlayer1Controller->buttonPressed |= U_JPAD;
            control_neutral = FALSE;
        }
        if (gPlayer1Controller->rawStickY < -60) {
            gPlayer1Controller->buttonPressed |= D_JPAD;
            control_neutral = FALSE;
        }
        if (gPlayer1Controller->rawStickX > 60) {
            gPlayer1Controller->buttonPressed |= R_JPAD;
            control_neutral = FALSE;
        }
        if (gPlayer1Controller->rawStickX < -60) {
            gPlayer1Controller->buttonPressed |= L_JPAD;
            control_neutral = FALSE;
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

    inventory_x = (INVENTORY_SLOTS_X+inventory_x)%INVENTORY_SLOTS_X;
    inventory_y = (INVENTORY_SLOTS_Y+inventory_y)%INVENTORY_SLOTS_Y;

    if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        if (is_inventory_slot_locked(inventory_x,inventory_y)) {
            play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundSource);
        } else {
            if (!(module_in_hand == MOD_EMPTY && inventory[inventory_y][inventory_x] == MOD_EMPTY)) {
                play_sound(SOUND_MENU_CLICK_FILE_SELECT, gGlobalSoundSource);
            }

            s16 module_to_pick_up = inventory[inventory_y][inventory_x];
            inventory[inventory_y][inventory_x] = module_in_hand;
            module_in_hand = module_to_pick_up;
        }
    }
}

void print_texture(void * tex, int size, int x, int y) {
    gDPPipeSync(gDisplayListHead++);
    gDPLoadTextureBlock(gDisplayListHead++, tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, size,size, 0, 0, 0, 0, 0, 0, 0);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + (size)) << 2,
                        (y + (size)) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
}

void print_module(int id, int x, int y) {
    if (id == MOD_EMPTY) return;
    u8 r = module_type_infos[module_infos[id].type].color[0];
    u8 g = module_type_infos[module_infos[id].type].color[1];
    u8 b = module_type_infos[module_infos[id].type].color[2];
    gDPSetEnvColor(gDisplayListHead++, r,g,b, 255);
    if (module_infos[id].type != MTYPE_NONMOD) {
        print_texture(micons_piece_rgba16,32,x,y);
    }
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_texture(module_infos[id].tex,16,x,y);
}

int inv_slot_printx(int x, int y) {
    if (y < 2) {
        return x*16+INVENTORY_PRINT_OFFSET_X+22;
    }
    return x*21+INVENTORY_PRINT_OFFSET_X;
}
int inv_slot_printx_w(int x, int y) {
    if (y < 2) {
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

char print_buffer[500];
void print_module_menu(void) {
    control_module_menu();

    inventory_vis_x = approach_f32_asymptotic(inventory_vis_x,inv_slot_printx(inventory_x,inventory_y),.3f);
    inventory_vis_y = approach_f32_asymptotic(inventory_vis_y,inv_slot_printy(inventory_x,inventory_y),.3f);

    prepare_blank_box();
    for (int x = 0; x<INVENTORY_SLOTS_X; x++) {
        for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
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
        for (int y = 0; y<INVENTORY_SLOTS_Y; y++) {
            print_module(inventory[y][x],inv_slot_printx(x,y), inv_slot_printy(x,y));

            if (is_inventory_slot_locked(x,y)) {
                gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
                print_texture(micons_lock_rgba16,16,inv_slot_printx(x,y), inv_slot_printy(x,y));
                gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
            }
        }
    }
    print_module(MOD_BUTTON_A,inv_slot_printx(-1,0), inv_slot_printy(-1,0));
    print_module(MOD_BUTTON_B,inv_slot_printx(-1,1), inv_slot_printy(-1,1));

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
        mod_inf_to_disp = inventory[inventory_y][inventory_x];
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