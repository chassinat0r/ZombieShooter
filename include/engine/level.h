#ifndef LEVEL_H
#define LEVEL_H

#include <engine/texturemgr.h>
#include <engine/types.h>

#include <string>
#include <vector>
#include <map>

class Level {
    public:
        Level();
        
        int addTile(std::string textureName, int r1 = 0, int c1 = 0, int r2 = 1, int c2 = 1, bool solid = false);
        void addHitbox(int tile, int x1, int y1, int x2, int y2);

        int newLayer(int tileWidth, int tileHeight);
        void fillLayer(int layer, int tile, int x1, int y1, int x2, int y2);

        void render(Camera camera);

    private:
        std::vector<std::map<int,std::map<int, int>>> layers;
        std::map<int,std::vector<Rect>> hitboxes;
        std::vector<Tile> tiles;

        std::map<int,std::pair<int, int>> layerTileSizes;
};

#endif
