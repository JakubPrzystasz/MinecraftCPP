#include "World.h"

World* World::instance = nullptr;
GLuint World::chunkSize = 16;
GLuint World::renderDistance = 4;
std::unordered_map<vec2, Chunk*> World::Chunks;
std::vector<Chunk*> World::RenderedChunks;
std::vector<vec2> World::RequestedGenChunks;
std::vector<vec2> World::RequestedUpdateChunks;
std::mutex World::generateMutex;
std::mutex World::updateMutex;
std::thread World::generator;

int World::RoundInt(GLfloat x) {
	return static_cast<int>(trunc(x));
}

void World::generatorFunc(World* world)
{
	while (true) {
		if (world->RequestedGenChunks.size() > 0) {
			auto chunk = world->RequestedGenChunks.back();
			world->GenerateChunk(chunk)->ChunkUpdate();
			world->RequestedGenChunks.pop_back();
			std::cout << "Generated chunk at: " << chunk.x << "  " << chunk.y << std::endl;
		}
	}
}

World* World::GetInstance()
{
	if (instance == nullptr)
		instance = new World();
	return instance;
}

void World::StartThreads()
{
	generator = std::thread(generatorFunc, instance);
}

void World::SetChunkSize(GLuint chunkSize)
{
	GetInstance()->chunkSize = chunkSize;
}

void World::DrawChunks(Camera& camera)
{
	for (auto& chunk : RenderedChunks) {
		if(chunk != nullptr)
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
	} else {
		chunk->PutBlock(_block, ToChunkPosition(pos));
	}

	RequestChunkUpdate(chunk->chunkPosition);
	if (chunk->chunkPosition.x == 0)
		RequestChunkUpdate(chunk->chunkPosition + vec2(-1, 0));
	
	if (chunk->chunkPosition.x == chunkSize-1)
		RequestChunkUpdate(chunk->chunkPosition + vec2(1, 0));

	if (chunk->chunkPosition.y == 0)
		RequestChunkUpdate(chunk->chunkPosition + vec2(0, -1));

	if (chunk->chunkPosition.y == chunkSize - 1)
		RequestChunkUpdate(chunk->chunkPosition + vec2(0, 1));
}

Chunk* World::GenerateChunk(vec2 chunkPos)
{
	Chunk* tmp = new Chunk();
	tmp->Init();
	tmp->chunkPosition = chunkPos;
	tmp->chunkSize = chunkSize;

	float grassHeight;
	float dirtHeight;

	int seed = time(NULL);

	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-2) + 10;
			//if (x == 0 && z == 0)
			//{
			//	for (int y = 0; y < 30; y++)
			//		tmp->PutBlock(BlockName::Cobble, x, y, z);
			//	continue;
			//}
			for (int y = 0; y < grassHeight; y++) {
				if (y < dirtHeight) {
					tmp->PutBlock(BlockName::Stone, x, y, z);
					continue;
				}
				if (y + 1 < grassHeight) {
					tmp->PutBlock(BlockName::Dirt, x, y, z);
					continue;
				}
				tmp->PutBlock(BlockName::Grass, x, y, z);
			}

		}

	}

	Chunks.emplace(chunkPos, tmp);
	return tmp;
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
	GetChunk(chunkPos)->ChunkUpdate();
}

void World::RequestChunkGen(vec2 chunkPos)
{
	generateMutex.lock();
	RequestedGenChunks.push_back(chunkPos);
	generateMutex.unlock();
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	RenderedChunks.clear();

	vec2 tmp; 
	Chunk* chunk;
	for (int y = -4; y < 5; y++) {
		for (int x = -4; x < 5; x++) {
			tmp = centerChunkPos + vec2(x, y);
			chunk = GetChunk(tmp);
			RenderedChunks.push_back(chunk);
		}
	}
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end())
		return tmp->second;

	RequestChunkGen(chunkPos);
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