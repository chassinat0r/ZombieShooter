#ifndef EVENT_ZOMBIEHIT
#define EVENT_ZOMBIEHIT

#include <zombie.h>
#include <vector>

class ZombieHitListener {
    public:
        virtual void onZombieHit(Zombie *zombie) = 0;
};

class ZombieHitEvent {
    public:
        ZombieHitEvent(Zombie *zombie);

        static void addListener(ZombieHitListener *listener);

    private:
        inline static std::vector<ZombieHitListener*> listeners;
};

#endif
