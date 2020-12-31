#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "stb_perlin.h"

struct WorldChunk {
	glm::vec2 Position;
	Chunk chunk;
	WorldChunk(glm::vec2 pos, Chunk _chunk) {
		Position = pos;
		chunk = _chunk;
	}
};

class World
{
	std::vector<WorldChunk> Chunks;
public:

	World() {
		Chunks = std::vector<WorldChunk>();
	};

	void DrawChunks(Camera &camera);

	void GenerateWorld();

};

