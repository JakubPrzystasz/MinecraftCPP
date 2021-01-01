#include "World.h"

World* World::instance = nullptr;
GLuint World::chunkSize = 16;
std::unordered_map<vec2, Chunk*> World::Chunks;

GLfloat World::RoundPos(GLfloat x)
{
	if (x >= 0)
		x = floor(x);
	else
		x = ceil(x);
	return x;
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
	for (auto& chunk : Chunks) {
		chunk.second->Draw(camera);
	}
}

void World::GenerateWorld()
{
	for (int i = 0; i < 16 * 16; i++) {
		int x = i / 16;
		int y = i % 16;
		GenerateChunk(vec2(x, y));
	}
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	auto block = chunk->blocks.find(ToChunkPosition(pos));
	if (block != chunk->blocks.end()) {
		chunk->blocks.erase(block);
		chunk->ChunkUpdate();
	}

}

void World::GenerateChunk(vec2 chunkPos)
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
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * chunkPos.x) / 16.f, 0.f, (float)(z + chunkSize * chunkPos.y) / 16.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * chunkPos.x) / 32.f, 0.f, (float)(z + chunkSize * chunkPos.y) / 32, 0, 0, 0, seed) * (-2) + 10;

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

	Chunks.emplace(chunkPos, std::move(tmp));
}

BlockName World::GetBlock(Chunk* chunk, vec3 pos)
{
	if (chunk == nullptr)
		return BlockName::Air;

	auto tmp = chunk->blocks.find(pos);
	if (tmp != chunk->blocks.end())
		return tmp->second;
}

BlockName World::GetBlock(glm::vec3 pos)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	auto _pos = ToChunkPosition(pos);
	return GetBlock(chunk, _pos);
}

BlockName World::GetBlock(vec3 pos)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	auto _pos = ToChunkPosition(pos);
	return GetBlock(chunk, _pos);
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end())
		return tmp->second;

	GenerateChunk(chunkPos);
	return GetChunk(chunkPos);
}

vec2 World::GetChunkPosition(glm::vec3 pos) {
	return vec2((int)((int)pos.x) / chunkSize, (int)((int)pos.z) / chunkSize);
}

vec2 World::GetChunkPosition(vec3 pos) {
		return vec2((int)((int)pos.x) / chunkSize, (int)((int)pos.z) / chunkSize);
}

vec3 World::ToChunkPosition(glm::vec3 worldPos)
{
	return vec3(RoundPos((int)worldPos.x % chunkSize), \
		RoundPos((int)worldPos.y), \
		RoundPos((int)worldPos.z % chunkSize));
}

vec3 World::ToChunkPosition(vec3 worldPos)
{
	return vec3(RoundPos((int)worldPos.x % chunkSize), \
		RoundPos((int)worldPos.y), \
		RoundPos((int)worldPos.z % chunkSize));
}