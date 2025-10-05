#ifndef PLAYER_H
#define PLAYER_H

#include <engine/sprite.h>

class Player : public Sprite {
    public:
        Player(float x, float y, int healthMax, int healthCurr, float scale = 1.0f, bool solid = false);
        Player();

        void update();

        void startMoving();
        void stopMoving();

        int getDirection();

        void jump();

        void setDirection(int dir);

    private:
        bool moving = false;
        bool running = false;
        bool falling = false;
        bool jumping = false;

        int direction = 1; // 1 left, 2 right
        float jumpSpeed = 200.0f;
        float speed = 30.0f;
};

#endif
