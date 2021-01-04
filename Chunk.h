#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include "ResourceManager.h"
#include "Vectors.h"
#include "World.h"

class World;
struct vec3;

class Chunk : public Model
{
private:
	std::atomic<bool> updateChunk;
	void ChunkUpdate();

public:
	Chunk() {
		updateChunk = true;
		chunkPosition = vec2(0,0);
		world = nullptr;
	};

	Chunk(vec2 chunkPos) {
		updateChunk = true;
		chunkPosition = chunkPos;
		world = nullptr;
	};

	void Init();

	World* world;

	vec2 chunkPosition;

	int chunkSize = 16;

	std::unordered_map<vec3,BlockName> blocks;
	
	void PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z);
	void PutBlock(BlockName blockName, glm::vec3 pos);
	void PutBlock(BlockName blockName, vec3 pos);
	
	void Draw(Camera &camera);

<<<<<<< HEAD
=======
	bool FindAdjacent(vec3 position) const;

	void ChunkUpdate();

>>>>>>> parent of 439d9a6... Fix chunk mesh generation
	Face AddPosToFace(vec3 pos, Face& face);

	inline glm::vec3 ToWorldPosition(vec3 pos);
	inline glm::vec3 ToWorldPosition(vec3 pos,vec2 chunkPos);

	friend World;
};

