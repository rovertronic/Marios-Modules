#ifndef MODULE_H
#define MODULE_H

#include "types.h"

#define MARIOS_MODULES_GAME_VERSION 1

#define INVENTORY_SLOTS_Y 50
#define INVENTORY_SLOTS_X 8

extern u8 gModuleMenuOpen;
extern u8 gModuleMenuMode;
extern s8 gRecycleChestContent;
extern s8 gRecycledModule;
extern u8 gGameSettings[];
extern Vec3f gModulePreviewPos;

extern f32 gMessageDisplayTimer;

extern s8 gMysteryModuleState;
extern s8 gMysteryModuleSelection;
extern s8 gMysteryModuleChoice[2];

struct module_panel {
    char * name;
    u8 offset;
    u8 size;
    s32 (*unlock)(void);
};

enum {
    PANEL_SETTINGS,
    PANEL_ACTIONS,
    PANEL_PASSIVE,
    PANEL_STORAGE,
    PANEL_VANITY,
    PANEL_CREATIVE,
    INVENTORY_PANEL_CT,
};

struct module_execution_thread {
    u32 input;
    u8 mod;
    u8 time_mod;
    u8 x;
    u8 y;
    u8 spd;
    u8 element;

    u8 executing:1;
    u8 halted:1;
    u8 cooldown:1;
    u8 input_notify:1;
    u8 manual:1;
    u8 mario_ground_listener:1;
    u8 jump_tier:2;

    u8 ifbool:1;
    u8 doaircooldown:1;
    u8 debug_monitor:1;
    u8 begin:1;

    u16 condition_flags;
    u8 condition_count;

    u8 landing_count;

    u16 used_flags;
    u16 timer;
    s16 cooltime;
    s16 currentAngle;
    void * extra_data;
    u8 option;
};

enum module_execution_ids {
    MODULE_EXEC_A,
    MODULE_EXEC_B,
    MODULE_EXEC_VANITY,
    MODULE_EXEC_SETTINGS,
    MODULE_EXEC_PASSIVE,
    MODULE_EXEC_COUNT,
};

enum module_menu_modes {
    MODULE_MENU_MODE_NORMAL,
    MODULE_MENU_MODE_RECYCLE,
};

#define GROUNDED (((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING)||((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_AUTOMATIC))

enum module_loot_ids {
    LOOT_NONE,
    LOOT_VANITY,
    LOOT_TIER_1,
    LOOT_TIER_2,
};

struct module_info {
    u8 type;
    u8 unchainable:1;
    u8 creative:1;
    u8 elementable:1;
    u8 loot_tier:2;
    char * name;
    void * tex;
    char * desc;
    char * upg_desc;
    void (*func)(struct module_execution_thread * met, u8 call_context);
    void * extra_data;
    char ** options;
    f32 cooldown;
};

enum module_call_context {
    MCC_INVOKE,
    MCC_HALTED,
};

struct module_type_info {
    char * text_color;
    char * name;
    u8 color[3];
};

enum module_type {
    MTYPE_MOVE,
    MTYPE_COND,
    MTYPE_BUFF,
    MTYPE_INPUT,
    MTYPE_NONMOD,
    MTYPE_SETTINGS,
    MTYPE_LOGIC,
    MTYPE_ELEMENT,
    MTYPE_PASSIVE,
    MTYPE_VANITY,
    MTYPE_COUNT,
};

#define MOD_EMPTY -1
enum module_id {
    MOD_BUTTON_A,
    MOD_BUTTON_B,
    MOD_JUMP,
    MOD_POW,
    MOD_POW2,
    MOD_HIT_GROUND,
    MOD_HIT_WALL,
    MOD_TIMER,
    MOD_ATTACK,
    MOD_INPUT,
    MOD_SPD,
    MOD_PLATFORM,
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
    MOD_TAN,
    MOD_BROWN,
    MOD_WOMAN,
    MOD_CAMERA_COLLISION,
    MOD_WIDESCREEN,
    MOD_60HZ,
    MOD_AA,
    MOD_WRAP,
    MOD_TORNADO,
    MOD_ICE,
    MOD_FLAME,
    MOD_STOP,
    MOD_REPEAT,
    MOD_IF,
    MOD_ENDBLOCK,
    MOD_FLIP_VEL,
    MOD_ZACTION,
    MOD_GROUND_UPG,
    MOD_IF_FLOOR,
    MOD_IF_DOWN,
    MOD_PASSIVE,
    MOD_IF_INPUT,
    MOD_COOL,
    MOD_HOLD,
    MOD_DEFENSE,
    MOD_NOMUSIC,
    MOD_ROTATE,
    MOD_MONITOR,
    MOD_IF_WALL,
    MOD_TIME_EXTEND,
    MOD_NONMOD_STAR,
    MOD_NONMOD_MYSTERY_CHEST,
    MOD_MINIMAP,
    MOD_IF_SENSOR,
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
    u16 whitelist_flags;
    u8 wrap;
};

