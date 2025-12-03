#ifndef FONTMGR_H
#define FONTMGR_H

#include <engine/shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <map>
#include <string>

typedef struct {
    unsigned int texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
} FontChar;

class FontManager {
    public:
        static void init();
        static void loadFont(std::string path, std::string fontName, int size);
        static void drawText(std::string text, std::string fontName, int x, int y, glm::vec3 color);

        inline static FT_Library ft;

        inline static std::map<std::string, std::map<char, FontChar>> loadedFonts;

        inline static Shader *textShader;
};

#endif
