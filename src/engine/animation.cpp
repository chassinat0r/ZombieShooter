#include <engine/animation.h>

Animation::Animation(std::string name) {
    this->name = name;
}

Animation::Animation() {

}

void Animation::addFrame(std::string textureName, int r1, int c1, int r2, int c2, float duration) {
    Frame frame;
    frame.textureName = textureName;
    frame.r1 = r1;
    frame.c1 = c1;
    frame.r2 = r2;
    frame.c2 = c2;
    frame.duration = duration;

    frames.push_back(frame);
}

int Animation::getNumberOfFrames() {
    return frames.size();
}

Frame Animation::getFrame(int index) {
    if (index < frames.size()) {
        return frames[index];
    }
}
