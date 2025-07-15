
#define ELEMENT_NORMAL (1<<0)
#define ELEMENT_ICE (1<<1)
#define ELEMENT_FLAME (1<<2)

struct ElementHitbox {
    u8 initialized;
    u8 element; // Element Flag
    u8 afflict; // Element Flag
    f32 radius;
    struct Object * object;
};

void obj_element_init(struct Object * obj, u8 element, f32 radius);
struct ElementHitbox * obj_check_lateral_element_collision(u8 element);
struct ElementHitbox * obj_check_element_collision(u8 element);
void summon_element_projectile(s8 element, Vec3f origin, Mat4 projectileTransform);