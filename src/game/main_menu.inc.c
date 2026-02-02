struct mariosModulesSaveGame gMariosModulesSave;
int gMariosModulesSaveIndex = 0;
int gMainMenuWarpLocation = 2;
int gMainMenuTitleAnimationIndex = -1;

u8 sMainMenuShowTitle = FALSE;
int sMainMenuModuleTimer = 0;

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

        if (!is_level_dungeon()) {
            tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);
        }
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
u8 gMainMenuState = MAIN_MENU_TITLE_TRANSITION_1;
u8 gMainMenuTargetState = MAIN_MENU_TITLE_TRANSITION_1;
f32 sMainMenuTransition = 1.0f;
s8 sMainMenuIndex = 0;
u16 sMainMenuSeedShaker[2];

f32 sBigTextScroll = 0.0f;

f32 sMainMenuHandPos[2] = {0.0f};
f32 sMainMenuHandTargetPos[2] = {0.0f};

char * sChangelogStr = "\
Mario's Modules v1.2\n\
\n\
Major Changes:\n\
→ Increased max framerate to 60\n\
→ Overhauled and refined module menu\n\
→ Added vanity, settings, and passive panels\n\
→ Added game saving via save blocks\n\
→ Added post-game creative mode\n\
→ Improved tutorial\n\
→ Hold Z in module menu to view minimap\n\
\n\
New Module Additions:\n\
→ Twirl (Action)\n\
→ Crouch Action (Action)\n\
→ Rotate (Action)\n\
→ Debug Monitor (Action)\n\
\n\
→ Ground Upgrade (Upgrade)\n\
→ Heat Sink (Upgrade)\n\
→ +2 (Upgrade)\n\
→ Time Extend (Upgrade)\n\
\n\
→ Red Dye (Vanity)\n\
→ Green Dye (Vanity)\n\
→ Blue Dye (Vanity)\n\
→ Yellow Dye (Vanity)\n\
→ Black Dye (Vanity)\n\
→ White Dye (Vanity)\n\
→ Tan Dye (Vanity)\n\
→ Brown Dye (Vanity)\n\
→ Pants (Vanity)\n\
→ Cap + Shirt (Vanity)\n\
→ Skin (Vanity)\n\
→ Hair (Vanity)\n\
→ Woman (Vanity)\n\
\n\
→ 60 FPS (Option)\n\
→ Camera Collision (Option)\n\
→ Widescreen (Option)\n\
→ Disable Music (Option)\n\
\n\
→ If Block (Logic)\n\
→ End Block (Logic)\n\
→ Stop (Logic)\n\
→ If Grounded (Logic)\n\
→ If Falling (Logic)\n\
→ If Touching Wall (Logic)\n\
\n\
→ Move (Passive)\n\
→ Hold (Passive)\n\
→ Defense (Passive)\n\
\n\
Minor Changes:\n\
→ Polished level visuals\n\
→ Made gameplay adjustments to level\n\
→ Added new rooms to level\n\
→ Added new stars to level\n\
→ New treasure chest type: Mystery Chest\n\
→ Added module warnings\n\
→ Hold to navigate menus added\n\
→ C<+> can push modules in menu\n\
→ Cv sends module back to inventory\n\
→ Double tap Cv sends row back to inventory\n\
→ B pushes all modules to the left\n\
→ Initial module cooldown is shorter, however...\n\
→ Some modules invoke a longer cooldown\n\
→ Re-organized module classifications\n\
→ Modules in chests are now 3D\n\
→ Can move cursor in menu even when modules are executing\n\
→ Fixed thwomp death softlock\n\
→ Fixed camera getting stuck at certain Y level\n\
\n\
Rebalances:\n\
→ Putting jumps together no longer increases jump tier\n\
→ Cap module incurs 5 second cooldown\n\
→ Cap module cap time extended to 2 secs\n\
→ Hover module changed to air platform, no longer follows Mario\n\
→ Hover module incurs .5s cooldown, grows with UPG.\n\
→ Down module behavior now consistent with Wall module";

char * sCreditsStr = "\
- CREDITS -\n\
\n\
@O@Rovertronic@@\n\
Code, Artwork, 3D Models, Level Design\n\
\n\
@O@Igorsantusa@@ and @O@Calem28@@\n\
Soundtrack\n\
From: SwApFell: I Hate You.\n\
\n\
Songs used:\n\
TITLE\n\
Mouth\n\
Ancient Kingdom\n\
TESTAMENT\n\
KING VERMILLION\n\
\n\
@O@HackerN64 Team@@\n\
HackerSM64\n\
\n\
This hack was made for\n\
a competition,\n\
Mario Jams 7: Element.";

char * sButtonsMain[] = {
    "@G@Play",
    "Credits",
    "Changelog",
    NULL,
};

char * sButtonsFile[] = {
    "Continue",
    "New Game",
    "@P@Rogue-Like",
    NULL,
};

