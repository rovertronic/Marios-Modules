#include "lore.inc.c"

extern Texture texture_hud_char_star[];

struct mariosModulesSaveGame gMariosModulesSave;
struct mariosModulesSaveFile gMariosMoudlesStats;
int gMariosModulesSaveIndex = 0;
int gMainMenuWarpLocation = -1;
int gMainMenuTitleAnimationIndex = -1;

u8 gResultsScreenDisplay = 0;

u8 sMainMenuShowTitle = FALSE;
u8 sMainMenuDoFade = FALSE;
int sMainMenuModuleTimer = 0;

s8 sJournalEntryIndex = 0;

void save_delete_file(int fileIndex) {
    int size = sizeof(struct mariosModulesSaveGame);
    gMariosModulesSave.file[gMariosModulesSaveIndex].flags = 0;

    if (gModuleCreativeEnabled) {return;}

    nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));
}

void save_marios_modules_new_game(u32 seed, s8 level) {
    int size = sizeof(struct mariosModulesSaveGame);
    int sizeFile = sizeof(struct mariosModulesSaveFile);

    bzero(&gMariosModulesSave.file[gMariosModulesSaveIndex],sizeFile);
    init_module_inventory();
    bcopy(&inventory,&gMariosModulesSave.file[gMariosModulesSaveIndex].inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
    bcopy(&inventoryParam,&gMariosModulesSave.file[gMariosModulesSaveIndex].inventoryParam,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);

    gMariosModulesSave.save_magic = SAVE_MAGIC;
    gMariosModulesSave.file[gMariosModulesSaveIndex].seed = seed;
    gMariosModulesSave.file[gMariosModulesSaveIndex].level = level;
    gMariosModulesSave.file[gMariosModulesSaveIndex].lives = 3;

    gMariosModulesSave.file[gMariosModulesSaveIndex].flags = SAVE_FLAG_EXIST;

    if (gModuleCreativeEnabled) {return;}

    nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));
}

void save_marios_modules_coins(void) {
    // Run this function when fail
    int size = sizeof(struct mariosModulesSaveGame);
    gMariosModulesSave.file[gMariosModulesSaveIndex].coins = gMarioState->numCoins;

    if (gModuleCreativeEnabled) {return;}

    nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));
}

void save_marios_modules_lose_life(void) {
    // Run this function when fail
    int size = sizeof(struct mariosModulesSaveGame);
    gMariosModulesSave.file[gMariosModulesSaveIndex].lives --;

    bzero(&gMariosModulesSave.file[gMariosModulesSaveIndex].bin[0],4*SAVE_BIN_COUNT);

    if (gModuleCreativeEnabled) {return;}

    nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));
}

void save_marios_modules_silent(Vec3f pos) {
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

        if (gModuleCreativeEnabled) {return;}

        nuPiWriteSram(0, &gMariosModulesSave, ALIGN8(size));
    }
}

void save_marios_modules(Vec3f pos) {
    if (gSramProbe != 0) {
        save_marios_modules_silent(pos);
        display_generic_message("@G@Game successfully saved.");
        play_sound(SOUND_GENERAL_HEART_SPIN, gGlobalSoundSource);
    }
}

void load_marios_modules_data_only(void) {
    int size = sizeof(struct mariosModulesSaveGame);
    
    if (gSramProbe != 0) {
        nuPiReadSram(0, &gMariosModulesSave, ALIGN8(size));

        if (gMariosModulesSave.save_magic != SAVE_MAGIC) {
            bzero(&gMariosModulesSave, size);
            gMariosModulesSave.save_magic = SAVE_MAGIC;
        }
    }
}

