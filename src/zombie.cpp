#include <zombie.h>
#include <vector>
#include <algorithm>

#include <global.h>
#include <util.h>

void Zombie::setTarget(int id) {
    this->targetId = id;
}

void Zombie::update() {
    Sprite *target;
    for (Sprite *s : Sprite::sprites) {
        if (s->getID() == targetId) {
            target = s;
            break;
        }
    }

    bool findPath = false;
    
    Pos_F direction = {0.0f, 0.0f};
    Pos_F nextStep;

    if (!isCollidingWith(*target)) {
        findPath = true;
    
        if (path.size() > 0) {
            nextStep = path.top();
            if (x < nextStep.x) {
                direction.x = 1.0f;
            } else if (x > nextStep.x) {
                direction.x = -1.0f;
            } else if (y < nextStep.y) {
                direction.y = 1.0f;
            } else if (y > nextStep.y) {
                direction.y = -1.0f;
            }

            velocityX += direction.x*13.0f;
            velocityY += direction.y*13.0f;
        }
    }

    Sprite::update();

    if (path.size() > 0 && findPath) {
        if (direction.x == 1.0f && x >= nextStep.x) {
            x = nextStep.x;
            path.pop();
        }
        if (direction.x == -1.0f && x <= nextStep.x) {
            x = nextStep.x;
            path.pop();
        }
        if (direction.y == 1.0f && y >= nextStep.y) {
            y = nextStep.y;
            path.pop();
        }
        if (direction.y == -1.0f && y <= nextStep.y) {
            y = nextStep.y;
            path.pop();
        }
    }
}

void Zombie::getPathToTarget() {
    while(!path.empty()) {
        path.pop();
    }
    Sprite clone = *this;
    clone.setSolid(false);
    clone.setAnimation("zombie_front_still");

    Sprite *target;
    for (Sprite *s : Sprite::sprites) {
        if (s->getID() == targetId) {
            target = s;
            break;
        }
    }

    if (isCollidingWith(*target)) {
        return;
    }

    std::map<std::string,std::vector<Rect_F>> targetHitboxes = target->getHitboxes();
    
    std::pair<float,float> start = { x, y }; // Get starting position of 
    std::queue<std::pair<float,float>> frontier;
    frontier.push(start);

    std::map<std::pair<float,float>,std::pair<float,float>> cameFrom;
    cameFrom.insert({{x, y}, {0, 0}});
    
    std::pair<float,float> current;
    
    bool found = false;
    Animation animation = animations[currentAnimation];
    Frame currentFrameObj = animation.getFrame(currentFrame);

    float texWidth = TextureManager::getTexWidth(currentFrameObj.textureName, currentFrameObj.c1, currentFrameObj.c2);
    float texHeight = TextureManager::getTexHeight(currentFrameObj.textureName, currentFrameObj.r1, currentFrameObj.r2);

    while (!frontier.empty()) {
        current = frontier.front();
        frontier.pop();
            
        float cx = current.first;
        float cy = current.second;

        clone.setPos(cx, cy);
        clone.update();

        std::map<std::string,std::vector<Rect_F>> projectedHitboxes = clone.getHitboxes();
        std::vector<Rect_F> projectedHitboxesVec = getHitboxVector(projectedHitboxes);

        if (projectedHitboxesVec.size() == 0) { return; }

        bool collidingWithTile = false;
        
        for (int l = 0; l < Global::level->getLayerCount(); l++) {
            if (std::find(collisionLayers.begin(), collisionLayers.end(), l) == collisionLayers.end()) {
                continue;
            }

            std::map<int, std::map<int,int>> layer = Global::level->getLayer(l);
            
            std::pair<int,int> tileDimensions = Global::level->getTileSize(l);
            int tileWidth = tileDimensions.first;
            int tileHeight = tileDimensions.second;

            int left;
            int right;
            int bottom;
            int top;

            bool leftDefined = false;
            bool rightDefined = false;
            bool bottomDefined = false;
            bool topDefined = false;

            for (Rect_F projHb : projectedHitboxesVec) {
                int tempLeft = std::round(projHb.x1 / tileWidth);
                int tempRight = std::round(projHb.x2 / tileWidth);
                int tempBottom = std::round(projHb.y1 / tileHeight);
                int tempTop = std::round(projHb.y2 / tileHeight);

                if (tempLeft < left || !leftDefined) {
                    left = tempLeft;
                    leftDefined = true;
                }
                if (tempRight > right || !rightDefined) {
                    right = tempRight;
                    rightDefined = true;
                }
                if (tempBottom < bottom || !bottomDefined) {
                    bottom = tempBottom;
                    bottomDefined = true;
                }
                if (tempTop > top || !topDefined) {
                    top = tempTop;
                    topDefined = true;
                }
            }

            left--;
            right++;
            bottom--;
            top++;

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
                    if (doObjectsCollide(projectedHitboxesVec, realTileHitboxes)) {
                        collidingWithTile = true;
                    }
                }

                if (collidingWithTile) {
                    break;
                }
            }
            if (collidingWithTile) {
                break;
            }
        }

        if (collidingWithTile) {
            continue;
        }

        if (clone.isCollidingWith(*target)) {
            found = true;
            break;
        }

        // calculate graph neighbours
        std::pair<float,float> potentialNeighbours[] = {{cx - 4.0f, cy}, {cx + 4.0f, cy}, {cx, cy - 4.0f}, {cx, cy + 4.0f}};

        for (std::pair<float,float> neighbour : potentialNeighbours) {
            if (cameFrom.find(neighbour) == cameFrom.end()) {
                frontier.push(neighbour);

                cameFrom.insert({neighbour, current});
            }
        }
    }
    if (found) {
        while (true) {
            current = cameFrom[current];
            if (current == start || cameFrom[current] == current) { break; }
    
            path.push({current.first, current.second});
        }
    }
}

