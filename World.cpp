#include "World.h"

GLuint World::renderDistance = 3;

GLuint World::chunkSize = 16;

GLuint World::WorldSeed = 0;

World* World::instance = nullptr;

Camera* World::camera = nullptr;

std::unordered_map<vec2, Chunk*> World::Chunks;

std::unique_ptr<VisibleChunk[]> World::RenderedChunks = nullptr;

size_t World::RenderedChunksSize = 0;

std::vector<std::thread> World::Threads;

std::queue<std::pair<vec2, Model*>> World::GenJobs;

std::atomic<bool> World::Run = true;


int World::RoundInt(GLfloat x) {
	return static_cast<int>(trunc(x));
}

int World::RoundInt(GLuint x) {
	return static_cast<int>(x);
}

World* World::GetInstance()
{
	if (instance == nullptr) {
		instance = new World();
		WorldSeed = static_cast<GLuint>(time(NULL));
	}
	return instance;
}

void World::SetChunkSize(GLuint chunkSize)
{
	GetInstance()->chunkSize = chunkSize;
}

void World::SetRenderDistance(GLuint distance)
{

}

void World::SetCamera(Camera* camera)
{
	GetInstance()->camera = camera;
}

void World::DrawChunks(vec2 centerChunkPos)
{
	//BuildMutex.lock();
	//if (RenderedChunksSize < 1) {
	//	BuildMutex.unlock();
	//	SetRenderedChunks(centerChunkPos);
	//	return;
	//}

	//for (GLuint i = 0; i < RenderedChunksSize; i++) {
	//	//RenderedChunks[i].model->BindData();
	//	//RenderedChunks[i].model->Draw();
	//}
	//BuildMutex.unlock();
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return;
	auto blockInChunkPos = ToChunkPosition(pos);
	auto block = chunk->blocks.find(blockInChunkPos);
	if ((block != chunk->blocks.end()))
		chunk->blocks.erase(block);
	else if (_block != BlockName::Air)
		chunk->PutBlock(_block, ToChunkPosition(pos));
	else
		return;

	UpdateMesh(chunk);
	if (blockInChunkPos.x == 0)
		UpdateMesh(chunk->chunkPosition + vec2(-1, 0));

	if (blockInChunkPos.x == chunkSize - 1)
		UpdateMesh(chunk->chunkPosition + vec2(1, 0));

	if (blockInChunkPos.z == 0)
		UpdateMesh(chunk->chunkPosition + vec2(0, -1));

	if (blockInChunkPos.z == chunkSize - 1)
		UpdateMesh(chunk->chunkPosition + vec2(0, 1));
}

Chunk* World::GenerateChunk(vec2 chunkPos)
{
	Chunk* newChunk = new Chunk(chunkPos);

	float grassHeight;
	float dirtHeight;

	for (GLuint x : range(0,chunkSize)) {
		for (GLuint z : range(0, chunkSize)) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, WorldSeed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 2048)) / 16.f, 0, 0, 0, WorldSeed) * (-2) + 10;
			for (GLuint y : range(0, grassHeight)) {
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

	//GenMutex.lock();
	//Chunks.emplace(chunkPos, newChunk);
	//GenMutex.unlock();
	//return newChunk;
	return nullptr;
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
	if (chunk == nullptr)
		return;
	//BuildMutex.lock();
	//BuildJobs.emplace(chunk);
	//BuildMutex.unlock();
}

void World::UpdateMesh(Chunk* chunk)
{
	if (chunk == nullptr)
		return;
	//BuildMutex.lock();
	//BuildJobs.emplace(chunk);
	//BuildMutex.unlock();
}

void World::RequestChunkGenerate(vec2 chunkPos)
{
	//GenMutex.lock();
	////GenJobs.emplace(chunkPos, newModel);
	//GenMutex.unlock();
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	auto rs = ResourceManager::GetInstance();

	//Chunk* lastRendered = RenderedChunks;
	size_t lastRenderedSize = RenderedChunksSize;

	Model* newModel = new Model();
	newModel->Init();
	newModel->SetShadingProgram(rs->GetShadingProgram("block"));
	newModel->AddTexture("blockTexture", rs->GetTexture("Textures/terrain.png"));
	newModel->shadingProgram->Use();
	newModel->shadingProgram->SetData("model", glm::mat4(1.0f));
	newModel->Textures["blockTexture"]->Bind();

	//vec2 tmp;
	//Chunk* chunk;
	//int _renderDistance = renderDistance;
	//for (int y = -1*_renderDistance; y <= _renderDistance; y++) {
	//	for (int x = -1*_renderDistance; x <= _renderDistance; x++) {
	//		tmp = centerChunkPos + vec2(x, y);
	//		chunk = GetChunk(tmp);
	//		if (chunk != nullptr) 
	//			RenderedChunks.push_back(chunk);
	//		else
	//			RequestChunkGenerate(tmp);
	//	}
	//}

}

Chunk* World::GetChunk(vec2 chunkPos)
{
	//GenMutex.lock();
	//auto tmp = Chunks.find(chunkPos);
	//if (tmp != Chunks.end()) {
	//	GenMutex.unlock();
	//	return tmp->second;
	//}
	//GenMutex.unlock();
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

void World::StartThreads()
{
	for (int i = 0; i < 1; i++)
		Threads.push_back(std::thread(RunThreads));
}

void World::StopThreads()
{
	Run = false;
	for (auto& thread : Threads)
		thread.join();
}

void World::RunThreads() {
	while (Run) {
		//GenMutex.lock();
		//if (GenJobs.size() > 0) {
		//	auto tmp = GenJobs.front();
		//	GenJobs.pop();
		//	GenMutex.unlock();
		//	//UpdateMesh(GenerateChunk(tmp.first, tmp.second));
		//	continue;
		//}
		//GenMutex.unlock();
		//using namespace std::chrono_literals;
		//std::this_thread::sleep_for(1ms);
	}
}