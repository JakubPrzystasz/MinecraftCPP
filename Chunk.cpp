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
		if (__block.first.x == 0) {
			auto foreginBlock = world->GetBlock(__block.first);
		}

		//Front
		if (!FindAdjacent(__block.first + vec3(0, 0, 1))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Front]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Back
		if (!FindAdjacent(__block.first + vec3(0, 0, -1))) {
			faces++;
			tmp = AddPosToFace(__block.first, _block.Faces[FaceName::Back]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
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
	return glm::vec3(\
		chunkPosition.x * chunkSize + pos.x, \
		pos.y, \
		chunkPosition.y * chunkSize + pos.z
	);
}



