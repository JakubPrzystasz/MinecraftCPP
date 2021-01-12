#include "World.h"

GLuint World::renderDistance = 1;

GLuint World::chunkSize = 16;

GLuint World::WorldSeed = 0;

World* World::instance = nullptr;

Camera* World::camera = nullptr;

std::unordered_map<vec2, Chunk*> World::Chunks;

std::unordered_map<vec2, std::pair<Model*, Chunk*>> World::RenderedChunks;

std::vector<std::thread> World::Threads;

std::list<std::pair<Model*, Chunk*>> World::Jobs;

std::mutex World::JobsMutex;

std::mutex World::ChunksMutex;

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
	auto rs = ResourceManager::GetInstance();
	auto texture = rs->GetTexture("Textures/terrain.png");
	auto shadingProgram = rs->GetShadingProgram("block");
	shadingProgram->Use();
	shadingProgram->SetData("model", glm::mat4(1.0f));
	shadingProgram->SetData("view", camera->GetViewMatrix());
	shadingProgram->SetData("projection", camera->Projection);

	renderDistance = distance;

	for (auto& chunk : RenderedChunks) {
		delete chunk.second.first;
	}
	RenderedChunks.clear();

	for (int i : range(0, distance)) {
		for (vec2 chunkPosition : rectangleCicrum(i)) {
			auto model = new Model();
			model->Init();
			model->SetShadingProgram(shadingProgram);
			model->AddTexture("blockTexture", texture);
			model->Textures["blockTexture"]->Bind();
			RenderedChunks.emplace(chunkPosition, std::make_pair(model, nullptr));
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
		//May request to generate chunk ? 
		if (chunk.second.second == nullptr)
			continue;

		chunk.second.second->Mutex.lock();

	//	std::cout << (int)(chunk.second.second->State.load()) << std::endl;

		//No mesh - request mesh build
		if (chunk.second.second->State == ChunkState::Generated) {
			auto pair = std::make_pair(chunk.second.first, chunk.second.second);
			chunk.second.second->Mutex.unlock();
			UpdateMesh(chunk.second.second);
			continue;
		}

		//Just draw chunk
		if (chunk.second.second->State == ChunkState::HasMesh) {
			chunk.second.first->BindData();
			chunk.second.first->Draw();
			chunk.second.second->Mutex.unlock();
			continue;
		}

		//Chunk has a new mesh, lets take it to the model
		if (chunk.second.second->State == ChunkState::BuildPending) {
			auto pair = std::make_pair(chunk.second.first, chunk.second.second);
			chunk.second.second->Mutex.unlock();
			JobsMutex.lock();
			Jobs.push_back(pair);
			JobsMutex.unlock();
			continue;
		}

		chunk.second.second->Mutex.unlock();
	}

}

