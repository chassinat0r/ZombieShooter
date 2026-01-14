#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <engine/sprite.h>
#include <engine/types.h>

#include <events/BulletHitEvent.h>

#include <bullet.h>
#include <character.h>

#include <stack>
#include <queue>

class Zombie : public Character, public BulletHitListener {
    public:
        Zombie(float x, float y, int healthMax, int healthCurr, float scale = 1.0f, bool solid = true);
        Zombie();

        void registerAttack();

        bool isInCooldown();

        void update();
        
        void setTarget(int targetId);
    
        void onBulletHit();

    private:
        bool timeOut = false;
        float cooldown = 500;
        float cdProgress = 0;

        Sprite *target = nullptr;

        int direction = 1; // 1 for left, 2 for right
        float speed = 15.0f;
};

#endif
