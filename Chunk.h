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
public:
	void Init();
	glm::vec2 chunkPosition;

	std::vector<BlockPos> blocks;
	
	void PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z);
	
	void Draw(Camera &camera);

	bool FindAdjacent(glm::vec3 position);

	bool isBlock(glm::vec3 position);
};

