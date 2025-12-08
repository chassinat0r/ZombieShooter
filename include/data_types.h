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
    int timeout;
    bool hasDurability;
    int currentDurability;
    int maxDurability;
    bool doesReload;
    int reloadTime;
    int loadedAmmo;
    int maxLoadedAmmo;
    int totalAmmo;
} Inv_Item;

#endif
