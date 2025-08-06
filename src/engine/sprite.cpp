#include <engine/sprite.h>
#include <engine/texturemgr.h>

#include <constants.h>

#include <algorithm>

#include <GLFW/glfw3.h>

Sprite::Sprite() {

}

Sprite::Sprite(float x, float y, float scale, bool solid) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->solid = solid;
    this->id = count;
    count++;
    sprites.push_back(this);
}

void Sprite::addAnimation(std::string name, Animation animation) {
    animations[name] = animation;
}

void Sprite::setAnimation(std::string name) {
    currentAnimation = name;
    currentFrame = 0;
}

void Sprite::update() {
    timer += 1000*glfwGetTime() - lastUpdate;
    lastUpdate = 1000*glfwGetTime();

    lastX = x;
    lastY = y;

    x += velocityX/FPS;
    y += velocityY/FPS;

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

    std::map<int, std::vector<Rect>> framesOfAnimation; 
    float texWidth = TextureManager::getTexWidth(currentFrameObj.textureName, currentFrameObj.c1, currentFrameObj.c2);
    float texHeight = TextureManager::getTexHeight(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.r2);
    
    currentHitboxes.clear();
    
    if (hitboxes.count(currentAnimation) != 0) {
        framesOfAnimation = hitboxes.at(currentAnimation);
        if (framesOfAnimation.count(currentFrame) != 0) {
            std::vector<Rect> frameHitboxes = framesOfAnimation.at(currentFrame);
            // printf("-----------------\n");
            for (Rect hb : frameHitboxes) {
                float realX1 = (x - 0.5f*scale*texWidth) + hb.x1*scale;
                float realX2 = (x - 0.5f*scale*texWidth) + hb.x2*scale;
                float realY1 = (y + 0.5f*scale*texHeight) - hb.y1*scale;
                float realY2 = (y + 0.5f*scale*texHeight) - hb.y2*scale;
                currentHitboxes.push_back((Rect_F) { realX1, realY1, realX2, realY2 });
                // debugging
                // printf("X: %.2f, Y: %.2f\n", x, y);
                // printf("X1: %.2f, Y1: %.2f, X2: %.2f, Y2: %.2f\n", realX1, realY1, realX2, realY2);
            }
        }
    }

    bool collision = false;

    if (solid) {
        for (Sprite *s : sprites) {
            if (s->solid && s->getID() != id) {
                std::vector<Rect_F> otherHitboxes = s->getHitboxes();
                for (Rect_F hb1 : currentHitboxes) {
                    for (Rect_F hb2 : otherHitboxes) {
                        if (std::min(hb1.x2, hb2.x2) > std::max(hb1.x1, hb2.x1) && std::min(hb1.y1, hb2.y1) > std::max(hb1.y2, hb2.y2)) {
                            collision = true;
                            break;
                        }
                    }
                    if (collision) { break; }
                }
            }
        }

        if (collision) {
            x = lastX;
            y = lastY;
        }
    }

    velocityX = 0.0f;
    velocityY = 0.0f;
}

void Sprite::draw(Camera camera) {
    Animation animation = animations[currentAnimation];
    Frame currentFrameObj = animation.getFrame(currentFrame);

    TextureManager::setTex(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.c1, currentFrameObj.r2, currentFrameObj.c2);
    TextureManager::drawTex(x, y, scale, camera);

   
}

void Sprite::move(float dx, float dy) {
    velocityX += dx;
    velocityY += dy;
}

float Sprite::getX() { return x; }

float Sprite::getY() { return y; }

void Sprite::addHitbox(std::string animationName, int frame, int x1, int y1, int x2, int y2) {
    std::map<int, std::vector<Rect>> framesForAnimation;
    std::vector<Rect> hitboxesForFrame;

    if (hitboxes.count(animationName) != 0) {
        framesForAnimation = hitboxes.at(animationName);
    }

    if (framesForAnimation.count(frame) != 0) {
        hitboxesForFrame = framesForAnimation.at(frame);
    }

    hitboxesForFrame.push_back({ x1, y1, x2, y2 });
    framesForAnimation[frame] = hitboxesForFrame;
    hitboxes[animationName] = framesForAnimation;
}

bool Sprite::isCollidingWith(Sprite sprite) {
    std::vector<Rect_F> otherHitboxes = sprite.getHitboxes();
    for (Rect_F hb1 : currentHitboxes) {
        for (Rect_F hb2 : otherHitboxes) {
            if (std::min(hb1.x2, hb2.x2) > std::max(hb1.x1, hb2.x1) && std::min(hb1.y1, hb2.y1) > std::max(hb1.y2, hb2.y2)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<Rect_F> Sprite::getHitboxes() {
    return currentHitboxes;
}

int Sprite::getID() {
    return id;
}
