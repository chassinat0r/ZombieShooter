#include <events/ZombieHitEvent.h>

ZombieHitEvent::ZombieHitEvent(Zombie *zombie) {
    for (ZombieHitListener *listener : listeners) {
        listener->onZombieHit(zombie);
    }
}

void ZombieHitEvent::addListener(ZombieHitListener *listener) {
    listeners.push_back(listener);
}
