#include "World.h"

GLuint World::renderDistance = 3;

GLuint World::chunkOffset = 5;

World* World::instance = nullptr;

GLuint World::chunkSize = 16;

std::unordered_map<vec2, Chunk*> World::Chunks;

std::vector<Chunk*> World::RenderedChunks;

std::vector<std::thread> World::Threads;

std::vector<std::pair<vec2, Model*>> World::GenJobs;

std::vector<Chunk*> World::BuildJobs;

std::mutex World::GenMutex;

std::mutex World::BuildMutex;

std::atomic<GLuint> World::CountMeshes = 0;

std::atomic<bool> World::Run = true;

bool World::worldGenerated = false;


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

	for (auto* chunk : RenderedChunks) {
		BuildMutex.lock();
		if (chunk->model != nullptr) {
			if (chunk->model->vertices.size() < 1) {
				BuildMutex.unlock();
				UpdateMesh(chunk);
				continue;
			}
			chunk->model->BindData();
			chunk->model->shadingProgram->Use();
			chunk->model->shadingProgram->SetData("projection", camera.Projection);
			chunk->model->shadingProgram->SetData("view", camera.GetViewMatrix());
			chunk->model->shadingProgram->SetData("model", glm::mat4(1.0f));
			chunk->model->Draw();
		}
		else {
			RequestChunkGenerate(chunk->chunkPosition);
		}
		BuildMutex.unlock();
	}

}

GLuint World::GenerateWorld() {
	GLuint count = 0;
	int GenDist = static_cast<int>(renderDistance) + static_cast<int>(chunkOffset);
	for (int y = -GenDist; y <= GenDist; y++) {
		for (int x = -GenDist; x <= GenDist; x++) {
			RequestChunkGenerate(vec2(x, y));
			count++;
		}
	}
	return count;
}

void World::SetBlock(vec3 pos, BlockName _block) {
	SetBlock(glm::vec3(pos.x, pos.y, pos.z), _block);
}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return;
	auto blockInChunkPos = ToChunkPosition(pos);
	auto block = chunk->blocks.find(blockInChunkPos);
	if ((block != chunk->blocks.end())) {
		if (block->second == BlockName::TNT) {
			for (int x : range((rand() % 10 - 10), (rand() % 10 ))) {
				for (int y : range((rand() % 10 - 10), (rand() % 10 ))) {
					for (int z : range((rand()%10 - 10), (rand() % 10 ))) {

						auto tmpPos = vec3(x, y, z) + pos;
						chunk = GetChunk(GetChunkPosition(tmpPos));
						if (chunk == nullptr)
							continue;
						blockInChunkPos = ToChunkPosition(tmpPos);
						block = chunk->blocks.find(blockInChunkPos);
						if((block != chunk->blocks.end())) {
							chunk->updateChunk = true;
							UpdateMesh(chunk);
							chunk->blocks.erase(block);
						}
					}
				}
			}
			for (int z : range((rand() % 10 - 10), (rand() % 10))) {
				for (int y : range((rand() % 10 - 10), (rand() % 10))) {
					for (int x : range((rand() % 10 - 10), (rand() % 10))) {

						auto tmpPos = vec3(x, y, z) + pos;
						chunk = GetChunk(GetChunkPosition(tmpPos));
						if (chunk == nullptr)
							continue;
						blockInChunkPos = ToChunkPosition(tmpPos);
						block = chunk->blocks.find(blockInChunkPos);
						if ((block != chunk->blocks.end())) {
							chunk->updateChunk = true;
							UpdateMesh(chunk);
							chunk->blocks.erase(block);
						}
					}
				}
			}
		}else
			chunk->blocks.erase(block);
	}
	else if (_block != BlockName::Air)
		chunk->PutBlock(_block, ToChunkPosition(pos));
	else
		return;

	chunk->updateChunk = true;
	vec2 tmpPos;

	UpdateMesh(chunk);
	if (blockInChunkPos.x == 0) {
		tmpPos = vec2(chunk->chunkPosition + vec2(-1, 0));
		GetChunk(tmpPos)->updateChunk = true;
		UpdateMesh(tmpPos);
	}

	if (blockInChunkPos.x == chunkSize - 1) {
		tmpPos = vec2(chunk->chunkPosition + vec2(1, 0));
		GetChunk(tmpPos)->updateChunk = true;
		UpdateMesh(tmpPos);
	}

	if (blockInChunkPos.z == 0) {
		tmpPos = vec2(chunk->chunkPosition + vec2(0, -1));
		GetChunk(tmpPos)->updateChunk = true;
		UpdateMesh(tmpPos);
	}
	if (blockInChunkPos.z == chunkSize - 1) {
		tmpPos = vec2(chunk->chunkPosition + vec2(0, 1));
		GetChunk(tmpPos)->updateChunk = true;
		UpdateMesh(tmpPos);
	}
}

