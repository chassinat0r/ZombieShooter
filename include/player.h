#ifndef PLAYER_H
#define PLAYER_H

#include <engine/sprite.h>
#include <character.h>

#include <events/ZombieHitEvent.h>

class Player : public Character, public ZombieHitListener {
    public:
        Player(float x, float y, float scale = 1.0f, bool solid = false);
        Player();

        void update();
        void drawHealthBar();

        void startMoving();
        void stopMoving();

        int getDirection();

        void jump();

        void setDirection(int dir);

        void die();

        void onZombieHit(Zombie *zombie);
        
    private:
        bool moving = false;
        bool running = false;
        bool falling = false;
        bool jumping = false;

        int direction = 2; // 1 left, 2 right
        float jumpSpeed = 200.0f;
        float speed = 30.0f;
};

#endif
