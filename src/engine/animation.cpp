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

    return Frame();
}

void Animation::addHitbox(std::string hbName, int frame, int x1, int y1, int x2, int y2) {
    std::map<std::string, std::vector<Rect>> frameHitboxes;
    if (hitboxes.count(frame) > 0) {
        frameHitboxes = hitboxes[frame];
    }

    std::vector<Rect> nameHitboxes;

    if (frameHitboxes.count(hbName) > 0) {
        nameHitboxes = frameHitboxes.at(hbName);
    }

    nameHitboxes.push_back({x1, y1, x2, y2});

    frameHitboxes[hbName] = nameHitboxes;

    hitboxes[frame] = frameHitboxes;
}

std::vector<Rect> Animation::getHitboxes(std::string hbName, int frame) {
    if (hitboxes.count(frame) > 0) {
        std::map<std::string, std::vector<Rect>> frameHitboxes = hitboxes.at(frame);

        if (frameHitboxes.count(hbName) > 0) {
            return frameHitboxes.at(hbName);
        }
    }

    return std::vector<Rect>();
}

 std::map<std::string, std::vector<Rect>> Animation::getAllHitboxes(int frame) {
    if (hitboxes.count(frame) > 0) {
        std::map<std::string, std::vector<Rect>> frameHitboxes = hitboxes.at(frame);
        return frameHitboxes;
    }

    return std::map<std::string, std::vector<Rect>>();
}
