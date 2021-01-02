#include "Chunk.h"

void Chunk::Init()
{
	auto RS = ResourceManager::GetInstance();
	Model::Init();
	SetShadingProgram(RS->GetShadingProgram("block"));
	AddTexture("face", RS->GetTexture("Textures/terrain.png"));
	shadingProgram->Use();
	Textures["face"]->Init();
	shadingProgram->SetData("texture1", Textures["face"]->GetId());
	Textures["face"]->Bind(GL_TEXTURE1);
	world = World::GetInstance();
}

void Chunk::PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z)
{
	blocks.emplace(vec3(x, y, z), blockName);
	updateChunk = true;
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

bool Chunk::FindAdjacent(vec3 position) const {
	for (auto const& block : blocks)
	{
		if (position.x == block.first.x && \
			position.y == block.first.y && \
			position.z == block.first.z) {
			if (block.second == BlockName::Air)
				return false;
			else
				return true;
		}
	}
	return false;
}

void Chunk::ChunkUpdate()
{
	auto rs = ResourceManager::GetInstance();
	Face tmp;
	Cube _block;
	GLuint faces = 0;
	vertices.clear();
	indices.clear();
	for (auto& __block : blocks)
	{
		if (__block.first.y == 0)
			continue;

		if (__block.second == BlockName::Air)
			continue;

		_block = *rs->GetBlock(__block.second);

		//back face 
		if (__block.first.z == 0) {
			auto tmpPos = __block.first;
			tmpPos.z = chunkSize - 1;
			auto x__ = ToWorldPosition(tmpPos, vec2(chunkPosition.x - 1, chunkPosition.y));
			auto foreginBlock = world->GetBlock(x__);
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Back]);
				AddIndices(tmp.indices, 6);
				AddVertices(tmp.vertices, 4);
			}
		}
		else {
			//Back
			if (!FindAdjacent(__block.first + vec3(0, 0, -1))) {
				faces++;
				tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Back]);
				AddIndices(tmp.indices, 6);
				AddVertices(tmp.vertices, 4);
			}
		}

		if (__block.first.z == chunkSize - 1) {

			auto tmpPos = __block.first;
			tmpPos.z = 0;
			auto foreginBlock = world->GetBlock(ToWorldPosition(tmpPos));
			if (foreginBlock == BlockName::Air) {
				faces++;
				tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Back]);
				AddIndices(tmp.indices, 6);
				AddVertices(tmp.vertices, 4);
			}

		}else {
			//Front
			if (!FindAdjacent(__block.first + vec3(0, 0, 1))) {
				faces++;
				tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Front]);
				AddIndices(tmp.indices, 6);
				AddVertices(tmp.vertices, 4);
			}
		}


		
		//Top
		if (!FindAdjacent(__block.first + vec3(0, 1, 0))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Top]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Bottom
		if (!FindAdjacent(__block.first + vec3(0, -1, 0))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Bottom]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Left
		if (!FindAdjacent(__block.first + vec3(-1, 0, 0))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Left]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Right
		if (!FindAdjacent(__block.first + vec3(1, 0, 0))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Right]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
	}

	updateChunk = false;
}

Face Chunk::AddPosToFace(vec3 pos, Face& face)
{
	
	Face tmp = Face(face);
	for (auto & vert : tmp.vertices) {
		vert.Position = ToWorldPosition(pos + vert.Position);
	}
	return tmp;
}

glm::vec3 Chunk::ToWorldPosition(vec3 pos)
{ 
	auto ret = glm::vec3(
		round((int)chunkPosition.x * (int)chunkSize + pos.x), 
		round(pos.y), 
		round((int)chunkPosition.y * (int)chunkSize + pos.z)
	);
	return ret;
}

inline glm::vec3 Chunk::ToWorldPosition(vec3 pos, vec2 chunkPos)
{
	auto ret = glm::vec3(
		round((int)chunkPos.x * (int)chunkSize + pos.x), 
		round(pos.y), 
		round((int)chunkPos.y * (int)chunkSize + pos.z)
	);

	return ret;
}



