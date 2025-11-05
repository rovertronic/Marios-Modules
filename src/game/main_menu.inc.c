struct mariosModulesSaveGame gMariosModulesSave;
int gMariosModulesSaveIndex = 0;

void save_marios_modules(Vec3f pos) {
    int size = sizeof(struct mariosModulesSaveGame);

    if (gSramProbe != 0) {
        gMariosModulesSave.file[gMariosModulesSaveIndex].version = MARIOS_MODULES_GAME_VERSION;
        for (int i = 0; i < 3; i++) {
            gMariosModulesSave.file[gMariosModulesSaveIndex].pos[i] = pos[i];
        }
        gMariosModulesSave.save_magic = SAVE_MAGIC;
        gMariosModulesSave.file[gMariosModulesSaveIndex].keys = gMarioState->numKeys;
        gMariosModulesSave.file[gMariosModulesSaveIndex].coins = gMarioState->numCoins;
        bcopy(&inventory,&gMariosModulesSave.file[gMariosModulesSaveIndex].inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        bcopy(&inventoryParam,&gMariosModulesSave.file[gMariosModulesSaveIndex].inventoryParam,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));

        display_generic_message("@G@Game successfully saved.");

        play_sound(SOUND_GENERAL_HEART_SPIN, gGlobalSoundSource);
    }
}

void load_marios_modules(void) {
    int size = sizeof(struct mariosModulesSaveGame);

    if (gSramProbe != 0) {
        nuPiReadSram(0, &gMariosModulesSave, ALIGN8(size));
        if (gMariosModulesSave.save_magic == SAVE_MAGIC) {
            bcopy(&gMariosModulesSave.file[gMariosModulesSaveIndex].inventory,&inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
            bcopy(&gMariosModulesSave.file[gMariosModulesSaveIndex].inventoryParam,&inventoryParam,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        } else {
            bzero(&gMariosModulesSave,size);
        }
        gMarioState->numKeys = gMariosModulesSave.file[gMariosModulesSaveIndex].keys;
        gMarioState->numCoins = gMariosModulesSave.file[gMariosModulesSaveIndex].coins;

        tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);
    }
}

int saveBinTotal[SAVE_BIN_COUNT];

void save_bin_reset(void) {
    for (int i = 0; i < SAVE_BIN_COUNT; i++) {
        saveBinTotal[i] = 0;
    }
}

void obj_save_bin_count(int type) {
    s32 id = saveBinTotal[type];

    o->saveBinId = id%32;
    o->saveBinType = type+(id/32);

    saveBinTotal[type]++;
}

u32 obj_save_bin_read(void) {
    return (gMariosModulesSave.file[gMariosModulesSaveIndex].bin[o->saveBinType] & (1 << o->saveBinId));
}

void obj_save_bin_write(struct Object * obj) {
    gMariosModulesSave.file[gMariosModulesSaveIndex].bin[obj->saveBinType] |= (1 << obj->saveBinId);
}

s32 save_bin_get_flag_total(int type) {
    int count = 0;
    for (int i = 0; i < saveBinTotal[type]; i++) {
        if (gMariosModulesSave.file[gMariosModulesSaveIndex].bin[type+(i/32)] & (1 << (i%32))) {
            count++;
        }
    }
    return count;
}

s32 save_bin_get_max_total(int type) {
    return saveBinTotal[type];
}

void marios_modules_savefile_load_position(void) {
    if (gMariosModulesSave.save_magic == SAVE_MAGIC) {
        for (int i = 0; i < 3; i++) {
            gMarioState->pos[i] = gMariosModulesSave.file[gMariosModulesSaveIndex].pos[i];
        }
    }
}

// MARIO'S MODULES: MAIN FUCKING MENU

u8 sMainMenuTitleAlpha = 255;
u8 gMainMenuState = MAIN_MENU_TITLE;
u8 gMainMenuTargetState = MAIN_MENU_TITLE;
f32 sMainMenuTransition = 1.0f;
s8 sMainMenuIndex = 0;
u16 sMainMenuSeedShaker[2];

f32 sBigTextScroll = 0.0f;

f32 sMainMenuHandPos[2] = {0.0f};
f32 sMainMenuHandTargetPos[2] = {0.0f};

char * sChangelogStr = "\
Major Changes:\n\
* Increased max framerate to 60\n\
* Overhauled and refined module menu\n\
* Added vanity, settings, and passive panels\n\
* Added game saving via save blocks\n\
* Added post-game creative mode\n\
\n\
New Module Additions:\n\
* Twirl (Action)\n\
* Crouch Action (Action)\n\
* Debug Monitor (Action)\n\
\n\
* Ground Upgrade (Upgrade)\n\
* Heat Sink (Upgrade)\n\
\n\
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
* If Block (Logic)\n\
* End Block (Logic)\n\
* Stop (Logic)\n\
* If Grounded (Logic)\n\
* If Falling (Logic)\n\
\n\
* Move (Passive)\n\
* Hold (Passive)\n\
* Defense (Passive)\n\
\n\
Minor Changes:\n\
* Polished level visuals\n\
* Made gameplay adjustments to level\n\
* Added module warnings\n\
* Hold to navigate menus added\n\
* C<+> can push modules in menu\n\
* CV sends module back to inventory\n\
* Double tap CV sends row back to inventory\n\
* Z pushes all modules to the left\n\
* Shortened module cooldown\n\
* Re-organized module classifications\n\
* Modules in chests are now 3D\n\
* Can move cursor in menu even when modules are executing\n\
* Fixed thwomp death softlock\n\
* Fixed camera getting stuck at certain Y level\n\
\n\
Rebalances:\n\
* Putting jumps together no longer increases jump tier\n\
* Cap module incurs 4 second cooldown\n\
* Cap module cap time extended to 2 secs\n\
* Hover module changed to air platform, no longer follows Mario\n\
* Hover module incurs .5s cooldown, grows with UPG.\n\
* Down module behavior now consistent with Wall module";

char * sButtonsMain[] = {
    "@G@Play",
    "Credits",
    "Changelog",
    NULL,
};

char * sButtonsFile[] = {
    "Continue",
    "New Game",
    NULL,
};

char * sButtonsMode[] = {
    "Standard Game",
    "@P@Mystery Mania",
    "@R@Rogue",
    "@G@Creative",
    NULL,
};

void render_menu_button_list(char * btns[]) {
    int i = 0;
    char * curStr = btns[0];
    while(curStr != NULL) {
        print_utf8_boxed(curStr,160,120-(i*22),sMainMenuTransition,TRUE);
        if (i == sMainMenuIndex) {
            int sx; int sy; utf8_size(curStr, &sx, &sy);
            sMainMenuHandTargetPos[0] = 165 + (sx/2);
            sMainMenuHandTargetPos[1] = 105 + (i*22);
        }

        i++;
        curStr = btns[i];
    }

}

void render_main_menu_hand(void) {
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    print_set_envcolour(255, 255, 255, sMainMenuTransition*255.0f);

    sMainMenuHandPos[0] = approach_f32_asymptotic(sMainMenuHandPos[0],sMainMenuHandTargetPos[0], .2f);
    sMainMenuHandPos[1] = approach_f32_asymptotic(sMainMenuHandPos[1],sMainMenuHandTargetPos[1], .2f);
    print_texture(micons_small_hand_1_rgba16,16,sMainMenuHandPos[0], sMainMenuHandPos[1]);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void render_main_menu_big_text(char * str) {
    utf8_print_reset();

    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, sMainMenuTransition*180.0f);
    render_4slice(-10,241,330,0);

    gDPSetEnvColor(gDisplayListHead++, 255,255,255, sMainMenuTransition*255.0f);
    print_utf8(str,10,220+sBigTextScroll);
    sBigTextScroll -= gFrameLerpDeltaTime*(gPlayer1Controller->rawStickY/16.0f);
    int sx; int sy; utf8_size(str, &sx, &sy);

    if (sBigTextScroll > -sy - 200) {
        sBigTextScroll = -sy - 200;
    }
    if (sBigTextScroll < 0) {
        sBigTextScroll = 0;
    }
}

void render_main_menu(void) {
    switch(gMainMenuState) {
        case MAIN_MENU_TITLE:
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
            print_set_envcolour(255, 255, 255, sMainMenuTransition*255.0f);
            render_multi_image(micons_mmodules_title_rgba16, 0, 40, 320, 124, 1, 1, G_CYC_1CYCLE);
            break;
        case MAIN_MENU_MAIN:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsMain);
            break;
        case MAIN_MENU_FILE:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsFile);
            break;
        case MAIN_MENU_CHANGELOG:
            render_main_menu_big_text(sChangelogStr);
            break;
    }
}

