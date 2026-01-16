#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>
#include <map>

#include <engine/types.h>
#include <engine/animation.h>

class Sprite : public CollidableObject {
    public:
        Sprite();
        Sprite(float x, float y, std::string tag, float scale = 1.0f, bool solid = false, bool gravity = false, bool dud = false);
        
        void addAnimation(std::string name, Animation animation);
        void setAnimation(std::string name);
        void resetHitboxes();

        virtual void draw(Camera *camera);
        static void drawAll(Camera *camera);

        void move(float dx, float dy);
        void jump(float jumpSpeed = 200.0f);
        void rotate(float diff);
        void setAngle(float angle);

        virtual void update();
        static void updateAll();
        
        void addHitbox(std::string animationName, int frame, int x1, int y1, int x2, int y2);
        void addCollisionLayer(int layer);

        bool doesCollideOnLayer(int layer);
        
        float getX();
        float getY();
        float getVelocityX();
        float getVelocityY();
        float getAngle();

        void setPos(float x, float y);
        void setSolid(bool solid);
        
        int getID();

        bool isCollidingWith(Sprite *sprite);
        bool isCollidingWithTag(std::string tag);
        std::vector<Rect_F> getHitboxes();

        bool hasMoved();
        bool isGrounded();
        
        inline static std::vector<Sprite*> sprites;

        void addTagCollision(std::string tag);

    protected:
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
        bool lastPosSet = false;
        float velocityX = 0.0f;
        float velocityY = 0.0f;

        std::vector<int> collisionLayers;

        bool grounded = false;
        bool gravity = false;

        std::vector<Sprite*> spriteCollisions;
        std::vector<std::string> tagCollisions;
};

#endif