Chunk* World::GenerateChunk(vec2 chunkPos, Model* model)
{
	Chunk* newChunk = new Chunk(chunkPos, model);

	float grassHeight;
	float dirtHeight;
	float tree;

	GLuint seed = (GLuint)time(NULL);

	for (GLuint x : range(0, chunkSize - 1)) {
		for (GLuint z : range(0, chunkSize - 1)) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 16498)) / 20.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 16498)) / 18.f, 0, 0, 0, seed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 16498)) / 16.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 16498)) / 16.f, 0, 0, 0, seed) * (-2) + 10;
			tree = stb_perlin_noise3_seed((float)(x + chunkSize * (chunkPos.x + 16498)) / 100.f, 0.f, (float)(z + chunkSize * (chunkPos.y + 16498)) / 100.f, 0, 0, 0, seed) * (-8) + 2;
			for (GLuint y : range(0, static_cast<GLuint>(grassHeight))) {
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

			if (tree >3.7f && (rand() % 10000) > 9900) {
				auto treeMaxH = static_cast<GLuint>(grassHeight + 1 + (rand() % 4 + 2));
				for (int treeH : range(static_cast<GLuint>(grassHeight + 1), treeMaxH)) {
					newChunk->PutBlock(BlockName::Wood, x, treeH, z);
					if (treeH - (grassHeight + 1) > 1) {
						newChunk->PutBlock(BlockName::Leave, x + 1, treeH, z);
						newChunk->PutBlock(BlockName::Leave, x - 1, treeH, z);
						newChunk->PutBlock(BlockName::Leave, x, treeH, z + 1);
						newChunk->PutBlock(BlockName::Leave, x, treeH, z - 1);
						newChunk->PutBlock(BlockName::Leave, x - 1, treeH, z - 1);
						newChunk->PutBlock(BlockName::Leave, x + 1, treeH, z - 1);
						newChunk->PutBlock(BlockName::Leave, x + 1, treeH, z + 1);
						newChunk->PutBlock(BlockName::Leave, x - 1, treeH, z + 1);
					}
				}
				newChunk->PutBlock(BlockName::Leave, x, treeMaxH+1, z);
				newChunk->PutBlock(BlockName::Leave, x+1, treeMaxH+1, z);
				newChunk->PutBlock(BlockName::Leave, x-1, treeMaxH+1, z);
				newChunk->PutBlock(BlockName::Leave, x, treeMaxH+1, z+1);
				newChunk->PutBlock(BlockName::Leave, x, treeMaxH+1, z-1);

			}

		}

	}

	newChunk->updateChunk = true;
	GenMutex.lock();
	Chunks.emplace(chunkPos, newChunk);
	GenMutex.unlock();
	return newChunk;
}

BlockName World::GetBlock(Chunk* chunk, vec3 inChunkpos)
{
	if (chunk == nullptr)
		return BlockName::Air;

	auto block = chunk->blocks.find(inChunkpos);
	if ((block != chunk->blocks.end()))
		return block->second;
	else
		return BlockName::Air;
}

