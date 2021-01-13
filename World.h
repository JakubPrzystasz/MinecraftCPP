#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "Model.h"
#include "Cube.h"
#include "Range.h"
#include "stb_perlin.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>
#include <list>
#include <memory>

class Chunk;
class Model;

class World
{
private:

	World() {};

	inline static int RoundInt(GLfloat x);

	inline static int RoundInt(GLuint x);

	static GLuint renderDistance;

	static World* instance;

	static GLuint chunkSize;

	static std::unordered_map<vec2, Chunk*> Chunks;

	static std::unordered_map<vec2, Model*> RenderedChunks;

	static Camera* camera;

	static std::vector<std::thread> Threads;

	static std::list<Model*> Jobs;

	static std::mutex JobsMutex;

	static std::mutex ChunksMutex;

	static std::atomic<bool> Run;

	static std::atomic<GLuint> MeshCount;

	static void RunThreads();

	static GLuint WorldSeed;

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

	static void SetRenderDistance(GLuint distance);

	static void SetCamera(Camera* camera);

	static void DrawChunks(vec2 centerChunkPos);

	static void SetBlock(glm::vec3 pos, BlockName block);

	static void GenerateChunk(Chunk* chunk);

	//Takes in chunk coords as parameter
	static BlockName GetBlock(Chunk* chunk, vec3 pos);

	//Takes world coords as parameter
	static BlockName GetBlock(glm::vec3 pos);

	static BlockName GetBlock(vec3 pos);

	static void UpdateMesh(vec2 chunkPosition);

	static void UpdateMesh(Model* model);

	static void RequestChunkGenerate(vec2 chunkPos, Model* model);

	static void SetRenderedChunks(vec2 centerChunkPos);

	inline static Chunk* GetChunk(vec2 chunkPos);

	inline static vec2 GetChunkPosition(glm::vec3 pos);

	inline static vec2 GetChunkPosition(vec3 pos);

	inline static vec3 ToChunkPosition(glm::vec3 worldPos);

	inline static vec3 ToChunkPosition(vec3 worldPos);
	
	static GLuint GetChunksCount();

	static GLuint GetJobsCount();

	static GLuint GetMeshCount();

	static void StartThreads();

	static void StopThreads();

	friend Chunk;
};

