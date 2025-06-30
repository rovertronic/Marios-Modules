#ifndef MODULE_H
#define MODULE_H

#include "types.h"

extern u8 gModuleMenuOpen;

struct module_panel {
    char * name;
    u8 offset;
    u8 size;
    s8 unlock_flag;
};

enum {
    ROW_UNUSED,
    ROW_STORAGE,
    ROW_SOCKET
};

struct inventory_row {
    u8 type;
    s8 icon;
    s8 mod_type_prio;
};

enum {
    PANEL_SETTINGS,
    PANEL_ACTIONS,
    PANEL_VANITY,
};

struct module_execution_thread {
    u32 input;
    u8 mod;
    u8 x;
    u8 y;
    u8 timer;
    u8 spd;

    u8 executing:1;
    u8 halted:1;
    u8 cooldown:1;
    u8 input_notify:1;
    u8 jump_tier:2;

    u16 used_flags;
};

enum module_execution_ids {
    MODULE_EXEC_A,
    MODULE_EXEC_B,
    MODULE_EXEC_COUNT,
};

struct module_info {
    u8 type;
    void * tex;
    char * desc;
    char * mod_desc;
    void (*func)(struct module_execution_thread * met, u8 call_context);
};

enum module_call_context {
    MCC_INVOKE,
    MCC_HALTED,
};

struct module_type_info {
    char * name;
    u8 color[3];
};

enum module_type {
    MTYPE_MOVE,
    MTYPE_COND,
    MTYPE_BUFF,
    MTYPE_INPUT,
    MTYPE_NONMOD,
    MTYPE_VANITY
};

#define MOD_EMPTY -1
enum module_id {
    MOD_BUTTON_A,
    MOD_BUTTON_B,
    MOD_JUMP,
    MOD_POW,
    MOD_HIT_GROUND,
    MOD_HIT_WALL,
    MOD_TIMER,
    MOD_ATTACK,
    MOD_INPUT,
    MOD_SPD,
    MOD_PLATFORM,
    MOD_REPEAT,
    MOD_SWAP,
    MOD_CAP,
    MOD_GRAPPLE,
    MOD_GRAV,
    MOD_NONMOD_KEY,
    MOD_VANITY,
    MOD_SETTINGS,
    MOD_VAN_CAP,
    MOD_VAN_
};

void add_inventory(s8 module);
void module_update(void);
void print_module_menu(void);
void print_module_hud_status(void);
void init_module_inventory(void);
s32 handle_module_inputs(void);
void control_module_menu(void);

#endif