BlockName World::GetBlock(glm::vec3 pos)
{
	return GetBlock(vec3(pos));
}

BlockName World::GetBlock(vec3 pos)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	auto blockInChunkPos = ToChunkPosition(pos);
	return GetBlock(chunk, blockInChunkPos);
}

void World::UpdateMesh(vec2 ChunkPosition)
{
	Chunk* chunk = GetChunk(ChunkPosition);
	if (chunk == nullptr)
		return;
	BuildMutex.lock();
	BuildJobs.push_back(chunk);
	BuildMutex.unlock();
}

void World::UpdateMesh(Chunk* chunk)
{
	if (chunk == nullptr)
		return;
	BuildMutex.lock();
	BuildJobs.push_back(chunk);
	BuildMutex.unlock();
}

GLuint World::GetMeshCount() {
	GLuint ret = CountMeshes;
	return ret;
}

void World::RequestChunkGenerate(vec2 chunkPos)
{
	auto chunk = GetChunk(chunkPos);

	auto RS = ResourceManager::GetInstance();
	Model* newModel = new Model();
	newModel->Init();
	newModel->SetShadingProgram(RS->GetShadingProgram("block"));
	newModel->AddTexture("blockTexture", RS->GetTexture("Textures/terrain.png"));
	newModel->shadingProgram->Use();
	newModel->Textures["blockTexture"]->Bind();
	newModel->shadingProgram->SetData("blockTexture", newModel->Textures["blockTexture"]->GetId());

	if (chunk != nullptr && chunk->model == nullptr)
	{
		chunk->model = newModel;
		return;
	}

	GenMutex.lock();
	GenJobs.push_back(std::make_pair(chunkPos, newModel));
	GenMutex.unlock();
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	RenderedChunks.clear();
	vec2 tmp;
	Chunk* chunk;

	auto RS = ResourceManager::GetInstance();

	int _generateDistance = static_cast<int>(renderDistance) + static_cast<int>(chunkOffset);
	int _renderDistance = static_cast<int>(renderDistance);

	for (int y : range(-_generateDistance, _generateDistance)) {
		for (int x : range(-_generateDistance, _generateDistance)) {
			tmp = centerChunkPos + vec2(x, y);
			chunk = GetChunk(tmp);
			if (chunk != nullptr)
			{
				if (y > -_renderDistance && y < _renderDistance &&
					x > -_renderDistance && x < _renderDistance) {
					RenderedChunks.push_back(chunk);
				}
			}
			else
				RequestChunkGenerate(tmp);
		}
	}
}

void World::BuildMesh() {
	Chunk* chunk;
	int _renderDistance = static_cast<int>(renderDistance);
	for (int y : range(-_renderDistance,_renderDistance)) {
		for (int x : range(-_renderDistance,_renderDistance)) {
			chunk = GetChunk(vec2(x, y));
			if (chunk == nullptr) {
				RequestChunkGenerate(vec2(x, y));
				while ((GetChunk(vec2(x, y)) == nullptr)) { ; }
				chunk = GetChunk(vec2(x, y));
			}
			GenMutex.lock();
			if (chunk->model == nullptr) {
				auto RS = ResourceManager::GetInstance();
				Model* newModel = new Model();
				newModel->Init();
				newModel->SetShadingProgram(RS->GetShadingProgram("block"));
				newModel->AddTexture("blockTexture", RS->GetTexture("Textures/terrain.png"));
				newModel->shadingProgram->Use();
				newModel->Textures["blockTexture"]->Bind();
				newModel->shadingProgram->SetData("blockTexture", newModel->Textures["blockTexture"]->GetId());
				chunk->model = newModel;
			}
			if (chunk->model->vertices.size() < 1) {
				GenMutex.unlock();
				UpdateMesh(chunk);
			}
			else
				GenMutex.unlock();
		}
	}
}

