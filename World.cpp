#include "World.h"

World* World::instance = nullptr;
GLuint World::chunkSize = 16;
std::unordered_map<vec2, Chunk*> World::Chunks;

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
	for (auto &chunk:Chunks){
		chunk.second->Draw(camera);
	}
}

void World::GenerateWorld()
{
	for (int i = 0; i < chunkSize* chunkSize; i++) {
		float xpos, ypos;
		xpos = i / chunkSize;
		ypos = i % chunkSize;
		Chunk *tmp = new Chunk();
		tmp->Init();
		tmp->chunkPosition = vec2(xpos,ypos);
		tmp->chunkSize = chunkSize;

		float grassHeight;
		float dirtHeight;

		int seed = time(NULL);
		for (int c = 0; c < 2; c++)
		{
			for (int x = 0; x < chunkSize; x++) {
				for (int z = 0; z < chunkSize; z++) {
					grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * xpos) / 16.f, 0.f, (float)(z + chunkSize * ypos) / 16.f, 0, 0, 0, seed) * (-8) + 16;
					dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * xpos) / 32.f, 0.f, (float)(z + chunkSize * ypos) / 32, 0, 0, 0, seed) * (-2) + 10;

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
		}
		Chunks.emplace(vec2(xpos, ypos), std::move(tmp));
	}
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunkPos = GetChunkPosition(pos);
	if (Chunks.find(chunkPos) != Chunks.end()) {
		auto chunk = Chunks.at(chunkPos);
		auto block = chunk->blocks.find(ToChunkPosition(pos));
		if (block != chunk->blocks.end()) {
			chunk->blocks.erase(block);
			chunk->ChunkUpdate();
		}
	}
}

//BlockPos* World::GetBlock(glm::vec3 pos)
//{
	/*glm::vec2 chunkPos = glm::vec2((int)pos.x / chunkSize, (int)pos.z / chunkSize);
	for (int i = 0; i < Chunks.size(); i++) {
		if (Chunks[i].Position == chunkPos) {
			glm::vec3 blockPos = glm::vec3(0);
			blockPos.x = (int)(pos.x - chunkPos.x * chunkSize);
			blockPos.z = (int)(pos.z - chunkPos.y * chunkSize);
			blockPos.y = (int)pos.y;
			for (int x = 0; x < Chunks[i].chunk.blocks.size(); x++) {
				if (Chunks[i].chunk.blocks[x].pos == blockPos) {
					return &Chunks[i].chunk.blocks[x];
				}
			}
		}
	}
	return nullptr;*/
//	return nullptr;
//}

bool World::IsBlock(glm::vec3 pos)
{
	return false;
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	return (Chunk*)Chunks.at(chunkPos);
}

vec2 World::GetChunkPosition(glm::vec3 pos) {
	return vec2((int)((int)pos.x) / chunkSize, (int)((int)pos.z) / chunkSize);
}

vec3 World::ToChunkPosition(glm::vec3 worldPos)
{
	return vec3((int)ceil(((int)worldPos.x)%chunkSize),(int)floor(worldPos.y),(int)ceil(((int)worldPos.z)%chunkSize));
}
