#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <engine/sprite.h>
#include <engine/types.h>

#include <stack>
#include <queue>

class Zombie : public Sprite {
    public:
        using Sprite::Sprite;

        void update();
        void setTarget(int id);
        void getPathToTarget();

    private:
        int targetId;

        std::stack<Pos_F> path;
};

#endif
