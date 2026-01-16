#ifndef BULLET_H
#define BULLET_H

#include <engine/sprite.h>

#include <vector>

class Bullet : public Sprite {
    public:
        Bullet(float x, float y, float angle, float scale);
        Bullet();

        inline static std::vector<Bullet*> bullets;

        void update();

    private:
        float speed = 60.0f;
        float moveAngle;
};

#endif