void World::SetRenderDistance(GLuint distance) {
	renderDistance = distance;
}

GLuint World::GetPlatformSize(GLuint distance) {
	GLuint d = 1;
	for (GLuint i = 1; i <= distance; i++) {
		d += 2;
	}
	return d * d;
}


void World::SetChunkOffset(GLuint offset) {
	chunkOffset = offset;
}


Chunk* World::GetChunk(vec2 chunkPos)
{
	GenMutex.lock();
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end()) {
		GenMutex.unlock();
		return tmp->second;
	}
	GenMutex.unlock();
	return nullptr;
}

vec2 World::GetChunkPosition(glm::vec3 pos) {
	return GetChunkPosition(vec3(pos));
}

vec2 World::GetChunkPosition(vec3 pos) {
	int x, z;
	if (pos.x >= 0)
		x = static_cast<int>(std::floor(pos.x / chunkSize));
	else
		x = static_cast<int>(std::floor(pos.x / chunkSize));

	if (pos.z >= 0)
		z = static_cast<int>(std::floor(pos.z / chunkSize));
	else
		z = static_cast<int>(std::floor(pos.z / chunkSize));

	return vec2(x, z);
}

vec3 World::ToChunkPosition(glm::vec3 worldPos)
{
	return ToChunkPosition(vec3(worldPos));
}

vec3 World::ToChunkPosition(vec3 worldPos)
{
	GLfloat x, y, z;

	y = static_cast<GLfloat>((static_cast<int>(std::floor(worldPos.y))));

	x = worldPos.x >= 0 ? worldPos.x : chunkSize - (-std::floor(worldPos.x));
	x = static_cast<GLfloat>((static_cast<int>(x) % chunkSize));

	z = worldPos.z >= 0 ? worldPos.z : chunkSize - (-std::floor(worldPos.z));
	z = static_cast<GLfloat>((static_cast<int>(z) % chunkSize));

	return vec3(x, y, z);
}

GLuint World::GetRenderDistance() {
	return renderDistance;
}

GLuint World::GetChunkOffset() {
	return chunkOffset;
}

GLuint World::GetChunksCount() {
	GenMutex.lock();
	auto ret = Chunks.size();
	GenMutex.unlock();
	return ret;
}

GLuint World::GetJobsCount() {
	GenMutex.lock();
	auto ret = GenJobs.size();
	GenMutex.unlock();
	BuildMutex.lock();
	ret += BuildJobs.size();
	BuildMutex.unlock();
	return ret;
}

void World::StartThreads()
{
	Threads.push_back(std::thread(RunThreadsGen));
	Threads.push_back(std::thread(RunThreadsBuild));
}

void World::StopThreads()
{
	Run = false;
	for (auto& thread : Threads)
		thread.join();
}

void World::RunThreadsGen() {
	while (Run) {
		GenMutex.lock();
		if (GenJobs.size() > 0) {
			auto tmp = GenJobs.back();
			GenJobs.pop_back();
			GenMutex.unlock();
			GenerateChunk(tmp.first, tmp.second);
			GenerateChunk(tmp.first + vec2(1, 0), nullptr);
			GenerateChunk(tmp.first + vec2(-1, 0), nullptr);
			GenerateChunk(tmp.first + vec2(0, 1), nullptr);
			GenerateChunk(tmp.first + vec2(0, -1), nullptr);
			continue;
		}
		GenMutex.unlock();
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}
}

void World::RunThreadsBuild() {
	while (Run) {
		BuildMutex.lock();
		if (BuildJobs.size() > 0) {
			auto tmp = BuildJobs.back();
			BuildJobs.pop_back();
			BuildMutex.unlock();
			if (tmp->BuildMesh())
				CountMeshes++;
			continue;
		}
		BuildMutex.unlock();
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}
}