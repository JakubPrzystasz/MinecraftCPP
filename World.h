#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "stb_perlin.h"
#include <unordered_map>


class World
{
private:
	
	World() {};

	static World* instance;

	static GLuint chunkSize;
	
	static std::unordered_map<vec2,Chunk*> Chunks;

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

	static void GenerateWorld();

	static void SetBlock(glm::vec3 pos, BlockName block);

	static bool IsBlock(glm::vec3 pos);

	inline static Chunk* GetChunk(vec2 chunkPos);

	inline static vec2 GetChunkPosition(glm::vec3 pos);

	static vec3 ToChunkPosition(glm::vec3 worldPos);
};

