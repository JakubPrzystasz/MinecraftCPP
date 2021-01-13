#pragma once
#pragma warning( disable : 4506 )
#include <glm/glm.hpp>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include "ResourceManager.h"
#include "Vectors.h"
#include "World.h"
#include "Cube.h"

class World;
struct vec3;
enum class BlockName;
class Model;

class Chunk
{
private:

	std::mutex Mutex;
	std::atomic<bool> updateChunk;

	vec2 chunkPosition;

	std::unordered_map<vec3, BlockName> blocks;

	GLuint faces;

	Model* model;
	std::vector<Vertex>* backVertices;
	std::vector<GLuint>* backIndices;


	bool BuildMesh();
public:

	Chunk(vec2 ChunkPos, Model* _model) {
		chunkPosition = ChunkPos;
		model = _model;
		faces = static_cast<GLuint>(0);
		blocks = std::unordered_map<vec3, BlockName>();
		backVertices = new std::vector<Vertex>;
		backIndices = new std::vector<GLuint>;
	};

	~Chunk() {
		delete backIndices;
		delete backVertices;
	}

	void PutBlock(BlockName blockName, vec3 pos);
	void PutBlock(BlockName blockName, GLuint x, GLuint y, GLuint z);

	BlockName GetBlock(vec3 position);


	Face AddPosToFace(vec3 pos, Face& face);

	glm::vec3 ToWorldPosition(vec3 pos);

	static glm::vec3 ToWorldPosition(vec3 pos, vec2 chunkPos);

	friend World;
};

