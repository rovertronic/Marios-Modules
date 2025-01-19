#ifndef MODULE_H
#define MODULE_H

#include "types.h"

extern u8 gModuleMenuOpen;

struct module_execution_thread {
    u8 mod;
    u8 x;
    u8 y;
    u8 timer;

    u8 executing:1;
    u8 halted:1;
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
    void (*func)(struct module_execution_thread * met);
};

struct module_type_info {
    char * name;
    u8 color[3]
};

enum module_type {
    MTYPE_MOVE,
    MTYPE_COND,
    MTYPE_BUFF,
    MTYPE_INPUT,
};

#define MOD_EMPTY -1
enum module_id {
    MOD_BUTTON_A,
    MOD_BUTTON_B,
    MOD_JUMP,
    MOD_POW,
    MOD_HIT_GROUND,
    MOD_HIT_WALL,
};

void module_update(void);
void print_module_menu(void);
void init_module_inventory(void);
s32 handle_module_inputs(void);

#endif