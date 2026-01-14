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

Sprite::Sprite(float x, float y, std::string tag, float scale, bool solid, bool gravity, bool dud) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->solid = solid;
    this->tag = tag;
    this->gravity = gravity;
    if (!dud) {
        this->id = count;
        count++;
        sprites.push_back(this);
    }
    angle = 0;
}

void Sprite::addAnimation(std::string name, Animation animation) {
    animations[name] = animation;
}

void Sprite::setAnimation(std::string name) {
    currentAnimation = name;
    currentFrame = 0;
    resetHitboxes();
}

void Sprite::resetHitboxes() {
    hitboxes.clear();
    Animation currAnimObject = animations.at(currentAnimation);
    hitboxes = currAnimObject.getHitboxes(currentFrame);
}

void Sprite::update() {
    timer += 1000*glfwGetTime() - lastUpdate;
    lastUpdate = 1000*glfwGetTime();

    lastX = x;
    lastY = y;
    lastPosSet = true;

    Animation animation = animations[currentAnimation];
    Frame currentFrameObj = animation.getFrame(currentFrame);
    if (timer >= currentFrameObj.duration) {
        currentFrame++;
        timer = 0;
        if (currentFrame == animation.getNumberOfFrames()) {
            currentFrame = 0;
        }
        resetHitboxes();

    }

    currentFrameObj = animation.getFrame(currentFrame);

    std::map<int, std::vector<Rect>> framesOfAnimation; 
    width = TextureManager::getTexWidth(currentFrameObj.textureName, currentFrameObj.c1, currentFrameObj.c2);
    height = TextureManager::getTexHeight(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.r2);

    bool collision = false;
    grounded = false;


    while (spriteCollisions.size() > 0) {
        spriteCollisions.erase(spriteCollisions.begin());
    }

    while (tagCollisions.size() > 0) {
        tagCollisions.erase(tagCollisions.begin());
    }

    x += velocityX*((glfwGetTime()*1000.0f - Global::last_frame_time)/1000.0f);

    if (solid) {
        for (Sprite *s : sprites) {
            int spriteId = s->getID();
            if (s->solid && spriteId != id) {
                if (doObjectsCollide(*this, *s)) {
                    spriteCollisions.push_back(s);
                    tagCollisions.push_back(s->tag);
                    s->addTagCollision(tag);
                    collision = true;
                    // printf("%s colliding with %s\n", tag.c_str(), s->tag.c_str());
                }
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

            int left = std::floor((x - 0.5f*width) / (float)tileWidth);
            int right = std::ceil((x + 0.5f*width) / (float)tileWidth);
            int bottom = std::floor((y - 0.5f*height) / (float)tileHeight);
            int top = std::ceil((y + 0.5f*height) / (float)tileHeight);

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

                    CollidableObject tileObj;
                    tileObj.tag = tile.tag;
                    tileObj.x = c*tileWidth;
                    tileObj.y = r*tileHeight;
                    tileObj.width = tileWidth;
                    tileObj.height = tileHeight;
                    tileObj.scale = 1.0f;
                    tileObj.angle = 0.0f;
                    tileObj.solid = true;
                    tileObj.anchorX = "centre";
                    tileObj.anchorY = "centre";
                    tileObj.hitboxes = Global::level->getHitboxes(t);

                    if (doObjectsCollide(*this, tileObj)) {
                        collision = true;
                        if (tile.tag.size() > 0) {
                            tagCollisions.push_back(tile.tag);
                            break;
                        } 
                    }
                }
                if (collision) { break; }
            }
            if (collision) { break; }
        }

        if (collision) {
            x = lastX;
        }

        y += velocityY*((glfwGetTime()*1000 - Global::last_frame_time)/1000.0f);
        currentHitboxes.clear();

        collision = false;

        for (Sprite *s : sprites) {
            int spriteId = s->getID();
            if (s->solid && this != s) {
                if (doObjectsCollide(*this, *s)) {
                    spriteCollisions.push_back(s);
                    tagCollisions.push_back(s->tag);
                    s->addTagCollision(tag);

                    collision = true;
                }
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

            int left = std::floor((x - 0.5f*width) / (float)tileWidth);
            int right = std::ceil((x + 0.5f*width) / (float)tileWidth);
            int bottom = std::floor((y - 0.5f*height) / (float)tileHeight);
            int top = std::ceil((y + 0.5f*height) / (float)tileHeight);

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

                    CollidableObject tileObj;
                    tileObj.tag = tile.tag;
                    tileObj.x = c*tileWidth;
                    tileObj.y = r*tileHeight;
                    tileObj.width = tileWidth;
                    tileObj.height = tileHeight;
                    tileObj.scale = 1.0f;
                    tileObj.angle = 0.0f;
                    tileObj.solid = true;
                    tileObj.anchorX = "centre";
                    tileObj.anchorY = "centre";
                    tileObj.hitboxes = Global::level->getHitboxes(t);

                    if (doObjectsCollide(*this, tileObj)) {
                        collision = true;
                        break;
                    }
                }
                if (collision) { break; }
            }
            if (collision) { break; }
        }

        if (collision) {
            y = lastY;
            velocityY = 0;
            grounded = true;
        }
    }

    velocityX = 0.0f;
    if (gravity) {
        velocityY -= 5.0f;
    }
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

bool Sprite::isCollidingWith(Sprite *sprite) {
    if (std::find(spriteCollisions.begin(), spriteCollisions.end(), sprite) != spriteCollisions.end()) {
        return true;
    }
    return false;
}

bool Sprite::isCollidingWithTag(std::string tag) {
    if (std::find(tagCollisions.begin(), tagCollisions.end(), tag) != tagCollisions.end()) {
        return true;
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

void Sprite::addTagCollision(std::string tag) {
    if (std::find(tagCollisions.begin(), tagCollisions.end(), tag) == tagCollisions.end()) {
        tagCollisions.push_back(tag);
    }
}