void load_marios_modules(void) {
    if (gMainMenuState == 0) {return;}
    if (gCurrLevelNum == LEVEL_GAMEOVER) {return;}
    if (gModuleCreativeEnabled) {return;}

    int size = sizeof(struct mariosModulesSaveGame);
    int sizeFile = sizeof(struct mariosModulesSaveFile);

    if (gSramProbe != 0) {
        if (gMariosModulesSave.save_magic != SAVE_MAGIC) {
            bzero(&gMariosModulesSave, size);
            gMariosModulesSave.save_magic = SAVE_MAGIC;
        }

        nuPiReadSram(0, &gMariosModulesSave, ALIGN8(size));
        if (gMariosModulesSave.file[gMariosModulesSaveIndex].flags & SAVE_FLAG_EXIST) {
            bcopy(&gMariosModulesSave.file[gMariosModulesSaveIndex].inventory,&inventory,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
            bcopy(&gMariosModulesSave.file[gMariosModulesSaveIndex].inventoryParam,&inventoryParam,INVENTORY_SLOTS_X*INVENTORY_SLOTS_Y);
        } else {
            bzero(&gMariosModulesSave.file[gMariosModulesSaveIndex],sizeFile);
        }
        gMarioState->numKeys = gMariosModulesSave.file[gMariosModulesSaveIndex].keys;
        gMarioState->numCoins = gMariosModulesSave.file[gMariosModulesSaveIndex].coins;

        if (!is_level_dungeon()) {
            tinymt32_init(&gGlobalRandomState,gMariosModulesSave.file[gMariosModulesSaveIndex].seed);
        }

        update_creative_inventory();
    }
}

int saveBinTotal[SAVE_BIN_COUNT];

void save_bin_reset(void) {
    for (int i = 0; i < SAVE_BIN_COUNT; i++) {
        saveBinTotal[i] = 0;
    }
}

void obj_save_bin_count(int type) {
    if (gCurrLevelNum == LEVEL_PITSTOP) {return;}
    s32 id = saveBinTotal[type];

    o->saveBinId = id%32;
    o->saveBinType = type+(id/32);

    saveBinTotal[type]++;
}

u32 obj_save_bin_read(void) {
    if (gCurrLevelNum == LEVEL_PITSTOP) {return 0;}
    return (gMariosModulesSave.file[gMariosModulesSaveIndex].bin[o->saveBinType] & (1 << o->saveBinId));
}

void obj_save_bin_write(struct Object * obj) {
    if (obj->saveBinId < 0) {return;} // Can happen during the roguelike shop
    if (gCurrLevelNum == LEVEL_PITSTOP) {return;}
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

s32 save_bin_get_star_all_levels(void) {
    int count = 0;
    count += save_bin_get_flag_total(SAVE_BIN_STARS);
    count += gMariosModulesSave.file[gMariosModulesSaveIndex].accumulatedStars;
    return count;
}

s32 save_bin_get_max_total(int type) {
    return saveBinTotal[type];
}

void marios_modules_savefile_load_position(void) {
    if (gMariosModulesSave.file[gMariosModulesSaveIndex].pos[0] == 0 &&
        gMariosModulesSave.file[gMariosModulesSaveIndex].pos[1] == 0) {
            return;
    }
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

f32 sBigTextScroll = 0.0f;

f32 sMainMenuHandPos[2] = {0.0f};
f32 sMainMenuHandTargetPos[2] = {0.0f};

int sMainMenuSongIndex = 0;
int sMainMenuLastSongIndex = 0;

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

char * sModeDescriptions[] = {
    "The standard Mario's Modules experience. Collect @Y@15 stars@@ by finding modules and building moves to complete the game.",
    "The temple has been insulated with tin-foil. @B@A socket, @G@B socket,@@ and moving with analog is disabled. Use infinite modules to craft a Mario that can get to the end of the temple.",
    "Infinite modules, everything unlocked. No achievements.",
};


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
@O@Code, Artwork, 3D Models, Level Design@@\n\
By: Rovertronic\n\
\n\
@O@Original Sound Track@@\n\
By: ornevelder\n\
\n\
Guitar by: Amon26\n\
Additional Voice Samples by: mermaidglade\n\
\n\
@O@Texture Sources@@\n\
Majora's Mask\n\
BroDute\n\
Vanilla SM64\n\
\n\
@O@Tools Used@@\n\
HackerSM64\n\
Blender\n\
RealWorld Paint\n\
GitHub\n\
FFmpeg\n\
Davinci Resolve";

char * sBetaInfo = "\
Mario's Modules 2 roguelite beta test\n\
N64 Save Type - SRAM 128\n\
Use this or roguelite will break, loads file every level init\n\
\n\
Notes / Known issues:\n\
1) Game over screen is scuffed, just reset\n\
2) Baron's Bounty visuals are WIP\n\
3) Only 2 levels\n\
4) False coin counter when reloading game\n\
5) N64 Only - Scuffed Minimap transform visuals";

char * sButtonsMain[] = {
    "@G@Play",
    "Extra",
    "Credits",
    //"Beta Test Info",
    NULL
};

char * sButtonsExtra[] = {
    "Listen to Soundtrack",
    "View Meta Progression",
    "View Journal Entries",
    "@1@??? - Locked",
    NULL
};

char * sButtonsMoreWaysToPlay[] = {
    "Sandbox Mode",
    "SurvAI.exe",
    NULL,
};

char * sButtonsOST[] = {
    "Play",
    "Stop",
    NULL
};

char * sButtonsFile[] = {
    "File A",
    "File B",
    "File C",
    NULL
};

char * sButtonsMode[] = {
    "@B@Manusanctuary Escape@@",
    "@P@Crystal Quest@@",
    NULL
};

char * sButtonsFileAction[] = {
    "Continue",
    "Erase",
    NULL
};

char * sButtonsFileCompleteAction[] = {
    "Show Results",
    "Erase",
    NULL
};

char * sButtonsYesNo[] = {
    "No",
    "Yes",
    NULL
};

char * sButtonsCreativeLevels[] = {
    "Manusanctuary",
    "Oasis Outpost",
    "Warped Castle",
    "Shattered Planet's Edge",
    NULL
};

struct SongEntry sSongEntries[] = {
    {.desc = "RAM Check\nBy: ornevelder",
    .seq = SEQ_MM64_TITLE,
    .unlock = -1},
    {.desc = "Initializing...\nBy: ornevelder",
    .seq = SEQ_MM64_INTRO,
    .unlock = -1},
    {.desc = "Before\nBy: ornevelder",
    .seq = SEQ_MM64_TUTORIAL,
    .unlock = 0},
    {.desc = "Mechanism\nBy: ornevelder",
    .seq = SEQ_MENU_TITLE_SCREEN,
    .unlock = 0},
    {.desc = "Augment\nBy: ornevelder",
    .seq = SEQ_MENU_TITLE_SCREEN,
    .unlock = 0},
    {.desc = "Another Module\nBy: ornevelder",
    .seq = SEQ_MENU_TITLE_SCREEN,
    .unlock = 0},
    {.desc = "Surge Protector\nBy: ornevelder",
    .seq = SEQ_UH,
    .unlock = 0},
    {.desc = "Overclocked\nBy: ornevelder",
    .seq = SEQ_WIGGLER,
    .unlock = 0},

    {.desc = "Heat Sink\nBy: ornevelder",
    .seq = SEQ_MENU_TITLE_SCREEN,
    .unlock = 1},
    {.desc = "Shop\nBy: ornevelder",
    .seq = SEQ_SHOP,
    .unlock = 1},
    {.desc = "Nostalgia.z64\nBy: ornevelder",
    .seq = SEQ_NOSTALGIA,
    .unlock = 1},
    {.desc = "Reflect\nBy: ornevelder",
    .seq = SEQ_MENU_TITLE_SCREEN,
    .unlock = 1},
    {.desc = "Beyond\nBy: ornevelder",
    .seq = SEQ_FAUXIMEDES,
    .unlock = 1},
    {.desc = "Null\nBy: ornevelder",
    .seq = SEQ_TENSE,
    .unlock = 1},
};

#define SONG_COUNT (sizeof(sSongEntries) / sizeof(sSongEntries[0]))

// PERSONALIZATION SURVEY

char * sSurveyFruit[] = {
    "Choose your favorite fruit.",
    "Apple",
    "Orange",
    "Banana",
    "Watermelon",
    "Blueberry",
    "Grape",
    NULL,
};

char * sSurveyPuzzle[] = {
    "Do you prefer puzzles or action?",
    "Puzzles",
    "Action",
    NULL,
};

/*
char * sSurveyMusic[] = {
    "What kind of music do you like?",
    "A funky jam.",
    "A holiday jam.",
    "An exciting jam.",
    "A relaxing jam.",
    "A spicy jam.",
    "A foreboding jam.",
    NULL,
};
*/

char * sSurveyAge[] = {
    "How old are you?",
    "Over 18",
    "Under 18",
    NULL,
};

char * sSurveyWeapon[] = {
    "Weapon of choice?",
    "Gun",
    "Mace",
    "Flamethrower",
    NULL,
};

char * sSurveySurprise[] = {
    "Do you like surprises?",
    "I hate surprises.",
    "I like some surprises.",
    "Novelty is my first name.",
    NULL,
};

/*
char * sSurveyTutorial[] = {
    "Do you know how to play Mario's Modules already?",
    "No",
    "Yes",
    NULL,
};
*/

char * sSurveyNeuro[] = {
    "Which condition best describes you?",
    "Neurotypical",
    "Autism",
    "ADHD",
    NULL,
};

char * sSurveySize[] = {
    "What level of scale do you appreciate most?",
    "The Micro",
    "The Macro",
    "Somewhere in-between",
    NULL,
};

char * sSurveyEnd[] = {
    "You accept everything that will happen from now on.",
    "I agree",
    NULL,
};

char ** sSurveyEntries[] = {
    sSurveyFruit,
    sSurveyPuzzle,
    sSurveyAge,
    sSurveyWeapon,
    sSurveySurprise,
    //sSurveyTutorial,
    sSurveyNeuro,
    sSurveySize,
    sSurveyEnd,
    NULL,
};

void render_menu_button_list(char * btns[]) {
    int i = 0;
    char * curStr = btns[0];
    while(curStr != NULL) {
        gHighlightUtf8Box = 0;
        if (i == sMainMenuIndex) {
            int sx; int sy; utf8_size(curStr, &sx, &sy);
            sMainMenuHandTargetPos[0] = 165 + (sx/2);
            sMainMenuHandTargetPos[1] = 85 + (i*22);
            gHighlightUtf8Box = 50;
        }

        print_utf8_boxed(curStr,160,140-(i*22),sMainMenuTransition,TRUE);

        i++;
        curStr = btns[i];
    }
    gHighlightUtf8Box = 0;

}

s32 menu_button_count(char * btns[]) {
    int i = 0;
    char * curStr = btns[0];
    while(curStr != NULL) {
        i++;
        curStr = btns[i];
    }
    return i-1;
}

void render_survey(char ** surveyStr) {
    if (surveyStr == NULL) {return;}
    print_utf8_boxed(surveyStr[0],160,180,sMainMenuTransition,TRUE);
    render_main_menu_hand();
    render_menu_button_list(&(surveyStr[1]));
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

    str = utf8_autonewline(str,300);

    print_utf8(str,10,220+sBigTextScroll);
    sBigTextScroll -= gFrameLerpDeltaTime*(gPlayer1Controller->rawStickY/16.0f);
    int sx; int sy; utf8_size(str, &sx, &sy);
    sy -= 16;

    if (sBigTextScroll > -sy - 200) {
        sBigTextScroll = -sy - 200;
    }
    if (sBigTextScroll <= -sy - 201) {
        grok_imagine_generate_rick_and_morty_eating_poop(280,10);
    }
    if (sBigTextScroll < 0) {
        sBigTextScroll = 0;
    }
}

void render_mode_info(int mode) {
    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, sMainMenuTransition*160.0f);
    render_4slice(25,82,33+260,25);

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
    char * str;
    switch(mode) {
        case 0:
            str = "Mario's Modules 2 campaign mode. "
            "Recommended for starting players! Chapter 1 of the MM2 story.";
            break;
        case 1:
            str = "Mario's Modules 2 roguelike mode. Progress through 3 randomly generated levels."
            " Only 3 lives, then permadeath. Chapter 2 of the MM2 story.";
            break;
    }
    print_utf8(utf8_autonewline(str,260), 30, 64);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
}

void render_mwtp_info(int mode) {
    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, sMainMenuTransition*160.0f);
    render_4slice(25,82,33+260,25);

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
    char * str;
    switch(mode) {
        case 0:
            str = "Drop into any map. Access to an unlimited supply of every module in the game.";
            break;
        case 1:
            str = "Take a survey. Once complete, the submission will be used to generate a personalized adventure.";
            break;
    }
    print_utf8(utf8_autonewline(str,260), 30, 64);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
}

void render_song_info(int song) {
    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, sMainMenuTransition*160.0f);
    render_4slice(25,82,33+260,25);

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
    char * str = sSongEntries[sMainMenuSongIndex].desc;
    s8 unlockValue = sSongEntries[sMainMenuSongIndex].unlock;
    if (unlockValue > -1 && !save_get_meta_flag(METAFLAGS_COMPLETION, unlockValue) ) {
        str = "@1@Not Yet Unlocked";
    }
    print_utf8(utf8_autonewline(str,260), 30, 64);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
}

