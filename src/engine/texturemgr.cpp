#include <engine/texturemgr.h>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <util.h>
#include <global.h>

void TextureManager::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    worldShader = new Shader("shaders/world.vert", "shaders/world.frag");
}

void TextureManager::loadTex(std::string filename, std::string name, int rows, int cols) {
    glBindVertexArray(VAO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture tex;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(false);

    unsigned char *data = stbi_load(filename.c_str(), &tex.width, &tex.height, &tex.nrc, 0);
    if (data)  {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Error loading texture at \"%s\"\n", filename.c_str());
        return;
    }

    stbi_image_free(data);
    tex.rows = rows;
    tex.cols = cols;

    textures.insert({name, tex});
}

void TextureManager::setTex(std::string name, int r1, int c1, int r2, int c2) {
    Texture tex = textures.at(name);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glBindVertexArray(VAO);

    float COL_FRAC = 1.0f / (float)tex.cols;
    float ROW_FRAC = 1.0f / (float)tex.rows;

    texCoords[0] = COL_FRAC * c2;
    texCoords[1] = ROW_FRAC * r1;
    texCoords[2] = COL_FRAC * c2;
    texCoords[3] = ROW_FRAC * r2;
    texCoords[4] = COL_FRAC * c1;
    texCoords[5] = ROW_FRAC * r2;
    texCoords[6] = COL_FRAC * c1;
    texCoords[7] = ROW_FRAC * r1;

    float verticesTexCoords[20];
    mergeArrays(vertices, 12, 3, texCoords, 8, 2, verticesTexCoords, 20);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTexCoords), verticesTexCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texWidth = (float)abs(c2 - c1) * ((float)tex.width / (float)tex.cols);
    texHeight = (float)abs(r2 - r1) * ((float)tex.height / (float)tex.rows);

}

void TextureManager::drawTex(int x, int y, float scale, Camera camera) {
    worldShader->use();

    float h = DEF_HEIGHT;
    float w = ((float)DEF_HEIGHT / (float)Global::height) * Global::width; 
    
    float left = -w*0.5f;
    float right = w*0.5f;
    float bottom = -h*0.5f;
    float top = h*0.5f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::scale(view, glm::vec3((float)scale, (float)scale, 1.0f));
    view = glm::scale(view, glm::vec3((float)texWidth, (float)texHeight, 1.0f));
    view  = glm::translate(view, glm::vec3(camera.x, camera.y, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));

    worldShader->setMatrix("projection", projection);
    worldShader->setMatrix("view", view);
    worldShader->setMatrix("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}
