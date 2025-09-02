#ifndef ANIMATION_H
#define ANIMATION_H

#include <engine/types.h>

#include <string>
#include <vector>
#include <map>

class Animation {
    public:
        Animation(std::string name);
        Animation();

        void addFrame(std::string textureName, int r1, int c1, int r2, int c2, float duration);
        void addHitbox(std::string hbName, int frame, int x1, int y1, int x2, int y2);
        std::vector<Rect> getHitboxes(std::string hbName, int frame);
        std::map<std::string, std::vector<Rect>> getAllHitboxes(int frame);
        
        int getNumberOfFrames();
        Frame getFrame(int index);

    private:
        std::string name;
        std::vector<Frame> frames;

        std::map<int, std::map<std::string, std::vector<Rect>>> hitboxes;
};

#endif
