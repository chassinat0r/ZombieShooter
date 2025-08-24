#include <engine/level.h>

#include <algorithm>
#include <stack>

Level::Level() {

}

int Level::addTile(std::string textureName, int r1, int c1, int r2, int c2, bool solid) {
    int size = tiles.size();
    Tile tile = { textureName, r1, c1, r2, c2, solid };
    tiles.push_back(tile);
    return size;
}

void Level::addHitbox(int tile, int x1, int y1, int x2, int y2) {
    std::vector<Rect> hitboxesForTile;
    if (hitboxes.count(tile) != 0) {
        hitboxesForTile = hitboxes.at(tile);
    }

    hitboxesForTile.push_back({ x1, y1, x2, y2 });
    hitboxes[tile] = hitboxesForTile;
}

void Level::setTile(int l, int tile, int x, int y) {
    if (layers.size() > l) {
        std::map<int,std::map<int, int>> layer = layers[l];
        
        std::map<int, int> row;

        if (layer.count(y) > 0) {
            row = layer[y];
        }

        row[x] = tile;
        layer[y] = row;
        layers[l] = layer;
    }
}

int Level::newLayer(int tileWidth, int tileHeight) {
    int size = layers.size();
    std::map<int,std::map<int, int>> newMap;
    layers.push_back(newMap);
    layerTileSizes[size] = { tileWidth, tileHeight };
    return size;
}

void Level::fillLayer(int layer, int tile, int x1, int y1, int x2, int y2) {
    std::map<int,std::map<int, int>> layerMap = layers[layer];
    for (int y = std::min(y1, y2); y < std::max(y1, y2); y++) {
        std::map<int,int> rowOfMap;
        if (layerMap.count(y) != 0) {
            rowOfMap = layerMap[y];
        }

        for (int x = std::min(x1, x2); x < std::max(x1, x2); x++) {
            rowOfMap[x] = tile;
        }


        layerMap[y] = rowOfMap;
    }

    layers[layer] = layerMap;

}

void Level::render(Camera camera) {
    std::map<int, std::pair<int, int>> lastRow;
    
    int layerNo = 0;
    for (std::map<int,std::map<int,int>> layer : layers) {
        int layerTileWidth = layerTileSizes[layerNo].first;
        int layerTileHeight = layerTileSizes[layerNo].second;
        std::stack<int> rows;

        for (auto it : layer) {
            rows.push(it.first);
        }

        while (!rows.empty()) {
            int r = rows.top();
            std::map<int,int> row = layer[r];
            for (auto it2 : row) {
                int c = it2.first;
                int t = it2.second;

                float x = (c * layerTileWidth);
                float y = (r * layerTileHeight);

                Tile tile = tiles[t];
                TextureManager::setTex(tile.textureName, tile.r1, tile.c1, tile.r2, tile.c2);

                int texWidth = TextureManager::getTexWidth(tile.textureName, tile.c1, tile.c2);
                int texHeight = TextureManager::getTexHeight(tile.textureName, tile.r1, tile.r2);

                if (texWidth > layerTileWidth) {
                    int diff = texWidth - layerTileWidth;
                    x -= 0.5f*(float)diff;
                }

                if (texHeight > layerTileHeight) {
                    int diff = texHeight - layerTileHeight;
                    y -= 0.5f*(float)diff;
                }
                
                TextureManager::drawTex(x, y, 1.0f, camera);
            }

            rows.pop();
        }

        layerNo++;
    }
}

int Level::getLayerCount() {
    return layers.size();
}

std::map<int, std::map<int,int>> Level::getLayer(int l) {
    return layers[l];
}

Tile Level::getTile(int t) {
    return tiles[t];
}

std::vector<Rect> Level::getHitboxes(int t) {
    return hitboxes[t];
}

std::pair<int, int> Level::getTileSize(int l) {
    return layerTileSizes[l];
}
