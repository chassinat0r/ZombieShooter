#ifndef HOTBAR_H
#define HOTBAR_H

#include <data_types.h>

class Hotbar {
    public:
        Hotbar();
        Hotbar(Inv_Item items[2]);

        Inv_Item * selectItem(int index);
        Inv_Item * getSelectedItem();

        void reload();
        void draw();
        void update();

    private:
        Inv_Item *items[2] = { nullptr, nullptr };

        int selectedIndex = 0;
};

#endif
