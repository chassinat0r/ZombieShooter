#include <engine/sprite.h>
#include <engine/texturemgr.h>

#include <GLFW/glfw3.h>

Sprite::Sprite() {

}

Sprite::Sprite(int x, int y) {
    this->x = x;
    this->y = y;
}

void Sprite::addAnimation(std::string name, std::vector<Frame> animation) {
    animations[name] = animation;
}

void Sprite::setAnimation(std::string name) {
    currentAnimation = name;
}

void Sprite::draw(Camera camera, float scale) {
    timer += 1000*glfwGetTime() - lastUpdate;
    lastUpdate = 1000*glfwGetTime();
    Frame currentFrameObj = animations[currentAnimation][currentFrame];
    if (timer >= currentFrameObj.duration) {
        currentFrame++;
        timer = 0;
    }
    if (currentFrame == animations[currentAnimation].size()) {
        currentFrame = 0;
    }
    currentFrameObj = animations[currentAnimation][currentFrame];

    TextureManager::setTex(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.c1, currentFrameObj.r2, currentFrameObj.c2);
    TextureManager::drawTex(x, y, scale, camera);
}
