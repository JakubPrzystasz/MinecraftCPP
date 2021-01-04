#pragma once
#pragma warning( disable : 4506 )
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
	bool updateChunk;
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

	GLuint chunkSize = 16;

	std::unordered_map<vec3,BlockName> blocks;
	
	void PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z);
	void PutBlock(BlockName blockName, glm::vec3 pos);
	void PutBlock(BlockName blockName, vec3 pos);

	inline BlockName GetBlock(vec3 position);
	
	void Draw(Camera &camera);

	Face AddPosToFace(vec3 pos, Face& face);

	inline glm::vec3 ToWorldPosition(vec3 pos);
	inline glm::vec3 ToWorldPosition(vec3 pos,vec2 chunkPos);

	friend World;
};

