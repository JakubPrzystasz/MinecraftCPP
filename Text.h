#pragma once
#include <ft2build.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "ResourceManager.h"
#include FT_FREETYPE_H  

struct Character {
    GLuint Texture; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    GLuint Advance;   // Horizontal offset to advance to next glyph
    Character(Character* _tmp) {
        Texture = _tmp->Texture;
        Size = glm::ivec2(_tmp->Size);
        Bearing = glm::ivec2(_tmp->Bearing);
        Advance = _tmp->Advance;
    }
    Character(GLuint _Texture, glm::ivec2 _Size, glm::ivec2 _Bearing, GLuint _Advance) {
        Texture = _Texture;
        Size = _Size;
        Bearing = _Bearing;
        Advance = _Advance;
    }
    Character() {};
};


class Text : public Model {
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    Shader* shader;
public:
    Text() {};
    void Init();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};