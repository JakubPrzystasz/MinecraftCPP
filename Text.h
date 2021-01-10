#pragma once
#include <iostream>
#include <string>
#include "ResourceManager.h"


struct Character : public Model {
    GLuint texture;
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    GLuint Advance;   // Horizontal offset to advance to next glyph
    Character(Character* _tmp) {
        texture = _tmp->texture;
        Size = glm::ivec2(_tmp->Size);
        Bearing = glm::ivec2(_tmp->Bearing);
        Advance = _tmp->Advance;
    }
    Character(GLuint _texture, glm::ivec2 _Size, glm::ivec2 _Bearing, GLuint _Advance) {
        texture = _texture;
        Size = _Size;
        Bearing = _Bearing;
        Advance = _Advance;
    }
    Character() {};
};


class Text{
public:
    Text() {};
    void Init();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};