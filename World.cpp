#include "World.h"

GLuint World::renderDistance = 1;

GLuint World::chunkSize = 16;

GLuint World::WorldSeed = 0;

World* World::instance = nullptr;

Camera* World::camera = nullptr;

std::unordered_map<vec2, Chunk*> World::Chunks;

std::unordered_map<vec2, Model*> World::RenderedChunks;

std::vector<std::thread> World::Threads;

std::list<Model*> World::Jobs;

std::mutex World::JobsMutex;

std::mutex World::ChunksMutex;

std::atomic<bool> World::Run = true;

std::atomic<GLuint> World::MeshCount = 0;


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
	auto rs = ResourceManager::GetInstance();
	auto texture = rs->GetTexture("Textures/terrain.png");
	auto shadingProgram = rs->GetShadingProgram("block");
	shadingProgram->Use();
	shadingProgram->SetData("model", glm::mat4(1.0f));
	shadingProgram->SetData("view", camera->GetViewMatrix());
	shadingProgram->SetData("projection", camera->Projection);

	renderDistance = distance;

	for (auto& chunk : RenderedChunks) {
		delete chunk.second;
	}
	RenderedChunks.clear();

	for (int i : range(0, distance)) {
		for (vec2 chunkPosition : rectangleCicrum(i)) {
			auto model = new Model();
			model->Init();
			model->SetShadingProgram(shadingProgram);
			model->AddTexture("blockTexture", texture);
			model->Textures["blockTexture"]->Bind();
			RenderedChunks.emplace(chunkPosition, model);
		}
	}

}

void World::SetCamera(Camera* camera)
{
	GetInstance()->camera = camera;
}

void World::DrawChunks(vec2 centerChunkPos)
{
	auto rs = ResourceManager::GetInstance();
	auto shadingProgram = rs->GetShadingProgram("block");
	shadingProgram->Use();
	shadingProgram->SetData("view", camera->GetViewMatrix());
	shadingProgram->SetData("projection", camera->Projection);

	for (auto& chunk : RenderedChunks) {
		if (chunk.second == nullptr)
			continue;

		if (chunk.second->chunk == nullptr)
			continue;

		std::unique_lock<std::mutex> lock(chunk.second->chunk->Mutex);

		//No mesh - request mesh build
		if (chunk.second->chunk->State == ChunkState::Generated) {
			lock.unlock();
			UpdateMesh(chunk.second);
			continue;
		}

		//Just draw chunk
		if (chunk.second->chunk->State == ChunkState::HasMesh) {
			chunk.second->BindData();
			chunk.second->Draw();
			continue;
		}

	}

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
	else {

		return;
	}


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

void World::GenerateChunk(Chunk* chunk)
{
	float grassHeight;
	float dirtHeight;

	for (GLuint x : range(0, chunkSize)) {
		for (GLuint z : range(0, chunkSize)) {
			grassHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunk->chunkPosition.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunk->chunkPosition.y + 2048)) / 16.f, 0, 0, 0, WorldSeed) * (-8) + 16;
			dirtHeight = stb_perlin_noise3_seed((float)(x + chunkSize * (chunk->chunkPosition.x + 2048)) / 16.f, 0.f, (float)(z + chunkSize * (chunk->chunkPosition.y + 2048)) / 16.f, 0, 0, 0, WorldSeed) * (-2) + 10;
			for (GLuint y : range(0, grassHeight)) {
				if (y < dirtHeight) {
					chunk->blocks.emplace(vec3(x, y, z), BlockName::Stone);
					continue;
				}
				if (y + 1 < grassHeight) {
					chunk->blocks.emplace(vec3(x, y, z), BlockName::Dirt);
					continue;
				}
				chunk->blocks.emplace(vec3(x, y, z), BlockName::Grass);
			}

		}
	}

	chunk->State = ChunkState::Generated;
}

