#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "stb_perlin.h"
#include <unordered_map>


class World
{
	GLuint chunkSize;
	std::unordered_map<vec2,Chunk> Chunks;
public:
	
	World() {
		Chunks = std::unordered_map<vec2, Chunk>();
	};

	World(GLuint chunkSize) {
		this->chunkSize = chunkSize;
		Chunks = std::unordered_map<vec2, Chunk>();
	}

	void DrawChunks(Camera &camera);

	void GenerateWorld();

	void SetBlock(glm::vec3 pos, BlockName block);

	//BlockPos* GetBlock(glm::vec3 pos);
	bool IsBlock(glm::vec3 pos);

};

