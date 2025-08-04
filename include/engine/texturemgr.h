#ifndef TEXTUREMGR_H
#define TEXTUREMGR_H

#include <engine/types.h>
#include <engine/shader.h>

#include <map>
#include <string>

#define DEF_WIDTH 200
#define DEF_HEIGHT 150

class TextureManager {
    public:
        inline static unsigned int VBO, VAO, EBO;

        inline static float vertices[12] = {
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        inline static float texCoords[8];

        inline static unsigned int indices[6] = {
            0, 1, 3,
            1, 2, 3
        };

        inline static std::map<std::string, Texture> textures;

        inline static Shader *worldShader;
        inline static Shader *screenShader;

        inline static float texWidth;
        inline static float texHeight;

        static void init();
        static void loadTex(std::string filename, std::string name, int rows = 1, int cols = 1);
        static void setTex(std::string name, int r1 = 0, int c1 = 0, int r2 = 1, int c2 = 1);
        static void drawTex(int x, int y, float scale, Camera camera);

        static float getTexWidth(std::string name, int c1, int c2);
        static float getTexHeight(std::string name, int r1,int r2);
};

#endif