void render_menu_fileinfo(void) {
    char printBuffer[200];

    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, sMainMenuTransition*160.0f);
    render_4slice(25,82,33+260,25);

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 255,255,255,255);
    char * str;
    if (gMariosModulesSave.file[sMainMenuIndex].flags & SAVE_FLAG_EXIST) {
        char * modeStr = "@B@Manusanctuary Escape@@";
        char * lvStr = "";
        if (gMariosModulesSave.file[sMainMenuIndex].level != -1) {
            modeStr = "@P@Crystal Quest@@";
            switch(gMariosModulesSave.file[sMainMenuIndex].level) {
                case 0:
                    lvStr = "Level: Oasis Outpost (1)\n";
                    break;
                case 1:
                    lvStr = "Level: Warped Castle (2)\n";
                    break;
                case 2:
                    lvStr = "Level: Shattered Planet's Edge (3)\n";
                    break;
            }
        }
        if (gMariosModulesSave.file[sMainMenuIndex].flags & SAVE_FLAG_COMPLETE) {
            lvStr = "@Y@Completed@@\n";
        }
        sprintf(printBuffer,"Mode: %s\n%sTime: %s",
        modeStr,
        lvStr,
        get_game_time_str(sMainMenuIndex));
        str = &printBuffer;
    } else {
        str = "@O@New File";
    }
    print_utf8(str, 30, 64);
    gSPDisplayList(gDisplayListHead++, mat_revert_micons_sm64ds_latin_layer1);
}

