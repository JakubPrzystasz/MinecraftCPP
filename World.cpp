#include "World.h"

World* World::instance = nullptr;
GLuint World::chunkSize = 16;
GLuint World::renderDistance = 4;
std::unordered_map<vec2, Chunk*> World::Chunks;
std::vector<Chunk*> World::RenderedChunks;

std::vector<std::thread> World::Threads;
std::mutex World::Mutex;
std::atomic<bool> World::Run = true;

void World::StopThreads()
{
	Run = false;

	for (auto& thread : Threads)
		thread.join();
}

int World::RoundInt(GLfloat x) {
	return static_cast<int>(trunc(x));
}

int World::RoundInt(GLuint x) {
	return static_cast<int>(trunc(x));
}

int World::RoundInt(int x) {
	return static_cast<int>(trunc(x));
}

World* World::GetInstance()
{
	if (instance == nullptr)
		instance = new World();
	return instance;
}

void World::SetChunkSize(GLuint chunkSize)
{
	GetInstance()->chunkSize = chunkSize;
}

void World::DrawChunks(Camera& camera)
{
	for (auto& chunk : RenderedChunks) {
		if (chunk != nullptr)
			chunk->Draw(camera);
	}
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return;
	auto block = chunk->blocks.find(ToChunkPosition(pos));
	if (block != chunk->blocks.end() && _block == BlockName::Air) {
		chunk->blocks.erase(block);
	}
	else {
		chunk->PutBlock(_block, ToChunkPosition(pos));
	}

	RequestChunkUpdate(chunk->chunkPosition);
	if (chunk->chunkPosition.x == 0)
		RequestChunkUpdate(chunk->chunkPosition + vec2(-1, 0));

	if (chunk->chunkPosition.x == chunkSize - 1)
		RequestChunkUpdate(chunk->chunkPosition + vec2(1, 0));

	if (chunk->chunkPosition.y == 0)
		RequestChunkUpdate(chunk->chunkPosition + vec2(0, -1));

	if (chunk->chunkPosition.y == chunkSize - 1)
		RequestChunkUpdate(chunk->chunkPosition + vec2(0, 1));
}

void World::GenerateChunk(Chunk* chunk)
{
	float grassHeight;
	float dirtHeight;

	int seed = time(NULL);

<<<<<<< HEAD
	for (GLuint x = 0; x < chunkSize; x++) {
		for (GLuint z = 0; z < chunkSize; z++) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunk->chunkPosition.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunk->chunkPosition.y + 2048)) / 16.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunk->chunkPosition.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunk->chunkPosition.y + 2048)) / 16.f, 0, 0, 0, seed) * (-2) + 10;
			//if (x == 0 && z == 0)
			//{
			//	for (int y = 0; y < 30; y++)
			//		tmp->PutBlock(BlockName::Cobble, x, y, z);
			//	continue;
			//}
			for (GLuint y = 0; y < grassHeight; y++) {
=======
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-2) + 10;
			/*if (x == 0 && z == 0)
			{
				for (int y = 0; y < 100; y++)
					tmp->PutBlock(BlockName::Stone, x, y, z);
				continue;
			}*/
			for (int y = 0; y < grassHeight; y++) {
>>>>>>> parent of 439d9a6... Fix chunk mesh generation
				if (y < dirtHeight) {
					chunk->PutBlock(BlockName::Stone, x, y, z);
					continue;
				}
				if (y + 1 < grassHeight) {
					chunk->PutBlock(BlockName::Dirt, x, y, z);
					continue;
				}
				chunk->PutBlock(BlockName::Grass, x, y, z);
			}

		}

	}
	Mutex.lock();
	Chunks.emplace(chunk->chunkPosition, chunk);
	Mutex.unlock();
}

BlockName World::GetBlock(Chunk* chunk, vec3 pos)
{
	if (chunk == nullptr)
		return BlockName::Air;

	auto tmp = chunk->blocks.find(pos);
	if (tmp != chunk->blocks.end())
		return tmp->second;
	return BlockName::Air;
}

BlockName World::GetBlock(glm::vec3 pos)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return BlockName::Air;
	auto _pos = ToChunkPosition(pos);
	return GetBlock(chunk, _pos);
}

BlockName World::GetBlock(vec3 pos)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return BlockName::Air;
	auto _pos = ToChunkPosition(pos);
	return GetBlock(chunk, _pos);
}

void World::RequestChunkUpdate(vec2 chunkPos)
{
	GetChunk(chunkPos)->updateChunk = true;
	GetChunk(chunkPos)->ChunkUpdate();
}

void World::RequestChunkGen(vec2 chunkPos)
{
	auto CHUNK = new Chunk();
	CHUNK->chunkPosition = chunkPos;
	CHUNK->chunkSize = chunkSize;
	CHUNK->updateChunk = true;
	Threads.emplace_back([&]() { GenerateChunk(CHUNK); });
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	RenderedChunks.clear();
	vec2 tmp;
	Chunk* chunk;
	for (int y = (-1) * renderDistance; y < renderDistance+1; y++) {
		for (int x = (-1) * renderDistance; x < renderDistance + 1; x++) {
			tmp = centerChunkPos + vec2(x, y);
			chunk = GetChunk(tmp);
			if(chunk == nullptr)
				RequestChunkGen(tmp);
			RenderedChunks.push_back(chunk);
		}
	}
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	Mutex.lock();
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end()) {
		Mutex.unlock();
		return tmp->second;
	}
	else
		Mutex.unlock();
	return nullptr;
}

vec2 World::GetChunkPosition(glm::vec3 pos) {
	auto ret = vec2(RoundInt(pos.x / chunkSize), RoundInt(pos.z / chunkSize));
	if (pos.x < 0)
		ret.x -= 1;
	if (pos.z < 0)
		ret.y -= 1;
	return ret;
}

vec2 World::GetChunkPosition(vec3 pos) {
	auto ret = vec2(RoundInt(pos.x / chunkSize), RoundInt(pos.z / chunkSize));
	if (pos.x < 0)
		ret.x -= 1;
	if (pos.z < 0)
		ret.y -= 1;
	return ret;
}

vec3 World::ToChunkPosition(glm::vec3 worldPos)
{
	if (worldPos.x < 0)
		worldPos.x -= 1;
	if (worldPos.z < 0)
		worldPos.z -= 1;
	return vec3(RoundInt(static_cast<int>(worldPos.x) % chunkSize), \
		RoundInt(static_cast<int>(worldPos.y)), \
		RoundInt(static_cast<int>(worldPos.z) % chunkSize));
}

vec3 World::ToChunkPosition(vec3 worldPos)
{
	return vec3(RoundInt(static_cast<int>(worldPos.x) % chunkSize), \
		RoundInt(static_cast<int>(worldPos.y)), \
		RoundInt(static_cast<int>(worldPos.z) % chunkSize));
}