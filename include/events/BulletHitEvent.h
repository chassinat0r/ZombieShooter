#ifndef EVENT_BULLETHIT
#define EVENT_BULLETHIT

#include <bullet.h>

class BulletHitListener {
    public:
        virtual void onBulletHit() = 0;
};

class BulletHitEvent {
    public:
        BulletHitEvent(BulletHitListener *hit);
};

#endif
