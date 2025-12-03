#include <engine/sprite.h>
#include <engine/texturemgr.h>

#include <constants.h>
#include <global.h>
#include <util.h>

#include <algorithm>
#include <math.h>

#include <GLFW/glfw3.h>

Sprite::Sprite() {

}

Sprite::Sprite(float x, float y, float scale, bool solid, bool dud) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->solid = solid;
    if (!dud) {
        this->id = count;
        count++;
        sprites.push_back(this);
    }
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

    x += velocityX*((glfwGetTime()*1000 - Global::last_frame_time)/1000.0f);

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

    std::vector<Rect> hitboxesForFrame = animation.getHitboxes(currentFrame);

    for (Rect hb : hitboxesForFrame) {
        currentHitboxes.push_back(getRealHitbox(hb, x, y, texWidth, texHeight, scale));
    }

    bool collision = false;
    grounded = false;

    int i = 0;

    while (i < spriteCollisions.size()) {
        std::pair<int,int> sc = spriteCollisions[i];
        if (sc.first == id) {
            spriteCollisions.erase(spriteCollisions.begin()+i);
            continue;
        }

        i++;
    }

    if (solid) {
        for (Sprite *s : sprites) {
            int spriteId = s->getID();
            if (s->solid && spriteId != id) {
                std::vector<Rect_F> otherHitboxes = s->getHitboxes();
                for (Rect_F myHb: currentHitboxes) {
                    for (Rect_F otherHb: otherHitboxes) {
                        collision = doHitboxesCollide(myHb, otherHb);

                        if (collision) {
                            spriteCollisions.push_back({id, spriteId});
                            break;
                        }
                    }
                    if (collision) { break; }
                }
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

            int left = std::floor((x - 0.5f*texWidth) / (float)tileWidth);
            int right = std::ceil((x + 0.5f*texWidth) / (float)tileWidth);
            int bottom = std::floor((y - 0.5f*texHeight) / (float)tileHeight);
            int top = std::ceil((y + 0.5f*texHeight) / (float)tileHeight);

            for (int r = bottom; r <= top; r++) {
                if (layer.count(r) == 0) {
                    continue;
                }

                std::map<int,int> row = layer[r];

                for (int c = left; c <= right; c++) {
                    if (row.count(c) == 0) {
                        continue;
                    }

                    float rx = (c * tileWidth);
                    float ry = (r * tileHeight);

                    int t = row[c];
                    Tile tile = Global::level->getTile(t);

                    if (!tile.solid) {
                        continue;
                    }

                    float texWidth = TextureManager::getTexWidth(tile.textureName, tile.c1, tile.c2);
                    float texHeight = TextureManager::getTexHeight(tile.textureName, tile.r1, tile.r2);
    
                    std::vector<Rect> tileHitboxes = Global::level->getHitboxes(t);
                    
                    std::vector<Rect_F> realTileHitboxes;

                    for (Rect hb : tileHitboxes) {
                        Rect_F realHb;
                        realHb.x1 = (rx - 0.5f*(float)tileWidth) + hb.x1;
                        realHb.x2 = (rx - 0.5f*(float)tileWidth) + hb.x2;
                        realHb.y1 = (ry + 0.5f*(float)tileHeight) - hb.y2;
                        realHb.y2 = (ry + 0.5f*(float)tileHeight) - hb.y1;
                    
                        realTileHitboxes.push_back(realHb);
                    }

                    if (doObjectsCollide(currentHitboxes, realTileHitboxes)) {
                        collision = true;
                        break;
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
        }

        y += velocityY*((glfwGetTime()*1000 - Global::last_frame_time)/1000.0f);
        currentHitboxes.clear();

        for (Rect hb : hitboxesForFrame) {
            currentHitboxes.push_back(getRealHitbox(hb, x, y, texWidth, texHeight, scale));
        }

        collision = false;

        for (Sprite *s : sprites) {
            int spriteId = s->getID();
            if (s->solid && spriteId != id) {
                std::vector<Rect_F> otherHitboxes = s->getHitboxes();
                for (Rect_F myHb: currentHitboxes) {
                    for (Rect_F otherHb: otherHitboxes) {
                        collision = doHitboxesCollide(myHb, otherHb);

                        if (collision) {
                            spriteCollisions.push_back({id, spriteId});
                            break;
                        }
                    }
                    if (collision) { break; }
                }
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

            int left = std::floor((x - 0.5f*texWidth) / (float)tileWidth);
            int right = std::ceil((x + 0.5f*texWidth) / (float)tileWidth);
            int bottom = std::floor((y - 0.5f*texHeight) / (float)tileHeight);
            int top = std::ceil((y + 0.5f*texHeight) / (float)tileHeight);

            for (int r = bottom; r <= top; r++) {
                if (layer.count(r) == 0) {
                    continue;
                }

                std::map<int,int> row = layer[r];

                for (int c = left; c <= right; c++) {
                    if (row.count(c) == 0) {
                        continue;
                    }

                    float rx = (c * tileWidth);
                    float ry = (r * tileHeight);

                    int t = row[c];
                    Tile tile = Global::level->getTile(t);

                    if (!tile.solid) {
                        continue;
                    }

                    float texWidth = TextureManager::getTexWidth(tile.textureName, tile.c1, tile.c2);
                    float texHeight = TextureManager::getTexHeight(tile.textureName, tile.r1, tile.r2);
    
                    std::vector<Rect> tileHitboxes = Global::level->getHitboxes(t);
                    
                    std::vector<Rect_F> realTileHitboxes;

                    for (Rect hb : tileHitboxes) {
                        Rect_F realHb;
                        realHb.x1 = (rx - 0.5f*(float)tileWidth) + hb.x1;
                        realHb.x2 = (rx - 0.5f*(float)tileWidth) + hb.x2;
                        realHb.y1 = (ry + 0.5f*(float)tileHeight) - hb.y2;
                        realHb.y2 = (ry + 0.5f*(float)tileHeight) - hb.y1;
                    
                        realTileHitboxes.push_back(realHb);
                    }

                    if (doObjectsCollide(currentHitboxes, realTileHitboxes)) {
                        collision = true;
                        break;
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
            y = lastY;
            velocityY = 0;
            grounded = true;
        }
    }

    velocityX = 0.0f;
    velocityY -= 5.0f;
}

void Sprite::draw(Camera *camera) {
    Animation animation = animations[currentAnimation];
    Frame currentFrameObj = animation.getFrame(currentFrame);
    float rx = std::floor(x);
    float ry = std::floor(y);

    TextureManager::setTex(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.c1, currentFrameObj.r2, currentFrameObj.c2);
    TextureManager::drawTex(rx, ry, scale, angle, camera);
}

void Sprite::move(float dx, float dy) {
    velocityX += dx;
    velocityY += dy;
}

float Sprite::getX() { return x; }

float Sprite::getY() { return y; }

float Sprite::getVelocityX() { return velocityX; }
float Sprite::getVelocityY() { return velocityY; }

void Sprite::addHitbox(std::string animationName, int frame, int x1, int y1, int x2, int y2) {
    std::map<int, std::vector<Rect>> framesForAnimation;
    std::vector<Rect> hitboxesForFrame;

    if (animations.count(animationName) == 0) {
        return;
    }

    Animation animation = animations[animationName];

    animation.addHitbox(frame, x1, y1, x2, y2);

    animations[animationName] = animation;
}

bool Sprite::isCollidingWith(Sprite sprite) {
    int spriteId = sprite.getID();
    for (std::pair<int, int> sc : spriteCollisions) {
        if ((sc.first == id && sc.second == spriteId) || (sc.first == spriteId && sc.second == id)) {
            return true;
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

void Sprite::setPos(float x, float y) {
    this->x = x;
    this->y = y;
}

void Sprite::setSolid(bool solid) {
    this->solid = solid;
}

void Sprite::jump(float jumpSpeed) {
    velocityY = jumpSpeed;
}

bool Sprite::isGrounded() {
    return grounded;
}

void Sprite::rotate(float diff) {
    setAngle(angle + diff);
}

void Sprite::setAngle(float angle) {
    this->angle = angle;
    while (angle < 0) {
        angle += 2*M_PI;
    }
    while (angle > 2*M_PI) {
        angle -= 2*M_PI;
    }
}

float Sprite::getAngle() { return angle; }
