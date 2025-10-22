#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <engine/sprite.h>
#include <engine/types.h>

#include <stack>
#include <queue>

class Zombie : public Sprite {
    public:
        Zombie(float x, float y, int healthMax, int healthCurr, float scale = 1.0f, bool solid = true);
        Zombie();

        void registerAttack();

        bool isInCooldown();

        void update();
        
        void setTarget(int targetId);
        // void addHealth(int change = 1);
        // void removeHealth(int change = 1);
    
    private:
        int health;
        int healthMax;

        bool timeOut = false;
        float cooldown = 500;
        float cdProgress = 0;

        Sprite *target = nullptr;

        int direction = 1; // 1 for left, 2 for right
        float speed = 15.0f;
};

#endif
