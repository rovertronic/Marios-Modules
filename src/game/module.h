#ifndef MODULE_H
#define MODULE_H

#include "types.h"

#define MARIOS_MODULES_GAME_VERSION 1

#define INVENTORY_SLOTS_Y 50
#define INVENTORY_SLOTS_X 8

extern u8 gModuleMenuOpen;
extern u8 gGameSettings[];
extern Vec3f gModulePreviewPos;

struct module_panel {
    char * name;
    u8 offset;
    u8 size;
    s8 unlock_flag;
};

enum {
    PANEL_SETTINGS,
    PANEL_ACTIONS,
    PANEL_VANITY,
    PANEL_CREATIVE,
    INVENTORY_PANEL_CT,
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
    u8 manual:1;

    u16 used_flags;
    void * extra_data;
};

enum module_execution_ids {
    MODULE_EXEC_A,
    MODULE_EXEC_B,
    MODULE_EXEC_VANITY,
    MODULE_EXEC_VANITY_2,
    MODULE_EXEC_SETTINGS,
    MODULE_EXEC_COUNT,
};

struct module_info {
    u8 type;
    void * tex;
    char * desc;
    char * mod_desc;
    void (*func)(struct module_execution_thread * met, u8 call_context);
    void * extra_data;
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
    MTYPE_VANITY,
    MTYPE_SETTINGS,
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
    MOD_VAN_PANTS,
    MOD_VAN_HAIR,
    MOD_VAN_SKIN,
    MOD_RED,
    MOD_BLUE,
    MOD_GREEN,
    MOD_YELLOW,
    MOD_WHITE,
    MOD_BLACK,
    MOD_WOMAN,
    MOD_CAMERA_COLLISION,
    MOD_WIDESCREEN,
    MOD_60HZ,
    MOD_AA,
    MOD_WRAP,
    MOD_COUNT,
};

// Inventory
enum {
    ROW_UNUSED,
    ROW_STORAGE,
    ROW_SOCKET
};

struct inventory_row {
    u8 type;
    s8 icon;
    s8 mod_type_prio;
    u8 whitelist_flags;
    u8 wrap;
};

#define WHITELIST_VANITY ((1 << MTYPE_BUFF) | (1 << MTYPE_VANITY))
#define WHITELIST_ACTION ((1 << MTYPE_MOVE) | (1 << MTYPE_COND) | (1 << MTYPE_BUFF) | (1 << MTYPE_VANITY))

enum {
    SETTING_60HZ,
    SETTING_CAMERA_COLLISION,
    SETTING_WIDE,
    SETTING_AA,
    SETTING_COUNT,
};

enum {
    SAVE_BIN_STARS,
    SAVE_BIN_CHESTS,
    SAVE_BIN_DOORS,
    SAVE_BIN_COUNT,
};

#define SAVE_MAGIC 0x0203DD10 //my favorite rom address
struct mariosModulesSave {
    u8 version;
    Vec3s pos;
    u8 inventory[INVENTORY_SLOTS_Y*INVENTORY_SLOTS_X];
    u32 bin[SAVE_BIN_COUNT];
    u16 coins;
    u8 keys;
    u32 save_magic;
};

void add_inventory(s8 module);
void module_update(void);
void print_module_menu(void);
void print_module_hud_status(void);
void init_module_inventory(void);
s32 handle_module_inputs(void);
void control_module_menu(void);
void update_vanity(void);
void update_settings(void);

void save_bin_reset(void);
u32 obj_save_bin_read(void);
void obj_save_bin_write(void);
void obj_save_bin_count(int type);
s32 save_bin_get_flag_total(int type);

void save_marios_modules(Vec3f pos);
void load_marios_modules(void);
void marios_modules_savefile_load_position(void);

#endif