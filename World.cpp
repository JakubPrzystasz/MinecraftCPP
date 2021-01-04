#include "World.h"

GLuint World::renderDistance = 4;

World* World::instance = nullptr;

GLuint World::chunkSize = 16;

std::unordered_map<vec2, Chunk*> World::Chunks;

std::vector<Chunk*> World::RenderedChunks;

std::vector<std::thread> World::Threads;
std::vector<std::pair<std::function<void(vec2)>, vec2>> World::Jobs;
std::mutex World::Mutex;
std::atomic<bool> World::Run = true;


int World::RoundInt(GLfloat x) {
	return static_cast<int>(trunc(x));
}

int World::RoundInt(GLuint x) {
	return static_cast<int>(x);
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
		chunk->model->BindData();
		chunk->model->shadingProgram->Use();
		chunk->model->shadingProgram->SetData("projection", camera.Projection);
		chunk->model->shadingProgram->SetData("view", camera.GetViewMatrix());
		chunk->model->shadingProgram->SetData("model", glm::mat4(1.0f));
		chunk->model->Draw();
	}
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return;
	auto blockInChunkPos = ToChunkPosition(pos);
	auto block = chunk->blocks.find(blockInChunkPos);
	if (block != chunk->blocks.end() && _block == BlockName::Air) {
		chunk->blocks.erase(block);
	}
	else {
		chunk->PutBlock(_block, ToChunkPosition(pos));
	}

	chunk->updateChunk = true;

	UpdateMesh(chunk->chunkPosition);
	if (blockInChunkPos.x == 0)
		UpdateMesh(chunk->chunkPosition + vec2(-1, 0));

	if (blockInChunkPos.x == chunkSize - 1)
		UpdateMesh(chunk->chunkPosition + vec2(1, 0));

	if (blockInChunkPos.z == 0)
		UpdateMesh(chunk->chunkPosition + vec2(0, -1));

	if (blockInChunkPos.z == chunkSize - 1)
		UpdateMesh(chunk->chunkPosition + vec2(0, 1));
}

Chunk* World::GenerateChunk(vec2 chunkPos,Model* newModel)
{
	Chunk* newChunk = new Chunk(chunkPos,newModel);

	float grassHeight;
	float dirtHeight;

	GLuint seed = (GLuint)time(NULL);

	for (GLuint x = 0; x < chunkSize; x++) {
		for (GLuint z = 0; z < chunkSize; z++) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, seed) * (-2) + 10;
			for (int y = 0; y < grassHeight; y++) {
				if (y < dirtHeight) {
					newChunk->PutBlock(BlockName::Stone, x, y, z);
					continue;
				}
				if (y + 1 < grassHeight) {
					newChunk->PutBlock(BlockName::Dirt, x, y, z);
					continue;
				}
				newChunk->PutBlock(BlockName::Grass, x, y, z);
			}

		}

	}

	newChunk->updateChunk = true;
	Chunks.emplace(chunkPos, newChunk);
	return newChunk;
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

void World::UpdateMesh(vec2 ChunkPosition)
{
	Chunk* chunk = GetChunk(ChunkPosition);
	if(chunk != nullptr)
		chunk->BuildMesh();
}

void World::RequestChunkGenerate(vec2 chunkPos)
{
	auto RS = ResourceManager::GetInstance();
	Model* newModel = new Model();
	newModel->Init();
	newModel->SetShadingProgram(RS->GetShadingProgram("block"));
	newModel->AddTexture("face", RS->GetTexture("Textures/terrain.png"));
	newModel->shadingProgram->Use();
	newModel->Textures["face"]->Bind();
	newModel->shadingProgram->SetData("blockTexture", newModel->Textures["face"]->GetId());

	GenerateChunk(chunkPos,newModel);
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	RenderedChunks.clear();
	vec2 tmp;
	Chunk* chunk;
	int _renderDistance = renderDistance;
	for (int y = -8; y <= 8; y++) {
		for (int x = -8; x <= 8; x++) {
			tmp = centerChunkPos + vec2(x, y);
			chunk = GetChunk(tmp);
			if (chunk != nullptr) 
				RenderedChunks.push_back(chunk);
			else
				RequestChunkGenerate(tmp);
		}
	}
	for (auto& chunk : RenderedChunks) {
		chunk->updateChunk = true;
		chunk->BuildMesh();
	}
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end())
		return tmp->second;
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
		RoundInt((worldPos.y)), \
		RoundInt(static_cast<int>(worldPos.z) % chunkSize));
}

vec3 World::ToChunkPosition(vec3 worldPos)
{
	return vec3(RoundInt(static_cast<int>(worldPos.x) % chunkSize), \
		RoundInt(worldPos.y), \
		RoundInt(static_cast<int>(worldPos.z) % chunkSize));
}
