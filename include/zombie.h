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

        bool targetMovedFromDestination();

    private:
        int targetId;

        std::stack<Pos_F> path;

        Sprite* getTarget();

        Pos_F end;
        bool endDefined = false;
};

#endif
