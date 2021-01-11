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
	
	vec2 chunkPosition;

	std::unordered_map<vec3, BlockName> blocks;

	GLuint faces;

	void BuildMesh(std::unique_ptr<Model> model);
public:

	Chunk(vec2 ChunkPos) {
		chunkPosition = ChunkPos;
		faces = static_cast<GLuint>(0); 
		blocks = std::unordered_map<vec3, BlockName>();
	};

	~Chunk() {
	}
	
	void PutBlock(BlockName blockName, vec3 pos);

	void PutBlock(BlockName blockName, GLuint x, GLuint y, GLuint z);

	BlockName GetBlock(vec3 position);

	GLuint CountFaces();

	Face AddPosToFace(vec3 pos, Face& face);

	glm::vec3 ToWorldPosition(vec3 pos);

	static glm::vec3 ToWorldPosition(vec3 pos,vec2 chunkPos);

	friend World;
};

