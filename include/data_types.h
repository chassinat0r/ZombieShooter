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
    bool hasDurability;
    int currentDurability;
    int maxDurability;
} Inv_Item;

#endif
