#ifndef HOTBAR_H
#define HOTBAR_H

#include <data_types.h>

class Hotbar {
    public:
        Hotbar();
        Hotbar(Inv_Item items[5]);

        Inv_Item * selectItem(int index);

        void draw();

    private:
        Inv_Item *items[5];

        int selectedIndex = 0;
};

#endif
