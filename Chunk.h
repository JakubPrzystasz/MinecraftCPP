#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ResourceManager.h"

struct BlockPos{
	BlockName block;
	glm::vec3 pos;
};

class Chunk : public Model
{
private:
	bool updateChunk;
public:
	Chunk() {
		updateChunk = true;
		glm::vec2 chunkPosition = glm::vec2(0,0);
	};

	Chunk(glm::vec2 chunkPos) {
		updateChunk = true;
		glm::vec2 chunkPosition = chunkPos;
	};

	void Init();
	glm::vec2 chunkPosition;
	GLuint chunkSize = 16;
	std::vector<BlockPos> blocks;
	
	void PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z);
	
	void Draw(Camera &camera);

	bool FindAdjacent(glm::vec3 position);

	void ChunkUpdate();

	Face AddPosToFace(glm::vec3 pos, Face& face);

	glm::vec3 ToWorldPosition(glm::vec3 pos);
};