void render_main_menu(void) {
    switch(gMainMenuState) {
        case MAIN_MENU_TITLE:
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

            gDPSetEnvColor(gDisplayListHead++, 0,0,0, 255 * sMainMenuTransition);
            gSPDisplayList(gDisplayListHead++, rogo_Plane_001_mesh);
            break;
        case MAIN_MENU_MAIN:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsMain);
            break;
        case MAIN_MENU_TITLE_TRANSITION_2:
            if (gMainMenuTargetState == MAIN_MENU_MAIN) {
                sMainMenuDoFade = TRUE;
            }
            break;
        case MAIN_MENU_FILE:
            print_utf8_boxed("Select a file.",160,180,sMainMenuTransition,TRUE);
            render_main_menu_hand();
            render_menu_button_list(&sButtonsFile);

            render_menu_fileinfo();
            break;
        case MAIN_MENU_MODE:
            print_utf8_boxed("Select a mode.",160,180,sMainMenuTransition,TRUE);
            render_main_menu_hand();
            render_menu_button_list(&sButtonsMode);

            render_mode_info(sMainMenuIndex);
            break;
        case MAIN_MENU_CHANGELOG:
            render_main_menu_big_text(sBetaInfo);
            break;
        case MAIN_MENU_CREDITS:
            render_main_menu_big_text(sCreditsStr);
            break;
        case MAIN_MENU_FILE_ACTION:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsFileAction);
            break;
        case MAIN_MENU_FILE_COMPLETE_ACTION:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsFileCompleteAction);
            break;
        case MAIN_MENU_FILE_ERASE:
            print_utf8_boxed("Are you sure you want to erase file?",160,180,sMainMenuTransition,TRUE);
            render_main_menu_hand();
            render_menu_button_list(&sButtonsYesNo);
            break;
        case MAIN_MENU_EXTRA:;
            s32 unlockedCreative = save_get_meta_flag(METAFLAGS_COMPLETION,0) || save_get_meta_flag(METAFLAGS_COMPLETION,1);
            if (unlockedCreative) {
                sButtonsExtra[3] = "More Ways To Play";
            }
            render_main_menu_hand();
            render_menu_button_list(&sButtonsExtra);
            break;
        case MAIN_MENU_MORE_WAYS_TO_PLAY:
            render_mwtp_info(sMainMenuIndex);

            render_main_menu_hand();
            render_menu_button_list(&sButtonsMoreWaysToPlay);

            if (gMainMenuTargetState == MAIN_MENU_SURVEY) {
                sMainMenuDoFade = TRUE;
            }
            break;
        case MAIN_MENU_SOUNDTRACK:
            render_song_info(0);

            render_main_menu_hand();
            render_menu_button_list(&sButtonsOST);

            print_utf8_boxed("Choose song with @<Y@←@Y@C@@ and @Y@C@Y>@→@@.",160,180,sMainMenuTransition,TRUE);
            break;
        case MAIN_MENU_FILE_VIEW:
            render_results_screen(4);
            break;
        case MAIN_MENU_JOURNAL_ENTRIES:;
            char entryStr[30];

            s32 unlocked = TRUE;
            char * lockedText = "@1@Locked - Star %d of Manusanctuary Escape";
            if (sJournalEntryIndex > 2 && sJournalEntryIndex < 18) {
                unlocked = save_get_meta_flag(METAFLAGS_CAMPAIGN_STARS,sJournalEntryIndex-3);
            } else if (sJournalEntryIndex == 18) {
                lockedText = "@1@Locked - Complete Crystal Quest";
                unlocked = save_get_meta_flag(METAFLAGS_COMPLETION, 1);
            } else if (sJournalEntryIndex == 19){
                lockedText = "@1@Locked - Complete Manusanctuary Escape";
                unlocked = save_get_meta_flag(METAFLAGS_COMPLETION, 0);            
            }

            if (!unlocked) {
                sprintf(entryStr,lockedText,sJournalEntryIndex-2);
                render_main_menu_big_text(entryStr);
            } else {
                render_main_menu_big_text(gLoreEntries[sJournalEntryIndex]);
            }
            sprintf(entryStr,"@<Y@←@Y@C@@ Entry %d/20 @Y@C@Y>@→@@",sJournalEntryIndex+1);
            print_utf8_boxed(entryStr,10,10,sMainMenuTransition,FALSE);
            break;
        case MAIN_MENU_META_PROGRESSION:
            gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
            gDPSetEnvColor(gDisplayListHead++, 0,0,0, 180 * sMainMenuTransition);
            render_4slice(20,220,300,20);

            gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);

            char resultScreenStr[100];

            utf8_print_reset();
            print_utf8("Manusanctuary Escape Stars:",30,200);
            print_utf8("Crystal Quest Stars:",30,160);
            print_utf8("Completion:",180,160);
            print_utf8("Discovered Modules:",30,120);
            sprintf(resultScreenStr,"Discovered Rooms: %d/35",
                save_tally_meta_flag(METAFLAGS_ROOMS,METAFLAGS_ROOMS_4));
            print_utf8(resultScreenStr,30,30);

            gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
            // Campaign
            for (int i = 0; i < 15; i++) {
                if (save_get_meta_flag(METAFLAGS_CAMPAIGN_STARS,i)) {
                    gDPSetEnvColor(gDisplayListHead++, 255,255,255, 255);
                } else {
                    gDPSetEnvColor(gDisplayListHead++, 50,50,50, 255);
                }
                print_texture(texture_hud_char_star,16,30 + (16*i),40);
            }
            // Rogue
            for (int i = 0; i < 8; i++) {
                if (save_get_meta_flag(METAFLAGS_ROGUE_STARS,i)) {
                    gDPSetEnvColor(gDisplayListHead++, 255,255,255, 255);
                } else {
                    gDPSetEnvColor(gDisplayListHead++, 50,50,50, 255);
                }
                print_texture(texture_hud_char_star,16,30 + (16*i),80);
            }
            // Completion
            for (int i = 0; i < 2; i++) {
                if (save_get_meta_flag(METAFLAGS_COMPLETION,i)) {
                    gDPSetEnvColor(gDisplayListHead++, 255,255,255, 255);
                } else {
                    gDPSetEnvColor(gDisplayListHead++, 50,50,50, 255);
                }
                print_texture(texture_hud_char_star,16,180 + (16*i),80);
            }
            // Discovered Modules
            int count = 0;
            for (int i = 0; i < MOD_COUNT; i++) {
                int x = 30+(16*(count%16));
                int y = 120+(16*(count/16));
                if (module_infos[i].creative) {
                    if (save_get_meta_flag(METAFLAGS_MODULES,module_infos[i].meta_flag)) {
                        print_module(i,x,y,0);
                    } else {
                        gDPSetEnvColor(gDisplayListHead++, 50,50,50, 255);
                        print_texture(micons_piece_rgba16,32,x,y);
                    }
                    count++;
                }
            }
            break;
        case MAIN_MENU_CREATIVE_LEVELS:
            render_main_menu_hand();
            render_menu_button_list(&sButtonsCreativeLevels);

            print_utf8_boxed("Choose a map.",160,180,sMainMenuTransition,TRUE);
            break;
        case MAIN_MENU_CLOSED:
            break;
        default: // Survey
            if (gMainMenuState < MAIN_MENU_SURVEY) {break;}
            gSPDisplayList(gDisplayListHead++, depths_depths_mesh);
            render_survey(sSurveyEntries[gMainMenuState-MAIN_MENU_SURVEY]);
            break;
    }

    s32 alpha = 255.0f-(sMainMenuTransition*255.0f);
    if (alpha > 0 && sMainMenuDoFade) {
        gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
        gDPSetEnvColor(gDisplayListHead++, 0,0,0, alpha);
        render_4slice(-50,241,370,0);
    } else {
        sMainMenuDoFade = FALSE;
    }
}

