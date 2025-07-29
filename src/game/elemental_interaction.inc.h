
#define ELEMENT_NORMAL (1<<0)
#define ELEMENT_ICE (1<<1)
#define ELEMENT_FLAME (1<<2)

struct ElementHitbox {
    u8 initialized;
    u8 element; // Element Flag
    u8 afflict; // Element Flag
    f32 radius;
    struct Object * object;

    // Slightly separate system - Parenting and childing with other objects
    struct Object * objectStackAbove;
    struct Object * objectStackBelow;
    Vec3f enemyModulePosition;
};

// Element Interaction System

void obj_element_init(struct Object * obj, u8 element, f32 radius);
struct ElementHitbox * obj_check_element_collision(u8 element, u8 lateral);
struct Object * summon_element_projectile(s8 element, Vec3f origin, Mat4 projectileTransform);

// Object Stacking System

void enemy_module_use(struct Object * user, struct Object * used);
void enemy_module_set_position(Vec3f pos);
void enemy_module_use_position(void);

// Shared Functionality

void object_clear_element_and_stacking(struct Object * obj);