#define WHITELIST_VANITY ((1 << MTYPE_VANITY) | (1 << MTYPE_LOGIC) | (1 << MTYPE_COND))
#define WHITELIST_ACTION ((1 << MTYPE_MOVE) | (1 << MTYPE_COND) | (1 << MTYPE_BUFF) | (1 << MTYPE_LOGIC))
#define WHITELIST_PASSIVE ((1 << MTYPE_MOVE) | (1 << MTYPE_COND) | (1 << MTYPE_BUFF) | (1 << MTYPE_VANITY) | (1 << MTYPE_LOGIC) | (1 << MTYPE_PASSIVE))

enum {
    SETTING_60HZ,
    SETTING_CAMERA_COLLISION,
    SETTING_WIDE,
    SETTING_AA,
    SETTING_NOMUSIC,
    SETTING_COUNT,
};

enum {
    PASSIVE_FLAG_RUN,
    PASSIVE_FLAG_HOLD,
    PASSIVE_FLAG_DEFENSE,
    PASSIVE_FLAG_MINIMAP,
};

enum {
    SAVE_BIN_STARS,
    SAVE_BIN_CHESTS,
    SAVE_BIN_CHESTS2,
    SAVE_BIN_DOORS,
    SAVE_BIN_COUNT,
};

#define SAVE_MAGIC 0x0203DD10 //my favorite rom address
struct mariosModulesSaveFile {
    u8 version;
    Vec3s pos;
    s8 inventory[INVENTORY_SLOTS_Y*INVENTORY_SLOTS_X];
    s8 inventoryParam[INVENTORY_SLOTS_Y*INVENTORY_SLOTS_X];
    u32 bin[SAVE_BIN_COUNT];
    u16 coins;
    u8 keys;
    u32 flags;
    u32 room_discover_flags;
    u32 seed;
};

#define SAVE_FLAG_PASSIVE (1 << 0)

struct mariosModulesSaveGame {
    struct mariosModulesSaveFile file[3];
    u32 achievements;
    u32 save_magic;
};

extern struct mariosModulesSaveGame gMariosModulesSave;
extern int gMariosModulesSaveIndex;

// Main Menu
extern u8 gMainMenuState;
extern u8 gMainMenuTargetState;
extern u8 gModuleTutorialState;
extern int gMainMenuWarpLocation;
extern int gMainMenuTitleAnimationIndex;

enum {
    MAIN_MENU_TITLE_TRANSITION_1,
    MAIN_MENU_TITLE,
    MAIN_MENU_TITLE_TRANSITION_2,
    MAIN_MENU_MAIN,
    MAIN_MENU_FILE,
    MAIN_MENU_CREDITS,
    MAIN_MENU_CHANGELOG,
    MAIN_MENU_OPENING_CUTSCENE,
    MAIN_MENU_LEVEL_WARP_CONTINUE,
    MAIN_MENU_CLOSED,
};

enum {
    TUTORIAL_WAIT_FOR_MODULE_COLLECT,
    TUTORIAL_PRESS_START,
    TUTORIAL_MOVE_CURSOR,
    TUTORIAL_PICK_UP_MOD,
    TUTORIAL_PLACE_MOD,
    TUTORIAL_GET_STAR,
    TUTORIAL_DONE,
    TUTORIAL_DISCONNECTED,
};

struct ScreenMessage {
    char text[80];
    char * stringId;
    f32 time;
    s8 tutorialHoldId;
};


struct Achievement {
    char * name;
    char * desc;
    u16 rank;
    u16 flag;
};

enum {
    ACHIEVEMENT_WIN,
    ACHIEVEMENT_STARS,
    ACHIEVEMENT_SHRED,
    ACHIEVEMENT_COSMETIC,
    ACHIEVEMENT_ECO,
    ACHIEVEMENT_REPEAT,
    ACHIEVEMENT_FAST,
    ACHIEVEMENT_FASTSPIN,
    ACHIEVEMENT_HOT,
    ACHIEVEMENT_FALL,
    ACHIEVEMENT_NO_HIT,
    ACHIEVEMENT_NO_AIR_PLATFORM,
    ACHIEVEMENT_COUNT,
};


void display_generic_message(char * str);
void display_tutorial_message(char * str, u8 tutorialId);


void render_main_menu(void);
void logic_main_menu(void);

Gfx *geo_module_material(s32 callContext, struct GraphNode *node, void *context);

s8 get_inventory(int x, int y);
void module_log_message(struct module_execution_thread * met,  char * logmsg, int num);
void module_log_clear(void);
void add_met_condition(struct module_execution_thread * met, s32 condition);

void add_inventory(s8 module);
void display_module_message(s8 id);
void module_update(void);
void print_module_menu(void);
void print_module_hud_status(void);
void print_module_generic_message(void);
void print_mini_map(void);
void init_module_inventory(void);
s32 handle_module_inputs(void);
void control_module_menu(void);
void update_vanity(void);
void update_settings(void);

void save_bin_reset(void);
u32 obj_save_bin_read(void);
void obj_save_bin_write(struct Object * obj);
void obj_save_bin_count(int type);
s32 save_bin_get_flag_total(int type);
s32 save_bin_get_max_total(int type);

void save_marios_modules(Vec3f pos);
void load_marios_modules(void);
void marios_modules_savefile_load_position(void);

#endif