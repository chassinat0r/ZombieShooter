#include <hotbar.h>

#include <engine/texturemgr.h>
#include <global.h>
#include <constants.h>
#include <util.h>

Hotbar::Hotbar() {

}

Hotbar::Hotbar(Inv_Item items[2]) {
    for (int i = 0; i < 2; i++) {
        this->items[i] = &items[i];
    }
}

Inv_Item * Hotbar::selectItem(int index) {
    if (index > 1) {
        index = 1;
    }
    if (index < 0) {
        index = 0;
    }

    selectedIndex = index;

    return items[selectedIndex];
}

Inv_Item * Hotbar::getSelectedItem() {
    return items[selectedIndex];
}

void Hotbar::draw() {
    std::pair<float,float> wh = getRenderWidthAndHeight();
    float w = wh.first;
    float h = wh.second;
    
    const int texWidth = TextureManager::getTexWidth("hotbar_slot", 0, 1);
    const int texHeight = TextureManager::getTexHeight("hotbar_slot", 0, 1);

    const int NUMBER_OF_SLOTS = 2;

    float posX = -texWidth*0.5f*(NUMBER_OF_SLOTS-1);
    float posXSelected = posX + texWidth*selectedIndex;
    float posY = -0.5f*h + 0.5f*texWidth + 5;

    TextureManager::setTex("hotbar_slot", 0, 0, 1, 1);

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        TextureManager::drawTex(posX, posY, 1.0f, 0);

        Inv_Item *item = items[i];

        if (item != nullptr) {
            switch (item->itemId) {
                case 0:
                    TextureManager::setTex("knife", 0, 0, 1, 1);
                    break;
                case 1:
                    TextureManager::setTex("pistol", 0, 0, 1, 1);
                    break;
            }

            TextureManager::drawTex(posX, posY, 0.8f, 0);
            TextureManager::setTex("hotbar_slot", 0, 0, 1, 1);
        }
        posX += texWidth;
    }

    TextureManager::setTex("hotbar_selected", 0, 0, 1, 1);
    TextureManager::drawTex(posXSelected, posY, 1.0f, 0);
}
