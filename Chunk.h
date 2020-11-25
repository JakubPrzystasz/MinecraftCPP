#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

class Chunk
{
public:
	std::array<glm::vec3,16*16> blocks;
	Chunk() {
		for (GLuint x = 0; x < 16; x++) {
			for (GLuint z = 0; z < 16; z++) {
				blocks[16*x + z] = glm::vec3(x,0.f,z);
			}
		}
	}
};

