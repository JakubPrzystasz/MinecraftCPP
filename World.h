#pragma once
#include "ResourceManager.h"
#include "Chunk.h"
#include "Model.h"
#include "Cube.h"
#include "Ray.h"
#include "Range.h"
#include "stb_perlin.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <memory>

class Chunk;
class Model;

class World
{
private:

	World() {};

	static GLuint renderDistance;

	static World* instance;

	static GLuint chunkSize;

	static GLuint chunkOffset;

	static std::unordered_map<vec2, Chunk*> Chunks;

	static std::vector<Chunk*> RenderedChunks;

	static int RoundInt(GLfloat x);

	static int RoundInt(GLuint x);

	static std::vector<std::thread> Threads;
	static std::vector<std::pair<vec2, Model*>> GenJobs;
	static std::vector<Chunk*> BuildJobs;
	static std::mutex GenMutex;
	static std::mutex BuildMutex;
	static std::atomic<GLuint> CountMeshes;
	static std::atomic<bool> Run;
	static void RunThreadsGen();
	static void RunThreadsBuild();

public:

	static bool worldGenerated;

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

	static void DrawChunks(Camera& camera);

	static void SetBlock(glm::vec3 pos, BlockName block);

	static void SetBlock(vec3 pos, BlockName block);

	static Chunk* GenerateChunk(vec2 chunkPos, Model* model);
	//Takes in chunk coords as parameter
	static BlockName GetBlock(Chunk* chunk, vec3 pos);
	//Takes world coords as parameter
	static BlockName GetBlock(glm::vec3 pos);
	
	static BlockName GetBlock(vec3 pos);

	static void BuildMesh(vec2 startPos);

	static void UpdateMesh(vec2 ChunkPoition);

	static void UpdateMesh(Chunk* chunk);

	static GLuint GenerateWorld(vec2 startPos);

	static void RequestChunkGenerate(vec2 chunkPos);

	static void SetRenderedChunks(vec2 centerChunkPos);

	static GLuint GetPlatformSize(GLuint distnace);

	static void SetRenderDistance(GLuint distance);

	static void SetChunkOffset(GLuint offset);

	static GLuint GetChunksCount();

	static GLuint GetMeshCount();

	static GLuint GetJobsCount();

	static GLuint GetRenderDistance();

	static GLuint GetChunkOffset();

	static Chunk* GetChunk(vec2 chunkPos);

	static vec2 GetChunkPosition(glm::vec3 pos);

	static vec2 GetChunkPosition(vec3 pos);

	static vec3 ToChunkPosition(glm::vec3 worldPos);

	static vec3 ToChunkPosition(vec3 worldPos);

	static vec3 ToChunkPositionTEST(vec3 worldPos);

	static void StartThreads();
	static void StopThreads();

	friend Chunk;
};

