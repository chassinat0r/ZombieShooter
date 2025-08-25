#include <engine/sprite.h>
#include <engine/texturemgr.h>

#include <constants.h>
#include <global.h>
#include <util.h>

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
            currentHitboxes = getRealHitboxes(frameHitboxes, x, y, texWidth, texHeight, scale);
        }
    }

    bool collision = false;

    if (solid) {
        for (Sprite *s : sprites) {
            if (s->solid && s->getID() != id) {
                std::vector<Rect_F> otherHitboxes = s->getHitboxes();
                collision = doObjectsCollide(currentHitboxes, otherHitboxes);
                if (collision) { break; }
            }
        }

        for (int l = 0; l < Global::level->getLayerCount(); l++) {
            if (std::find(collisionLayers.begin(), collisionLayers.end(), l) == collisionLayers.end()) {
                continue;
            }

            std::map<int, std::map<int,int>> layer = Global::level->getLayer(l);
            
            std::pair<int,int> tileDimensions = Global::level->getTileSize(l);
            int tileWidth = tileDimensions.first;
            int tileHeight = tileDimensions.second;

            int left = NULL;
            int right = NULL;
            int bottom = NULL;
            int top = NULL;

            for (Rect_F hb : currentHitboxes) {
                int tempLeft = std::round(hb.x1 / tileWidth);
                int tempRight = std::round(hb.x2 / tileWidth);
                int tempBottom = std::round(hb.y1 / tileWidth);
                int tempTop = std::round(hb.y2 / tileWidth);

                if (tempLeft < left || left == NULL) {
                    left = tempLeft;
                }
                if (tempRight > right || right == NULL) {
                    right = tempRight;
                }
                if (tempBottom < bottom || bottom == NULL) {
                    bottom = tempBottom;
                }
                if (tempTop > top || top == NULL) {
                    top = tempTop;
                }
            }

            left--;
            right++;
            top++;
            bottom--;

            for (int r = bottom; r <= top; r++) {
                if (layer.count(r) == 0) {
                    continue;
                }

                std::map<int,int> row = layer[r];

                for (int c = left; c <= right; c++) {
                    if (row.count(c) == 0) {
                        continue;
                    }
                    int t = row[c];
                    Tile tile = Global::level->getTile(t);

                    if (!tile.solid) {
                        continue;
                    }

                    std::vector<Rect> tileHitboxes = Global::level->getHitboxes(t);
                    std::vector<Rect_F> realTileHitboxes = getRealHitboxes(tileHitboxes, c*tileWidth, r*tileHeight, tileWidth, tileHeight, 1.0f);
                    if (doObjectsCollide(currentHitboxes, realTileHitboxes)) {
                        collision = true;
                    }
                }

                if (collision) {
                    break;
                }
            }
            if (collision) {
                break;
            }
        }

        if (collision) {
            x = lastX;
            y = lastY;
            // printf("Collision\n");
        } else {
            // printf("No collision\n");
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

void Sprite::addCollisionLayer(int layer) {
    if (std::find(collisionLayers.begin(), collisionLayers.end(), layer) == collisionLayers.end()) {
        collisionLayers.push_back(layer);
    }
}

bool Sprite::doesCollideOnLayer(int layer) {
    return std::find(collisionLayers.begin(), collisionLayers.end(), layer) != collisionLayers.end();
}

bool Sprite::hasMoved() {
    return (lastX != x || lastY != y);
}

