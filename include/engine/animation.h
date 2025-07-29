#ifndef ANIMATION_H
#define ANIMATION_H

#include <engine/types.h>

#include <string>
#include <vector>

class Animation {
    public:
        Animation(std::string name);
        Animation();

        void addFrame(std::string textureName, int r1, int c1, int r2, int c2, float duration);

        int getNumberOfFrames();
        Frame getFrame(int index);

    private:
        std::string name;
        std::vector<Frame> frames;
};

#endif
