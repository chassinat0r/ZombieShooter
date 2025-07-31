#include <engine/sprite.h>
#include <engine/texturemgr.h>

#include <constants.h>

#include <GLFW/glfw3.h>

Sprite::Sprite() {

}

Sprite::Sprite(float x, float y) {
    this->x = x;
    this->y = y;
}

void Sprite::addAnimation(std::string name, Animation animation) {
    animations[name] = animation;
}

void Sprite::setAnimation(std::string name) {
    currentAnimation = name;
    currentFrame = 0;
}

void Sprite::draw(Camera camera, float scale) {
    timer += 1000*glfwGetTime() - lastUpdate;
    lastUpdate = 1000*glfwGetTime();
    Animation animation = animations[currentAnimation];
    Frame currentFrameObj = animation.getFrame(currentFrame);
    if (timer >= currentFrameObj.duration) {
        currentFrame++;
        timer = 0;
    }
    if (currentFrame == animation.getNumberOfFrames()) {
        currentFrame = 0;
    }
    currentFrameObj = animation.getFrame(currentFrame);

    TextureManager::setTex(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.c1, currentFrameObj.r2, currentFrameObj.c2);
    TextureManager::drawTex(x, y, scale, camera);
}

void Sprite::move(float dx, float dy) {
    x += dx/FPS;
    y += dy/FPS;
}
