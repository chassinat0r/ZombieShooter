#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef enum {
    MELEE,
    LD_RANGED,
    HD_RANGED,
    CURSE,
    POWERUP
} Item_Type;

typedef struct {
    int itemId;
    Item_Type itemType;
    bool isInTimeout;
    int timeout;
    int timeoutDuration;
    bool hasDurability;
    int currentDurability;
    int maxDurability;
    bool doesReload;
    bool isReloading;
    int reloadTimePerAmmo;
    int reloadTimer;
    int loadedAmmo;
    int maxLoadedAmmo;
    int totalAmmo;
} Inv_Item;

#endif
