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

        // void update();
        
        // void addHealth(int change = 1);
        // void removeHealth(int change = 1);
    
    private:
        int health;
        int healthMax;
};

#endif
