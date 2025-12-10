#ifndef CHARACTER_H
#define CHARACTER_H

#include <engine/sprite.h>

class Character : public Sprite {
    using Sprite::Sprite;

    public:
        void setHealth(int health);
        void setMaxHealth(int healthMax);
        void addHealth(int change = 1);
        void removeHealth(int change = 1);

    protected:
        int health = 1;
        int healthMax = 1;
};

#endif
