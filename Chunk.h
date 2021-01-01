#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include "ResourceManager.h"
#include "Vectors.h"



class Chunk : public Model
{
private:
	bool updateChunk;
public:
	Chunk() {
		updateChunk = true;
		chunkPosition = vec2(0,0);
	};

	Chunk(vec2 chunkPos) {
		updateChunk = true;
		chunkPosition = chunkPos;
	};

	void Init();

	vec2 chunkPosition;
	GLuint chunkSize = 16;
	std::unordered_map<vec3,BlockName> blocks;
	
	void PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z);
	
	void Draw(Camera &camera);

	bool FindAdjacent(vec3 position);

	void ChunkUpdate();

	Face AddPosToFace(vec3 pos, Face& face);

	glm::vec3 ToWorldPosition(vec3 pos);
};