char * sButtonsMode[] = {
    "Standard Game",
    "@O@Last Stand",
    "@G@Creative",
    NULL,
};

char * sModeDescriptions[] = {
    "The standard Mario's Modules experience. Collect @Y@15 stars@@ by finding modules and building moves to complete the game.",
    "The temple has been insulated with tin-foil. @B@A socket, @G@B socket,@@ and moving with analog is disabled. Use infinite modules to craft a Mario that can get to the end of the temple.",
    "Infinite modules, everything unlocked. No achievements.",
};

struct Achievement achievementList[] = {
    [ACHIEVEMENT_WIN] = {
        .name = "@O@Fate of the Kingdom",
        .desc = "Escape the manufacturing dungeon.",
        .rank = 0,
        .flag = 0,
    },
    [ACHIEVEMENT_STARS] = {
        .name = "@O@Mario's All Powered Up",
        .desc = "Collect all 15 stars.",
        .rank = 0,
        .flag = 1,
    },
    [ACHIEVEMENT_SHRED] = {
        .name = "@O@Thrifter",
        .desc = "Reroll a module with the Recycletron.",
        .rank = 0,
        .flag = 2,
    },
    [ACHIEVEMENT_COSMETIC] = {
        .name = "@O@Starving Artist",
        .desc = "Shred 5 cosmetic modules.",
        .rank = 0,
        .flag = 3,
    },
    [ACHIEVEMENT_ECO] = {
        .name = "@1@Eco Friendly",
        .desc = "Triple jump without any cooldown.",
        .rank = 1,
        .flag = 4,
    },
    [ACHIEVEMENT_REPEAT] = {
        .name = "@1@Threepeater",
        .desc = "Repeat 3 times during a single socket execution.",
        .rank = 1,
        .flag = 5,
    },
    [ACHIEVEMENT_FAST] = {
        .name = "@1@Supersonic",
        .desc = "Break the sound barrier. (1143+ speed)",
        .rank = 1,
        .flag = 6,
    },
    [ACHIEVEMENT_FASTSPIN] = {
        .name = "@1@Superkirby",
        .desc = "Twirl at 14 revolutions per second.",
        .rank = 1,
        .flag = 7,
    },
    [ACHIEVEMENT_HOT] = {
        .name = "@1@Overclocked and Overcooked",
        .desc = "Incur a 15+ second cooldown.",
        .rank = 1,
        .flag = 8,
    },
    [ACHIEVEMENT_FALL] = {
        .name = "@Y@Watch Me Fly, Mama!",
        .desc = "Stay airborne for 30 seconds or more.",
        .rank = 2,
        .flag = 9,
    },
    [ACHIEVEMENT_NO_HIT] = {
        .name = "@Y@You CAN Dodge Forever!",
        .desc = "Collect all 15 stars without taking damage.",
        .rank = 2,
        .flag = 10,
    },
    [ACHIEVEMENT_NO_AIR_PLATFORM] = {
        .name = "@Y@Not A Bowser In The Sky",
        .desc = "Collect all 15 stars without using the air platform module.",
        .rank = 2,
        .flag = 11,
    },
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
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
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
        case MAIN_MENU_CREDITS:
            render_main_menu_big_text(sCreditsStr);
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
    if (sMainMenuShowTitle) {
        gMainMenuTitleAnimationIndex ++;
    } else {
        gMainMenuTitleAnimationIndex --;
    }
    gMainMenuTitleAnimationIndex = CLAMP(gMainMenuTitleAnimationIndex,-1,19);
    switch(gMainMenuState) {
        case MAIN_MENU_TITLE_TRANSITION_1:
            if (sMainMenuModuleTimer++ >= 20) {
                gMainMenuTargetState = MAIN_MENU_TITLE;
                gMainMenuState = MAIN_MENU_TITLE;
                sMainMenuModuleTimer = 0;
                sMainMenuShowTitle = TRUE;
            }
            break;
        case MAIN_MENU_TITLE:
            if (sMainMenuTitleAlpha > 0) {
                if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                    gMainMenuTargetState = MAIN_MENU_TITLE_TRANSITION_2;
                    sMainMenuShowTitle = FALSE;
                }
            }
            break;
        case MAIN_MENU_TITLE_TRANSITION_2:
            if (sMainMenuModuleTimer++ >= 10) {
                gMainMenuTargetState = MAIN_MENU_MAIN;
                sMainMenuModuleTimer = 0;
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
            main_menu_handle_scroll(3);
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        gMainMenuWarpLocation = 2;
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
                    case 2:
                        gMainMenuWarpLocation = 4;
                        gMariosModulesSave.file[gMariosModulesSaveIndex].seed = (sMainMenuSeedShaker[0] | (sMainMenuSeedShaker[1] << 16));
                        level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                        gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                        gModuleTutorialState = TUTORIAL_DONE;
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