void main_menu_handle_scroll(u8 max) {
    if (gMainMenuState != gMainMenuTargetState) {return;}

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
    gMariosModulesSave.persistentSeedTimer++;

    if (gMainMenuState != gMainMenuTargetState) {
        sMainMenuTransition -= .1f;
        if (sMainMenuTransition <= 0.0f) {
            sMainMenuIndex = 0;
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

    if (gMainMenuWarpLocation != -1) {return;}

    switch(gMainMenuState) {
        case MAIN_MENU_TITLE_TRANSITION_1:
            if (sMainMenuModuleTimer++ >= 20) {
                gMainMenuTargetState = MAIN_MENU_TITLE;
                gMainMenuState = MAIN_MENU_TITLE;
                sMainMenuTransition = 0.0f;
                sMainMenuModuleTimer = 0;
                sMainMenuShowTitle = TRUE;

                set_background_music(0, SEQ_MM64_TITLE, 0);
                sMainMenuLastSongIndex = SEQ_MM64_TITLE;
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
                load_marios_modules_data_only();
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
                        gMainMenuTargetState = MAIN_MENU_EXTRA;
                        break;
                    case 2:
                        gMainMenuTargetState = MAIN_MENU_CREDITS;
                        break;
                    case 3:
                        gMainMenuTargetState = MAIN_MENU_CHANGELOG;
                        break;
                }
            }
            break;
        case MAIN_MENU_FILE:;
            main_menu_handle_scroll(3);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_MAIN;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                gMariosModulesSaveIndex = sMainMenuIndex;
                if (gMariosModulesSave.file[sMainMenuIndex].flags & SAVE_FLAG_EXIST) {
                    if (gMariosModulesSave.file[sMainMenuIndex].flags & SAVE_FLAG_COMPLETE) {
                        gMainMenuTargetState = MAIN_MENU_FILE_COMPLETE_ACTION;
                    } else {
                        gMainMenuTargetState = MAIN_MENU_FILE_ACTION;
                    }
                } else {
                    gMainMenuTargetState = MAIN_MENU_MODE;
                }
            }
            break;
        case MAIN_MENU_MODE:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_FILE;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {

                tinymt32_init(&gGlobalRandomState,gMariosModulesSave.persistentSeedTimer);
                u32 seed = tinymt32_generate_u32(&gGlobalRandomState);

                switch (sMainMenuIndex) {
                    case 0:
                        save_marios_modules_new_game(seed,-1);
                        gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_NEW;

                        gMainMenuWarpLocation = 2;
                        level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                        break;
                    case 1:
                        gMainMenuWarpLocation = 4;
                        level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                        gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                        gModuleTutorialState = TUTORIAL_DONE;
                        save_marios_modules_new_game(seed,0);
                        break;
                }
            }
            break;
        case MAIN_MENU_FILE_ACTION:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_FILE;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        if (gMariosModulesSave.file[gMariosModulesSaveIndex].level != -1) {
                            // Roguelite
                            gMainMenuWarpLocation = 4;
                            if (gMariosModulesSave.file[gMariosModulesSaveIndex].level == 2) {
                                gMainMenuWarpLocation = 5;
                            }
                        } else {
                            // Campaign
                            gMainMenuWarpLocation = 2;
                        }
                        level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                        gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                        gModuleTutorialState = TUTORIAL_DONE;
                        break;
                    break;
                    case 1:
                        gMainMenuTargetState = MAIN_MENU_FILE_ERASE;
                        break;
                }
            }
            break;
        case MAIN_MENU_FILE_COMPLETE_ACTION:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_FILE;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        gMainMenuTargetState = MAIN_MENU_FILE_VIEW;
                        break;
                    break;
                    case 1:
                        save_delete_file(gMariosModulesSaveIndex);
                        gMainMenuTargetState = MAIN_MENU_FILE;
                        break;
                }
            }
            break;
        case MAIN_MENU_FILE_ERASE:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_FILE;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0: // No
                        gMainMenuTargetState = MAIN_MENU_FILE_ACTION;
                    break;
                    case 1: // Yes
                        save_delete_file(gMariosModulesSaveIndex);
                        gMainMenuTargetState = MAIN_MENU_FILE;
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
        case MAIN_MENU_FILE_VIEW:
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON|B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_FILE_COMPLETE_ACTION;
            }
            break;
        case MAIN_MENU_EXTRA:
            main_menu_handle_scroll(4);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_MAIN;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        gMainMenuTargetState = MAIN_MENU_SOUNDTRACK;
                        break;
                    case 1:
                        gMainMenuTargetState = MAIN_MENU_META_PROGRESSION;
                        break;
                    case 2:
                        gMainMenuTargetState = MAIN_MENU_JOURNAL_ENTRIES;
                        break;
                    case 3:;
                        s32 unlockedCreative = save_get_meta_flag(METAFLAGS_COMPLETION,0) || save_get_meta_flag(METAFLAGS_COMPLETION,1);
                        if (unlockedCreative) {
                            gMainMenuTargetState = MAIN_MENU_MORE_WAYS_TO_PLAY;
                        }
                        break;
                }
            }
            break;
        case MAIN_MENU_MORE_WAYS_TO_PLAY:
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_EXTRA;
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        gMainMenuTargetState = MAIN_MENU_CREATIVE_LEVELS;
                    break;
                    case 1:
                        gMainMenuTargetState = MAIN_MENU_SURVEY;

                        stop_background_music(SEQUENCE_ARGS(4, sMainMenuLastSongIndex ));
                        set_background_music(0, SEQ_TENSE, 0);
                        sMainMenuLastSongIndex = SEQ_TENSE;
                        break;
                }
            }
            break;
        case MAIN_MENU_SOUNDTRACK:;
            main_menu_handle_scroll(2);
            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_EXTRA;
                break;
            }
            if (gPlayer1Controller->buttonPressed & R_CBUTTONS) {
                sMainMenuSongIndex++;
            }
            if (gPlayer1Controller->buttonPressed & L_CBUTTONS) {
                sMainMenuSongIndex--;
            }
            sMainMenuSongIndex = (sMainMenuSongIndex + SONG_COUNT) % SONG_COUNT;

            int song = sSongEntries[sMainMenuSongIndex].seq;
            s8 unlockValue = sSongEntries[sMainMenuSongIndex].unlock;
            if (unlockValue > -1 && !save_get_meta_flag(METAFLAGS_COMPLETION, unlockValue)) {
                break;
            }
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                switch (sMainMenuIndex) {
                    case 0:
                        stop_background_music(SEQUENCE_ARGS(4, sMainMenuLastSongIndex ));
                        set_background_music(0, song, 0);
                        sMainMenuLastSongIndex = song;
                        break;
                    case 1:
                        stop_background_music(SEQUENCE_ARGS(4, sMainMenuLastSongIndex ));
                        break;
                }
            }
            break;
        case MAIN_MENU_META_PROGRESSION:
            animate_wildcolor_module();
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON|B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_EXTRA;
            }
            break;
        case MAIN_MENU_JOURNAL_ENTRIES:
                if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON|B_BUTTON)) {
                    gMainMenuTargetState = MAIN_MENU_EXTRA;
                }

                int entryCount = 20;
                if (gPlayer1Controller->buttonPressed & R_CBUTTONS) {
                    sJournalEntryIndex++;
                }
                if (gPlayer1Controller->buttonPressed & L_CBUTTONS) {
                    sJournalEntryIndex--;
                }
                sJournalEntryIndex = (sJournalEntryIndex + entryCount) % entryCount;
            break;
        case MAIN_MENU_CREATIVE_LEVELS:
            main_menu_handle_scroll(4);

            if (gPlayer1Controller->buttonPressed & (B_BUTTON)) {
                gMainMenuTargetState = MAIN_MENU_MORE_WAYS_TO_PLAY;
                break;
            }

            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                gModuleCreativeEnabled = TRUE; // Disables nuPiSramWrite

                gMariosModulesSaveIndex = 3; // Slot 3 for minigames
                save_marios_modules_new_game(0,-1 + sMainMenuIndex);

                if (gMariosModulesSave.file[0].level != -1) {
                    // Roguelite
                    gMainMenuWarpLocation = 4;
                    if (gMariosModulesSave.file[0].level == 2) {
                        gMainMenuWarpLocation = 5;
                    }
                } else {
                    // Campaign
                    gMainMenuWarpLocation = 2;
                }

                level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                gModuleTutorialState = TUTORIAL_DONE;
            }
            break;
        case MAIN_MENU_CLOSED:
            break;
        default: // Survey Logic
            if (gMainMenuState < MAIN_MENU_SURVEY) {break;}
            main_menu_handle_scroll(menu_button_count(sSurveyEntries[gMainMenuState-MAIN_MENU_SURVEY]));
            if (gPlayer1Controller->buttonPressed & (START_BUTTON|A_BUTTON)) {
                if (gMainMenuState != gMainMenuTargetState) {break;} // Fix mash crash
                gSurveyData[gMainMenuState-MAIN_MENU_SURVEY] = sMainMenuIndex;

                if (sSurveyEntries[gMainMenuState-MAIN_MENU_SURVEY+1] == NULL) {
                    gMariosModulesSaveIndex = 3; // Slot 3 for minigames
                    save_marios_modules_new_game(0,3);
                    gMariosModulesSave.file[3].lives = 0;
                    gMainMenuWarpLocation = 4;
                    level_trigger_warp(gMarioState,WARP_OP_LOOK_UP);
                    gMainMenuTargetState = MAIN_MENU_LEVEL_WARP_CONTINUE;
                    gModuleTutorialState = TUTORIAL_DONE;
                    break;
                }

                // Hardcoded Case that skips size question if you answered ADHD, impossible gen
                if (gSurveyData[SURVEY_NEURO] == 2 && 
                    gMainMenuState-MAIN_MENU_SURVEY+1 == SURVEY_SIZE) {
                    gMainMenuTargetState++;
                }

                gMainMenuTargetState++;
                break;
            }
            break;
    }
}

