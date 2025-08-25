#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>
#include <map>

#include <engine/types.h>
#include <engine/animation.h>

class Sprite {
    public:
        Sprite();
        Sprite(float x, float y, float scale = 1.0f, bool solid = false);

        void addAnimation(std::string name, Animation animation);
        void setAnimation(std::string name);
        void draw(Camera camera);
        void move(float dx, float dy);
        virtual void update();

        void addHitbox(std::string animationName, int frame, int x1, int y1, int x2, int y2);
        void addCollisionLayer(int layer);
        bool doesCollideOnLayer(int layer);
        
        float getX();
        float getY();

        int getID();

        bool isCollidingWith(Sprite sprite);
        std::vector<Rect_F> getHitboxes();

        bool hasMoved();
        
        bool solid;

    protected:
        float x;
        float y;
        float scale;

        std::map<std::string,Animation> animations;
        std::map<std::string, std::map<int, std::vector<Rect>>> hitboxes;
        std::vector<Rect_F> currentHitboxes;
        
        std::string currentAnimation;
        int currentFrame = 0;
        double timer = 0;
        double lastUpdate = 0;

        inline static std::vector<Sprite*> sprites;
        inline static int count = 0;
        int id;

        float lastX;
        float lastY;
        float velocityX = 0.0f;
        float velocityY = 0.0f;

        std::vector<int> collisionLayers;
};

#endif
