#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#include "wiggler_body/model.inc.c"
#include "wiggler_body/anims/data.inc.c"
#include "wiggler_body/anims/table.inc.c"

#include "wiggler_head/model.inc.c"
#include "wiggler_head/anims/data.inc.c"
#include "wiggler_head/anims/table.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

#include "king_bobomb/model.inc.c"
#include "king_bobomb/anims/data.inc.c"
#include "king_bobomb/anims/table.inc.c"

#include "micons/model.inc.c"
#include "chest/model.inc.c"
#include "chest/anims/data.inc.c"
#include "chest/anims/table.inc.c"
#include "hover/model.inc.c"
#include "cchest/model.inc.c"
#include "woman/model.inc.c"
#include "ui/model.inc.c"
#include "save/anims/data.inc.c"
#include "save/anims/table.inc.c"
#include "save/model.inc.c"
#include "savec/collision.inc.c"
#include "saveanim/anims/data.inc.c"
#include "saveanim/anims/table.inc.c"
#include "module3d/model.inc.c"
#include "ice/model.inc.c"
#include "ice/collision.inc.c"
#include "mchest/collision.inc.c"
#include "mchest/model.inc.c"
#include "key/collision.inc.c"
#include "key/model.inc.c"
#include "hover/collision.inc.c"
#include "silver_star/model.inc.c"
#include "utilityMace/model.inc.c"
#include "utilityFlame/model.inc.c"
#include "car/collision.inc.c"
#include "car/model.inc.c"
#include "live/model.inc.c"
