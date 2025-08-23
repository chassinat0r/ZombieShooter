#include <zombie.h>
#include <vector>
#include <algorithm>

#include <global.h>

void Zombie::setTarget(int id) {
    this->targetId = id;
}

void Zombie::update() {
    Pos_F nextStep;
    Pos_F direction = {0.0f, 0.0f};
    
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

        velocityX += direction.x*16.0f;
        velocityY += direction.y*16.0f;
    }

    Sprite::update();

    if (path.size() > 0) {
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
    Sprite *target;
    for (Sprite *s : Sprite::sprites) {
        if (s->getID() == targetId) {
            target = s;
            break;
        }
    }

    std::vector<Rect_F> targetHitboxes = target->getHitboxes();
    
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

        Rect_F projHb = { 
            cx - 0.5f*scale*texWidth, cy - 0.5f*scale*texHeight,
            cx + 0.5f*scale*texWidth, cy + 0.5f*scale*texHeight,
        };


        for (Rect_F hb : targetHitboxes) {
            float x1 = std::min(hb.x1, hb.x2);
            float x2 = std::max(hb.x1, hb.x2);
            float y1 = std::min(hb.y1, hb.y2);
            float y2 = std::max(hb.y1, hb.y2);

            if (std::min(projHb.x2, x2) > std::max(projHb.x1, x1) && std::min(projHb.y2, y2) > std::max(projHb.y1, y1)) {
                found = true;
                break;
            }
        }

        if (found) {
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
            // printf("X: %.2f, Y: %.2f\n", current.first, current.second);
            if (current == start) { break; }
            path.push({current.first, current.second});
        }
    }
}