void World::SetBlock(glm::vec3 pos, BlockName _block)
{
	auto chunk = GetChunk(GetChunkPosition(pos));
	if (chunk == nullptr)
		return;
	auto blockInChunkPos = ToChunkPosition(pos);
	chunk->Mutex.lock();
	auto block = chunk->blocks.find(blockInChunkPos);
	if ((block != chunk->blocks.end()))
		chunk->blocks.erase(block);
	else if (_block != BlockName::Air)
		chunk->PutBlock(_block, ToChunkPosition(pos));
	else {
		chunk->Mutex.unlock();
		return;
	}
	chunk->Mutex.unlock();

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

void World::GenerateChunk(Chunk* chunk)
{
	float grassHeight;
	float dirtHeight;

	chunk->Mutex.lock();
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
	chunk->Mutex.unlock();
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

void World::UpdateMesh(vec2 ChunkPoition)
{
	auto chunk = GetChunk(ChunkPoition);
	if (chunk != nullptr)
		UpdateMesh(chunk);
}

void World::UpdateMesh(Chunk* chunk)
{
	auto pair = std::make_pair(nullptr, chunk);
	chunk->Mutex.lock();
	if (chunk->State != ChunkState::HasMesh) {
		chunk->Mutex.unlock();
		JobsMutex.lock();
		Jobs.push_back(pair);
		JobsMutex.unlock();
		return;
	}
	chunk->Mutex.unlock();
}


Chunk* World::RequestChunkGenerate(vec2 chunkPosition)
{
	if (GetChunk(chunkPosition) != nullptr)
		return nullptr;

	auto chunk = new Chunk(chunkPosition);
	auto pair = std::make_pair(nullptr, chunk);
	JobsMutex.lock();
	Jobs.push_back(std::make_pair(nullptr, chunk));
	JobsMutex.unlock();
	return chunk;
}

void World::SetRenderedChunks(vec2 centerChunkPos)
{
	vec2 position;

	std::unordered_map<vec2, std::pair<Model*, Chunk*>> oldChunks;

	for (auto& chunk : RenderedChunks) {
		if (chunk.second.second == nullptr)
			break;
		oldChunks.emplace(chunk.second.second->chunkPosition, std::make_pair(chunk.second.first, chunk.second.second));
	}

	for (auto& chunk : RenderedChunks) {
		position = centerChunkPos + chunk.first;

		//Scenario 1 - no chunks data
		if (chunk.second.second == nullptr) {
			chunk.second.second = RequestChunkGenerate(position);
			continue;
		}

		{
			auto tmpChunk = oldChunks.find(position);

			//Put existing chunk 
			if (tmpChunk != oldChunks.end()) {
				chunk.second.first = tmpChunk->second.first;
				chunk.second.second = tmpChunk->second.second;
				continue;
			}
		}
		//No chunk found let's try in Chunk's map:

		{
			auto tmpChunk = GetChunk(position);
			if (tmpChunk != nullptr) {
				//Put old mesh to chunk
				chunk.second.second->vertices = std::move(chunk.second.first->vertices);
				chunk.second.second->indices = std::move(chunk.second.first->indices);
				//Put new chunk's mesh to model
				chunk.second.second = tmpChunk;
				chunk.second.first->vertices = std::move(tmpChunk->vertices);
				chunk.second.first->indices = std::move(tmpChunk->indices);
				continue;
			}
		}

		chunk.second.second = RequestChunkGenerate(position);
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

			if (job.second == nullptr)
				continue;

			job.second->Mutex.lock();

			if (job.first == nullptr) {
				//Generate CHUNK
				if (job.second->State == ChunkState::NoData) {
					job.second->Mutex.unlock();
					GenerateChunk(job.second);
					ChunksMutex.lock();
					Chunks.emplace(job.second->chunkPosition, job.second);
					ChunksMutex.unlock(); 

					UpdateMesh(job.second->chunkPosition + vec2(-1, 0));
					UpdateMesh(job.second->chunkPosition + vec2(1, 0));
					UpdateMesh(job.second->chunkPosition + vec2(0, -1));
					UpdateMesh(job.second->chunkPosition + vec2(0, 1));
					
					continue;
				}
				//Build mesh
				if (job.second->State == ChunkState::Generated) {
					job.second->Mutex.unlock();
					job.second->BuildMesh();
					continue;
				}
			}

			//We have complete pair, what to do?
			if (job.first != nullptr) {
				//Lets update model's mesh
				if (job.second->State == ChunkState::BuildPending) {
					/*job.first->indices.clear();
					job.first->vertices.clear();
					job.first->vertices = std::move(job.second->vertices);
					job.first->indices = std::move(job.second->indices);
					job.second->State = ChunkState::HasMesh;
					job.second->Mutex.unlock();*/
					continue;
				}
				//Build chunk's mesh
				if (job.second->State == ChunkState::Generated) {
					job.second->Mutex.unlock();
					job.second->BuildMesh();
					job.second->Mutex.lock();
					job.first->vertices = (job.second->vertices);
					job.first->indices = (job.second->indices);
					job.second->Mutex.unlock();
					continue;
				}
			}
			
			job.second->Mutex.unlock();
		}
		else
			JobsMutex.unlock();

		auto now = std::chrono::high_resolution_clock::now();
		do {
			std::this_thread::yield();
		} while (std::chrono::high_resolution_clock::now() < now + std::chrono::microseconds(20));
	}
}