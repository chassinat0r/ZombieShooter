#include <engine/fontmgr.h>
#include <engine/texturemgr.h>

#include <constants.h>
#include <global.h>

#include <stdio.h>

void FontManager::init() {
    if (FT_Init_FreeType(&FontManager::ft)) {
        printf("Failed to initialise FreeType library\n");
    }

    textShader = new Shader("shaders/text.vert", "shaders/text.frag");
}

void FontManager::loadFont(std::string path, std::string fontName, int size) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    FT_Face face;

    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        printf("Failed to load font at %s\n", path.c_str());
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    std::map<char,FontChar> fontChars;

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("Failed to load glyph of %c\n", c);
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        FontChar fc = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        fontChars.insert({c, fc});
    }

    loadedFonts.insert({fontName, fontChars});

    FT_Done_Face(face);
}

void FontManager::drawText(std::string text, std::string fontName, int x, int y, glm::vec3 color) {
    int originalX = x;

    float left = -Global::width*0.5f;
    float right = Global::width*0.5f;
    float bottom = -Global::height*0.5f;
    float top = Global::height*0.5f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

    glBindVertexArray(TextureManager::VAO);
    glBindBuffer(GL_ARRAY_BUFFER, TextureManager::VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    textShader->use();
    glUniform3f(glGetUniformLocation(textShader->ID, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(TextureManager::VAO);

    std::map<char, FontChar> fontChars = loadedFonts.at(fontName);

    float strWidth = 0;
    
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++) {
        FontChar fc = fontChars[*c];

        strWidth += fc.advance >> 6;
    }

    float xf = x - 0.5f*strWidth;
    float yf = y;

    for (c = text.begin(); c != text.end(); c++) {
        FontChar fc = fontChars[*c];

        float xPos = xf + fc.bearing.x;
        float yPos = yf - (fc.size.y - fc.bearing.y);

        float w = fc.size.x;
        float h = fc.size.y;

        float vertices[6][4] = {
            { xPos,     yPos + h,   0.0f, 0.0f },            
            { xPos,     yPos,       0.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 1.0f },

            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos + w, yPos,       1.0f, 1.0f },
            { xPos + w, yPos + h,   1.0f, 0.0f }           
        };

        glBindTexture(GL_TEXTURE_2D, fc.texture);

        glBindBuffer(GL_ARRAY_BUFFER, TextureManager::VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        xf += (fc.advance >> 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