BlockName World::GetBlock(Chunk* chunk, vec3 pos)
{
	if (chunk == nullptr)
		return BlockName::Air;


	auto tmp = chunk->blocks.find(pos);
	if (tmp != chunk->blocks.end()) {
		return tmp->second;
	}

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

void World::UpdateMesh(vec2 chunkPosition)
{
	Model* chunk = nullptr;
	for (auto& tmpChunk : RenderedChunks) {
		if (tmpChunk.second->chunk == nullptr)
			continue;

		std::unique_lock<std::mutex> lock(tmpChunk.second->chunk->Mutex);

		if (tmpChunk.second->chunk->chunkPosition == chunkPosition &&
			tmpChunk.second->chunk->State != ChunkState::NoData) {
			chunk = tmpChunk.second;
			break;
		}
	}

	if (chunk != nullptr)
		UpdateMesh(chunk);
}

void World::UpdateMesh(Model* model)
{
	JobsMutex.lock();
	Jobs.push_back(model);
	JobsMutex.unlock();
	return;
}


void World::RequestChunkGenerate(vec2 chunkPosition, Model* model)
{
	if (GetChunk(chunkPosition) != nullptr)
		return;

	auto chunk = new Chunk(chunkPosition);
	model->chunk = chunk;
	JobsMutex.lock();
	Jobs.push_back(model);
	JobsMutex.unlock();
	return;
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	vec2 position;

	for (auto& chunk : RenderedChunks) {
		if (chunk.second->chunk == nullptr)
			continue;
		//Put data to chunk
		chunk.second->chunk->vertices = std::move(chunk.second->vertices);
		chunk.second->chunk->indices = std::move(chunk.second->indices);
		chunk.second->chunk = nullptr;
	}

	for (auto& chunk : RenderedChunks) {
		position = centerChunkPos + chunk.first;

		auto tmpChunk = GetChunk(position);
		if (tmpChunk != nullptr) {
			chunk.second->chunk = tmpChunk;

			if (tmpChunk->State == ChunkState::Generated ||
				tmpChunk->State == ChunkState::BuildPending ||
				tmpChunk->State == ChunkState::HasMesh)
				UpdateMesh(position);

			if (tmpChunk->State == ChunkState::NoData) {
				RequestChunkGenerate(position, chunk.second);

				UpdateMesh(chunk.second->chunk->chunkPosition + vec2(-1, 0));

				UpdateMesh(chunk.second->chunk->chunkPosition + vec2(1, 0));

				UpdateMesh(chunk.second->chunk->chunkPosition + vec2(0, -1));

				UpdateMesh(chunk.second->chunk->chunkPosition + vec2(0, 1));
			}
		}
		else {
			RequestChunkGenerate(position, chunk.second);

			UpdateMesh(chunk.second->chunk->chunkPosition + vec2(-1, 0));

			UpdateMesh(chunk.second->chunk->chunkPosition + vec2(1, 0));

			UpdateMesh(chunk.second->chunk->chunkPosition + vec2(0, -1));

			UpdateMesh(chunk.second->chunk->chunkPosition + vec2(0, 1));

		}

		continue;
	}
}

Chunk* World::GetChunk(vec2 chunkPos)
{
	ChunksMutex.lock();
	auto tmp = Chunks.find(chunkPos);
	if (tmp != Chunks.end()) {
		ChunksMutex.unlock();
		return tmp->second;
	}
	ChunksMutex.unlock();
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

GLuint World::GetChunksCount()
{
	GLuint num = 0;
	ChunksMutex.lock();
	num = static_cast<GLuint>(Chunks.size());
	ChunksMutex.unlock();
	return num;
}

GLuint World::GetJobsCount()
{
	GLuint num = 0;
	JobsMutex.lock();
	num = static_cast<GLuint>(Jobs.size());
	JobsMutex.unlock();
	return num;
}

GLuint World::GetMeshCount()
{
	return MeshCount;
}

void World::StartThreads()
{
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

		JobsMutex.lock();
		if (Jobs.size() > 0) {
			auto job = Jobs.front();
			Jobs.pop_front();
			JobsMutex.unlock();

			if (job->chunk == nullptr)
				continue;

			std::unique_lock<std::mutex> lock(job->chunk->Mutex);
			if (job->chunk->State == ChunkState::NoData) {
				GenerateChunk(job->chunk);
				ChunksMutex.lock();
				Chunks.emplace(job->chunk->chunkPosition, job->chunk);
				ChunksMutex.unlock();

				lock.unlock();

				continue;
			}
			if (job->chunk->State == ChunkState::Generated) {
				job->chunk->BuildMesh(job);
				job->chunk->State = ChunkState::HasMesh;
				MeshCount++;
				continue;
			}
			if (job->chunk->State == ChunkState::HasMesh) {
				if (job->chunk->vertices.size() > 0) {
					job->vertices = std::move(job->chunk->vertices);
					job->indices = std::move(job->chunk->indices);
				}
				continue;
			}
		}
		else
		{
			JobsMutex.unlock();
			auto now = std::chrono::high_resolution_clock::now();
			do {
				std::this_thread::yield();
			} while (std::chrono::high_resolution_clock::now() < now + std::chrono::microseconds(25));
		}
	}
}