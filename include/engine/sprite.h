#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>
#include <map>

#include <engine/types.h>
#include <engine/animation.h>

class Sprite {
    public:
        Sprite();
        Sprite(int x, int y);

        void addAnimation(std::string name, Animation animation);
        void setAnimation(std::string name);
        void draw(Camera camera, float scale = 1.0f);

    private:
        int x;
        int y;

        std::map<std::string,Animation> animations;
        std::string currentAnimation;
        int currentFrame = 0;
        double timer = 0;
        double lastUpdate = 0;
};

#endif
