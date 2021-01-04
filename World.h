#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "stb_perlin.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>

class Chunk;

class World
{
private:
	
	World() {};

	static GLuint renderDistance;

	static World* instance;

	static GLuint chunkSize;
	
	static std::unordered_map<vec2,Chunk*> Chunks;

	static std::vector<Chunk*> RenderedChunks;

	inline static int RoundInt(GLfloat x);

public:
	


	/// <summary>
	/// Do not allow to copy an object
	/// </summary>
	/// <param name="object"></param>
	World(World& object) = delete;

	/// <summary>
	/// Do not allow to assign a instance to new object
	/// </summary>
	/// <param name=""></param>
	void operator=(const World&) = delete;

	/// <summary>
	/// On first run, creates static object of class 
	/// every next use of this function returns pointer to the same object.
	/// </summary>
	/// <returns>Pointer to Singleton instance of ResourceManager</returns>
	static World* GetInstance();

	static void SetChunkSize(GLuint chunkSize);

	static void DrawChunks(Camera &camera);

	static void SetBlock(glm::vec3 pos, BlockName block);

	static Chunk* GenerateChunk(vec2 chunkPos);
	//Takes in chunk coords as parameter
	static BlockName GetBlock(Chunk* chunk,vec3 pos);
	//Takes world coords as parameter
	static BlockName GetBlock(glm::vec3 pos);
	static BlockName GetBlock(vec3 pos);

	static void RequestChunkUpdate(vec2 chunkPos);

	static void RequestChunkGen(vec2 chunkPos);

	static void SetRenderedChunks(vec2 centerChunkPos);

	inline static Chunk* GetChunk(vec2 chunkPos);

	inline static vec2 GetChunkPosition(glm::vec3 pos);
	inline static vec2 GetChunkPosition(vec3 pos);

	inline static vec3 ToChunkPosition(glm::vec3 worldPos);
	inline static vec3 ToChunkPosition(vec3 worldPos);

	friend Chunk;
};

