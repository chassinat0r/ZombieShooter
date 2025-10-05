#include <engine/level.h>
#include <global.h>

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

void Level::render(Camera *camera, bool debug) {
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

                float ox = x;
                float oy = y;

                if (texWidth > layerTileWidth) {
                    int diff = texWidth - layerTileWidth;
                    x -= 0.5f*(float)diff;
                }

                if (texHeight > layerTileHeight) {
                    int diff = texHeight - layerTileHeight;
                    y -= 0.5f*(float)diff;
                }
                
                TextureManager::drawTex(x, y, 1.0f, camera);
                if (debug) {
                    std::vector<Rect> hitboxesForTile = getHitboxes(t);
                    std::vector<Rect_F> realHitboxes;
                    for (Rect hb : hitboxesForTile) {
                        Rect_F realHb;
                        realHb.x1 = (ox - 0.5f*(float)layerTileWidth) + hb.x1;
                        realHb.x2 = (ox - 0.5f*(float)layerTileWidth) + hb.x2;
                        realHb.y1 = (oy + 0.5f*(float)layerTileHeight) - hb.y2;
                        realHb.y2 = (oy + 0.5f*(float)layerTileHeight) - hb.y1;
                    
                        realHitboxes.push_back(realHb);
                    }
                    
                    drawHitboxes(1.0f, camera, realHitboxes);

                }
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

void Level::drawHitboxes(float scale, Camera *camera, std::vector<Rect_F> realHitboxes) {
    float vertices[12] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };

    unsigned int indices[6] = {
        0, 1, 3,
        1, 2, 3
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    hbShader->use();

    float h = DEF_HEIGHT;
    float w = ((float)DEF_HEIGHT / (float)Global::height) * Global::width; 
    
    float left = -w*0.5f;
    float right = w*0.5f;
    float bottom = -h*0.5f;
    float top = h*0.5f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

    for (Rect_F hb : realHitboxes) {
        glm::mat4 view = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(-camera->x, -camera->y, 0.0f));    
        view = glm::scale(view, glm::vec3((float)scale, (float)scale, 1.0f));
        float w = hb.x2 - hb.x1;
        float h = hb.y2 - hb.y1;
        view = glm::scale(view, glm::vec3(w, h, 1.0f));
        glm::mat4 model = glm::mat4(1.0f);
        float mx = (hb.x1 + hb.x2) / 2.0f;
        float my = (hb.y1 + hb.y2) / 2.0f;
        model = glm::translate(model, glm::vec3(mx, my, 0.0f));
        hbShader->setMatrix("projection", projection);
        hbShader->setMatrix("view", view);
        hbShader->setMatrix("model", model);
        glUniform3fv(glGetUniformLocation(hbShader->ID, "color"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    }
}
