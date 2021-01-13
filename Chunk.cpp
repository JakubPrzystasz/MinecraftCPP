#include "Chunk.h"

void Chunk::BuildMesh(Model* model)
{
	if (State != ChunkState::BuildPending && State != ChunkState::Generated)
		return;

	auto rs = ResourceManager::GetInstance();
	auto world = World::GetInstance();

	Face tmpFace;
	Cube blockModel;

	for (auto& chunkBlock : blocks)
	{
		//ommit rendering bottom face
		if (chunkBlock.first.y == 0)
			continue;
		//if block is air
		if (chunkBlock.second == BlockName::Air)
			continue;

		blockModel = *rs->GetBlock(chunkBlock.second);

		//Top
		if (GetBlock(chunkBlock.first + vec3(0, 1, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Top]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}
		//Bottom
		if (GetBlock(chunkBlock.first + vec3(0, -1, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Bottom]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}

		//FRONT
		if (chunkBlock.first.z == static_cast<GLfloat>(world->chunkSize - 1)) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.z = 0;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(0, 1));
			if (foreginChunk != nullptr) {
				auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
				if (foreginBlock == BlockName::Air) {
					faces++;
					tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Front]);
					model->AddIndices(tmpFace.indices, 6);
					model->AddVertices(tmpFace.vertices, 4);
				}
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(0, 0, 1)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Front]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}


		//BACK
		if (chunkBlock.first.z == 0) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.z = static_cast<GLfloat>(world->chunkSize - 1);
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(0, -1));
			if (foreginChunk != nullptr) {
				auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
				if (foreginBlock == BlockName::Air) {
					faces++;
					tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Back]);
					model->AddIndices(tmpFace.indices, 6);
					model->AddVertices(tmpFace.vertices, 4);
				}
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(0, 0, -1)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Back]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}

		//LEFT face
		if (chunkBlock.first.x == 0) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.x = static_cast<GLfloat>(world->chunkSize - 1);
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(-1, 0));
			if (foreginChunk != nullptr) {
				auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
				if (foreginBlock == BlockName::Air) {
					faces++;
					tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Left]);
					model->AddIndices(tmpFace.indices, 6);
					model->AddVertices(tmpFace.vertices, 4);
				}
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(-1, 0, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Left]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}

		//RIGHT face
		if (chunkBlock.first.x == static_cast<GLfloat>(world->chunkSize - 1)) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.x = 0;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(1, 0));
			if (foreginChunk != nullptr) {
				auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
				if (foreginBlock == BlockName::Air) {
					faces++;
					tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Right]);
					model->AddIndices(tmpFace.indices, 6);
					model->AddVertices(tmpFace.vertices, 4);
				}
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(1, 0, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Right]);
			model->AddIndices(tmpFace.indices, 6);
			model->AddVertices(tmpFace.vertices, 4);
		}

	}

	State = ChunkState::HasMesh;
}

void Chunk::PutBlock(BlockName blockName, vec3 pos)
{
	blocks.emplace(pos, blockName);
}

void Chunk::PutBlock(BlockName blockName, GLuint x, GLuint y, GLuint z)
{
	blocks.emplace(vec3(x, y, z), blockName);
}

BlockName Chunk::GetBlock(vec3 position)
{
	auto tmp = blocks.find(position);
	if (tmp != blocks.end())
		return tmp->second;
	return BlockName::Air;
}

GLuint Chunk::CountFaces()
{
	return this->faces;
}

Face Chunk::AddPosToFace(vec3 pos, Face& face)
{

	Face tmp = Face(face);
	for (auto& vert : tmp.vertices) {
		vert.Position = ToWorldPosition(pos + vert.Position, chunkPosition);
	}
	return tmp;
}

glm::vec3 Chunk::ToWorldPosition(vec3 pos)
{
	auto world = World::GetInstance();
	return glm::vec3(
		World::RoundInt(chunkPosition.x * static_cast<int>(world->chunkSize) + pos.x),
		World::RoundInt(pos.y),
		World::RoundInt(chunkPosition.y * static_cast<int>(world->chunkSize) + pos.z)
	);
}

glm::vec3 Chunk::ToWorldPosition(vec3 pos, vec2 chunkPos)
{
	auto world = World::GetInstance();
	return glm::vec3(
		World::RoundInt(chunkPos.x * static_cast<int>(world->chunkSize) + pos.x),
		World::RoundInt(pos.y),
		World::RoundInt(chunkPos.y * static_cast<int>(world->chunkSize) + pos.z)
	);
}
