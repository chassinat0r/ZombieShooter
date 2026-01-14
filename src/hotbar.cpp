#include <hotbar.h>

#include <engine/texturemgr.h>
#include <engine/fontmgr.h>
#include <global.h>
#include <constants.h>
#include <util.h>

#include <GLFW/glfw3.h>

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

    Inv_Item *selectedItem = getSelectedItem();

    if (selectedItem->itemType == LD_RANGED && selectedItem->doesReload) {
        char ammoText[30];
        sprintf(ammoText, "Loaded: %d Reserves: %d", selectedItem->loadedAmmo, selectedItem->maxLoadedAmmo);
        std::string ammoTextStr(ammoText);
        FontManager::drawText(ammoTextStr, "arial24", 0, -0.5f*Global::height + 100, glm::vec3(150, 150, 150));
    }
}

void Hotbar::update() {
    for (Inv_Item *item : items) {
        if (item != nullptr) {
            if (item->isInTimeout) {
                item->timeout += 1000*glfwGetTime() - (Global::last_frame_time);
                if (item->timeout >= item->timeoutDuration) {
                    item->isInTimeout = false;
                    item->timeout = 0;
                }
            }

            if (item->isReloading) {
                item->reloadTimer += 1000*glfwGetTime() - (Global::last_frame_time);
                if (item->reloadTimer >= item->reloadTimePerAmmo) {
                    item->loadedAmmo++;
                    item->totalAmmo--;
                    if (item->loadedAmmo == item->maxLoadedAmmo || item->totalAmmo == 0) {
                        item->isReloading = false;
                    }
                    item->reloadTimer = 0;
                    printf("%d/%d\n", item->loadedAmmo, item->maxLoadedAmmo);
                }
            }
        }
    }
}

void Hotbar::reload() {
    Inv_Item *selectedItem = getSelectedItem();

    if (selectedItem->isReloading) {
        printf("Already reloading\n");
    } else if (selectedItem->doesReload) {
        if (selectedItem->loadedAmmo == selectedItem->maxLoadedAmmo) {
            printf("Maximum ammo already loaded\n");
        } else if (selectedItem->totalAmmo == 0) {
            printf("No more ammo left\n");
        } else {
            selectedItem->isReloading = true;
            selectedItem->reloadTimer = 0;
        }
    }
}
