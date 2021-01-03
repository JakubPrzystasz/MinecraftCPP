#include "Chunk.h"

void Chunk::Init()
{
	auto RS = ResourceManager::GetInstance();
	Model::Init();
	SetShadingProgram(RS->GetShadingProgram("block"));
	AddTexture("face", RS->GetTexture("Textures/terrain.png"));
	shadingProgram->Use();
	Textures["face"]->Bind();
	shadingProgram->SetData("blockTexture", Textures["face"]->GetId());
	world = World::GetInstance();
}

void Chunk::PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z)
{
	blocks.emplace(vec3(x, y, z), blockName);
	updateChunk = true;
}

void Chunk::PutBlock(BlockName blockName, glm::vec3 pos)
{
	blocks.emplace(vec3(pos.x, pos.y, pos.z), blockName);
	updateChunk = true;
}

void Chunk::PutBlock(BlockName blockName, vec3 pos)
{
	blocks.emplace(pos, blockName);
	updateChunk = true;
}

BlockName Chunk::GetBlock(vec3 position)
{
	auto tmp = blocks.find(position);
	if (tmp != blocks.end())
		return tmp->second;
	return BlockName::Air;
}

void Chunk::Draw(Camera& camera)
{
	if (updateChunk)
		ChunkUpdate();
	if (indices.size() < 1)
		return;
	BindData();
	shadingProgram->Use();
	shadingProgram->SetData("projection", camera.Projection);
	shadingProgram->SetData("view", camera.GetViewMatrix());
	shadingProgram->SetData("model", glm::mat4(1.0f));
	Model::Draw();

}

void Chunk::ChunkUpdate()
{
	auto rs = ResourceManager::GetInstance();

	Face tmpFace;
	Cube blockModel;
	//Count faces
	GLuint faces = 0;
	vertices.clear();
	indices.clear();

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
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}
		//Bottom
		if (GetBlock(chunkBlock.first + vec3(0, -1, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Bottom]);
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}

		//FRONT
		if (chunkBlock.first.z == chunkSize - 1) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.z = 0;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(0, 1));
			auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Front]);
				AddIndices(tmpFace.indices, 6);
				AddVertices(tmpFace.vertices, 4);
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(0, 0, 1)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Front]);
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}


		//BACK
		if (chunkBlock.first.z == 0) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.z = chunkSize - 1;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(0, -1));
			auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Back]);
				AddIndices(tmpFace.indices, 6);
				AddVertices(tmpFace.vertices, 4);
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(0, 0, -1)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Back]);
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}


		if (chunkBlock.first.x == 0) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.x = chunkSize - 1;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(-1, 0));
			auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Left]);
				AddIndices(tmpFace.indices, 6);
				AddVertices(tmpFace.vertices, 4);
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(-1, 0, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Left]);
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}

		if (chunkBlock.first.x == chunkSize - 1) {
			auto foreginBlockPos = chunkBlock.first;
			foreginBlockPos.x = 0;
			auto foreginChunk = world->GetChunk(chunkPosition + vec2(1, 0));
			auto foreginBlock = foreginChunk->GetBlock(foreginBlockPos);
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Right]);
				AddIndices(tmpFace.indices, 6);
				AddVertices(tmpFace.vertices, 4);
			}
		}
		else if (GetBlock(chunkBlock.first + vec3(1, 0, 0)) == BlockName::Air) {
			faces++;
			tmpFace = AddPosToFace(chunkBlock.first, blockModel.Faces[FaceName::Right]);
			AddIndices(tmpFace.indices, 6);
			AddVertices(tmpFace.vertices, 4);
		}

	}

	updateChunk = false;
}

Face Chunk::AddPosToFace(vec3 pos, Face& face)
{

	Face tmp = Face(face);
	for (auto& vert : tmp.vertices) {
		vert.Position = ToWorldPosition(pos + vert.Position);
	}
	return tmp;
}

glm::vec3 Chunk::ToWorldPosition(vec3 pos)
{
	auto ret = glm::vec3(
		World::RoundInt(World::RoundInt(chunkPosition.x) * chunkSize + pos.x),
		World::RoundInt(pos.y),
		World::RoundInt(World::RoundInt(chunkPosition.y) * chunkSize + pos.z)
	);
	return ret;
}

inline glm::vec3 Chunk::ToWorldPosition(vec3 pos, vec2 chunkPos)
{
	auto ret = glm::vec3(
		World::RoundInt(World::RoundInt(chunkPos.x) * chunkSize + pos.x),
		World::RoundInt(pos.y),
		World::RoundInt(World::RoundInt(chunkPos.y) * chunkSize + pos.z)
	);

	return ret;
}
