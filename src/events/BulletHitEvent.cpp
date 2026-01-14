#include <events/BulletHitEvent.h>

BulletHitEvent::BulletHitEvent(BulletHitListener *hit) {
    hit->onBulletHit();
}
