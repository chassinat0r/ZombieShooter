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
        Sprite(float x, float y, float scale = 1.0f, bool solid = false, bool dud = false);
        
        void addAnimation(std::string name, Animation animation);
        void setAnimation(std::string name);
        void draw(Camera *camera);
        void move(float dx, float dy);
        void jump(float jumpSpeed = 200.0f);

        virtual void update();
        
        void addHitbox(std::string animationName, int frame, int x1, int y1, int x2, int y2);
        void addCollisionLayer(int layer);

        bool doesCollideOnLayer(int layer);
        
        float getX();
        float getY();
        float getVelocityX();
        float getVelocityY();
        
        void setPos(float x, float y);
        void setSolid(bool solid);
        
        int getID();

        bool isCollidingWith(Sprite sprite);
        std::vector<Rect_F> getHitboxes();

        bool hasMoved();
        bool isGrounded();
        
        inline static std::vector<Sprite*> sprites;

        bool solid;

    protected:
        float x;
        float y;
        float scale;

        std::map<std::string,Animation> animations;
        std::vector<Rect_F> currentHitboxes;
        
        std::string currentAnimation;
        int currentFrame = 0;
        double timer = 0;
        double lastUpdate = 0;

        inline static int count = 0;
        int id;

        float lastX;
        float lastY;
        float velocityX = 0.0f;
        float velocityY = 0.0f;

        std::vector<int> collisionLayers;

        bool grounded = false;
};

#endif
