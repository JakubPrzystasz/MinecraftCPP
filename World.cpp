#include "World.h"

World* World::instance = nullptr;
GLuint World::chunkSize = 16;
GLuint World::renderDistance = 4;
std::unordered_map<vec2, Chunk*> World::Chunks;
std::vector<Chunk*> World::RenderedChunks;

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
	for (auto& chunk : RenderedChunks) {
		chunk->Draw(camera);
	}
}

void World::GenerateWorld()
{
	GLuint tmp = 2;
	for (int i = 0; i < tmp * tmp; i++) {
		int x = i / 2;
		int y = i % 2;
		GenerateChunk(vec2(x, y));
	}
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	auto block = chunk->blocks.find(ToChunkPosition(pos));
	if (block != chunk->blocks.end() && _block == BlockName::Air) {
		chunk->blocks.erase(block);
		chunk->ChunkUpdate();
	} else {
		chunk->PutBlock(_block, ToChunkPosition(pos));
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
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048 ) )/16.f, 0.f, (float)(z + chunkSize * (chunkPos.y+2048))/16.f , 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) /16.f, 0.f, (float)(z + chunkSize * (chunkPos.y+2048))/16.f, 0, 0, 0, seed) * (-2) + 10;

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
	return BlockName::Air;
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

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	RenderedChunks.clear();
	vec2 tmp; 
	Chunk* chunk;// GetChunk(centerChunkPos);
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

	GenerateChunk(chunkPos);
	return GetChunk(chunkPos);
}

vec2 World::GetChunkPosition(glm::vec3 pos) {
	auto ret = vec2((int)(pos.x / chunkSize), (int)(pos.z / chunkSize));
	return ret;
}

vec2 World::GetChunkPosition(vec3 pos) {
	auto ret = vec2((int)(pos.x / chunkSize), (int)(pos.z / chunkSize));
	return ret;
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