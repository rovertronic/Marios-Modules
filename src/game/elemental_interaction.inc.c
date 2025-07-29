struct ElementHitbox sElementHitboxArray[100];

// Elemental Interaction System

void bhv_init_element_flame(void) {
    obj_element_init(o,ELEMENT_FLAME,100.0f);
}

void obj_element_init(struct Object * obj, u8 element, f32 radius) {
    for (int i = 0; i < 100; i++) {
        if (!sElementHitboxArray[i].initialized) {
            sElementHitboxArray[i].initialized = TRUE;
            obj->element = &sElementHitboxArray[i];

            sElementHitboxArray[i].object = obj;
            sElementHitboxArray[i].element = element;
            sElementHitboxArray[i].afflict = 0;
            sElementHitboxArray[i].radius = radius;
            return;
        }
    }
}

struct ElementHitbox * obj_check_element_collision(u8 element, u8 lateral) {
    if (o->element == NULL) {return NULL;} // Object not part of element system
    if (o->element->element == element) {return NULL;} // Cannot collide with own members
    if (o->element->afflict & element) {return NULL;} // Cannot collide if afflicted with this element

    for (int i = 0; i < 100; i++) {
        if (sElementHitboxArray[i].initialized == FALSE) {continue;} // Don't collide with uninitialized
        if (sElementHitboxArray[i].element != element) {continue;} // Don't collide if element doesn't match
        if ((o->element->element & element)) {continue;} // Don't collide with own members
        //if (!((sElementHitboxArray[i].afflict & element))) {continue;} // Don't collide with afflicted with own element
        if (o->element == &sElementHitboxArray[i]) {continue;} // Don't collide with self


        Vec3f pos1; vec3f_copy(pos1, &o->oPosVec);
        Vec3f pos2; vec3f_copy(pos2, &sElementHitboxArray[i].object->oPosVec);

        f32 distSqr = (sqr(pos1[0] - pos2[0])) + (sqr(pos1[2] - pos2[2]));
        if (!lateral) {
            distSqr += (sqr(pos1[1] - pos2[1]));
        }
        f32 touchThresh = sqr(o->element->radius + sElementHitboxArray[i].object->element->radius);

        if (distSqr < touchThresh) {
            return &sElementHitboxArray[i];
        }
    }
    return NULL;
}

void obj_element_enemy_loop(void) {
    // Check collisions
    if (obj_check_element_collision(ELEMENT_FLAME,0)) {
        o->element->afflict |= ELEMENT_FLAME;
    }

    struct ElementHitbox * iceTouch = obj_check_element_collision(ELEMENT_ICE,0);
    if (iceTouch && iceTouch->object->element->objectStackAbove == NULL) {
        if ((o->element->objectStackBelow)&&(!obj_has_behavior(o->element->objectStackBelow,bhvIceProjectile))) {

        } else {
            o->element->afflict |= ELEMENT_ICE;
            iceTouch->radius = o->element->radius;

            Mat4 neutral;
            struct Object * iceCube = summon_element_projectile(ELEMENT_ICE,&o->oPosVec,neutral);
            enemy_module_use(iceCube,o);
        }
    }

    // Inflictions
    if (o->element->afflict & ELEMENT_FLAME) {
        struct Object * particle = spawn_object(o, MODEL_RED_FLAME, bhvKoopaShellFlame);
        //particle = 

        if (o->oTimer % 10 == 0) {
            spawn_object(o, MODEL_BURN_SMOKE, bhvBlackSmokeMario);
        } 
        o->element->element = ELEMENT_FLAME;
        o->oInteractType = INTERACT_FLAME;
        //o->oAction = OBJ_ACT_SQUISHED;
    }

    if (o->element->afflict & ELEMENT_ICE) {
        //if (!obj_has_behavior(o->element->objectStackBelow,bhvIceProjectile)) {
            //o->element->afflict &= ~ELEMENT_ICE;
        //}
    }

    enemy_module_use_position();
}

struct WallCollisionData sParticleHitbox;