void render_results_screen(int type) {
    f32 alphaDelta = 1.0f;
    char * primstr;
    switch(type) {
        case 1:
            primstr = "@Y@Crystal Quest COMPLETE!@@";
            break;
        case 2:
            primstr = "@R@GAME OVER@@";
            break;
        case 3:
            primstr = "@Y@Manusanctuary Escape COMPLETE!@@";
            break;
        case 4:
            primstr = "Completed File";
            alphaDelta = sMainMenuTransition;
            break;
        case 5:
            primstr = "@Y@SurvAI.exe Personalized Quest COMPLETE!@@";
            break;
    }

    gSPDisplayList(gDisplayListHead++, mat_micons_fourslice_layer1);
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, 180 * alphaDelta);
    render_4slice(20,220,300,20);

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);

    char resultScreenStr[100];

    utf8_print_reset();
    gDPSetEnvColor(gDisplayListHead++, 0,0,0, 255 * alphaDelta);
    print_utf8(primstr,30,190);
    print_utf8("------------------------------",30,170);

    sprintf(resultScreenStr,"Seed: %d",gMariosModulesSave.file[gMariosModulesSaveIndex].seed);
    print_utf8(resultScreenStr,30,150);

    sprintf(resultScreenStr,"Time Taken: %s",get_game_time_str(gMariosModulesSaveIndex));
    print_utf8(resultScreenStr,30,130);

    print_utf8("Modules Used:",30,110);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    int used = 0;
    for (int i = 0; i < MOD_COUNT; i++) {
        int fflag = i%32;
        int findex = i/32;
        if (gMariosModulesSave.file[gMariosModulesSaveIndex].usedModules[findex] & (1<<fflag)) {
            int x = 100 + (used%12)*16;
            int y = 115 + ((used/12)*16);
            print_module(i,x,y,0);
            used++;
        }
    }
}