void main_menu_handle_scroll(u8 max) {
    joystick_to_dpad();

    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        sMainMenuIndex++;
    }
    if (gPlayer1Controller->buttonPressed & U_JPAD) {
        sMainMenuIndex--;
    }

    sMainMenuIndex = (sMainMenuIndex + max)%max;
}

void logic_main_menu(void) {
    sMainMenuSeedShaker[0] = random_u16();
    sMainMenuSeedShaker[1] = random_u16();

    if (gMainMenuState != gMainMenuTargetState) {
        sMainMenuTransition -= .1f;
        if (sMainMenuTransition <= 0.0f) {
            sMainMenuTransition = 0.0f;
            gMainMenuState = gMainMenuTargetState;
        }
    } else {
        sMainMenuTransition = CLAMP(sMainMenuTransition+.1f,0.0f,1.0f);
    }
    switch(gMainMenuState) {
        case MAIN_MENU_TITLE:
            if (sMainMenuTitleAlpha > 0) {
                if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                    gMainMenuTargetState = MAIN_MENU_MAIN;
                }
            }
            break;
        case MAIN_MENU_MAIN:
            main_menu_handle_scroll(3);
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        gMainMenuTargetState = MAIN_MENU_FILE;
                        break;
                    case 1:
                        gMainMenuTargetState = MAIN_MENU_CREDITS;
                        break;
                    case 2:
                        gMainMenuTargetState = MAIN_MENU_CHANGELOG;
                        break;
                }
            }
            break;
        case MAIN_MENU_FILE:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                        gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                        gModuleTutorialState = TUTORIAL_DONE;
                        break;
                    case 1:
                        gMainMenuTargetState = MAIN_MENU_OPENING_CUTSCENE;
                        gMainMenuState = MAIN_MENU_OPENING_CUTSCENE;
                        gMariosModulesSave.file[gMariosModulesSaveIndex].seed = (sMainMenuSeedShaker[0] | (sMainMenuSeedShaker[1] << 16));
                        tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);
                        saveBinTotal[SAVE_BIN_CHESTS] = 0;
                        play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, SEQ_MM64_INTRO), 0);
                        break;
                }
            }
            break;
        case MAIN_MENU_CREDITS:
        case MAIN_MENU_CHANGELOG:
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_MAIN;
            }
            break;
    }
}