s16 element_projectile_step(void) {
    o->header.gfx.scale[1] = approach_f32_asymptotic(o->header.gfx.scale[1],o->element->radius/50.0f,.1f);
    cur_obj_scale(o->header.gfx.scale[1]);

    o->oGravity = 2.5f;
    o->oFriction = 0;//0.99f;
    o->oBuoyancy = 1.4f;
    o->oBounciness = 0;
    f32 speed = ABS(o->oVelX-o->oVelZ);

    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;

    o->oVelY -= 2.5f;

    // WALL COL
    sParticleHitbox.x = o->oPosX;
    sParticleHitbox.y = o->oPosY;
    sParticleHitbox.z = o->oPosZ;
    sParticleHitbox.offsetY = 25.0f;
    sParticleHitbox.radius = o->element->radius;

    find_wall_collisions(&sParticleHitbox);

    if (sParticleHitbox.walls[0]) {
        cur_obj_play_sound_2(SOUND_ACTION_BONK);
        o->oVelX += (sParticleHitbox.x - o->oPosX);
        o->oVelZ += (sParticleHitbox.z - o->oPosZ);

        o->oPosX += o->oVelX;
        o->oPosZ += o->oVelZ;
    }

    sParticleHitbox.walls[0] = NULL;

    // FLOOR COL
    struct Surface * floor;
    find_floor(o->oPosX,o->oPosY+25.0f,o->oPosZ,&floor);
    if (floor) {
        f32 floorHeight = find_floor_height(o->oPosX,o->oPosY+100.0f,o->oPosZ);
        if (floorHeight + 5.0f + speed >= o->oPosY && o->oVelY <= 0.0f) {
            o->oVelX += floor->normal.x*2.0f;
            //o->oVelY += (floorHeight-o->oPosY)*.2f;
            o->oVelZ += floor->normal.z*2.0f;

            //o->oVelY = -o->oVelY*.98f;
            o->oVelY = 0.0f;
            o->oPosY = floorHeight;

            if (speed > 10.0f) {
                cur_obj_play_sound_1(SOUND_ENV_MOVING_SAND_SNOW);
                small_breakable_box_spawn_dust();
            }
        }
    }

    enemy_module_use_position();
}

void bhv_flame_projectile(void) {
    element_projectile_step();
}

void bhv_ice_projectile(void) {
    if (o->oDistanceToMario < 200.0f && (gMarioState->flags & MARIO_PUSHING)) {
        o->oVelX += sins(gMarioState->faceAngle[1]);
        o->oVelZ += coss(gMarioState->faceAngle[1]);
        gMarioState->pos[0] += o->oVelX;
        gMarioState->pos[2] += o->oVelZ;
    }

    element_projectile_step();
    enemy_module_set_position(&o->oPosVec);
}

struct Object * summon_element_projectile(s8 element, Vec3f origin, Mat4 projectileTransform) {
    if (element != ELEMENT_NORMAL) {
        u16 model;
        BehaviorScript beh;
        switch(element) {
            case ELEMENT_FLAME:
                beh = bhvFlameProjectile;
                model = MODEL_RED_FLAME;
                break;
            case ELEMENT_ICE:
                beh = bhvIceProjectile;
                model = MODEL_ICE;
                break;
        }

        struct Object * projectile = spawn_object(o,model,beh);
        obj_element_init(projectile,element,50.0f);
        obj_scale(projectile,.0f);
        //projectile->oVelX = sins(o->oFaceAngleYaw) * 15.0f;
        projectile->oVelY = gMarioState->vel[1];
        //projectile->oVelZ = coss(o->oFaceAngleYaw) * 15.0f;

        vec3f_copy(&projectile->oPosVec,origin);

        return projectile;
    }
    return NULL;
}

// ENEMY STACKING SYSTEM

void enemy_module_use(struct Object * user, struct Object * used) {
    if (!(user->element && used->element)) {return;}

    //if (used->element->objectStackBelow) {
    //    used->element->objectStackBelow->element->objectStackAbove = NULL;
    //}
    //if (used->element->objectStackAbove) {
    //    used->element->objectStackAbove->element->objectStackBelow = user;
    //}

    user->element->objectStackAbove = used;
    used->element->objectStackBelow = user;
}

void enemy_module_set_position(Vec3f pos) {
    if (!o->element) {return;}
    vec3f_copy(o->element->enemyModulePosition,pos);
}

void enemy_module_use_position(void) {
    if (!(o->element && o->element->objectStackBelow)) {return;}
    vec3f_copy(&o->oPosVec,o->element->objectStackBelow->element->enemyModulePosition);
    vec3f_copy(&o->oHomeVec,o->element->objectStackBelow->element->enemyModulePosition);
    o->oVelY = 0.0f;
}

// Shared Functionality

void object_clear_element_and_stacking(struct Object * obj) {
    if (!obj->element) {return;}

    obj->element->initialized = FALSE;

    if (obj->element->objectStackBelow) {
        obj->element->objectStackBelow->element->objectStackAbove = NULL;
    }
    if (obj->element->objectStackAbove) {
        obj->element->objectStackAbove->element->objectStackBelow = NULL;
    }
    obj->element->objectStackBelow = NULL;
    obj->element->objectStackAbove = NULL;
}