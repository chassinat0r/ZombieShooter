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

        void setTile(int l, int tile, int x, int y);
        int newLayer(int tileWidth, int tileHeight);
        void fillLayer(int layer, int tile, int x1, int y1, int x2, int y2);

        int getLayerCount();
        std::map<int, std::map<int,int>> getLayer(int l);

        Tile getTile(int t);
        std::vector<Rect> getHitboxes(int t);
        std::pair<int,int> getTileSize(int l);
        
        void render(Camera camera, bool debug = false);
        void drawHitboxes(float scale, Camera camera, std::vector<Rect_F> realHitboxes);

        inline static unsigned int VAO, VBO, EBO;

        inline static Shader *hbShader;
        
    private:
        std::vector<std::map<int,std::map<int, int>>> layers;
        std::map<int,std::vector<Rect>> hitboxes;
        std::vector<Tile> tiles;

        std::map<int,std::pair<int, int>> layerTileSizes;

};

#endif
