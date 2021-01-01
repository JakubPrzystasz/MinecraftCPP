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
}

void Chunk::PutBlock(BlockName blockName, unsigned int x, unsigned int y, unsigned int z)
{
	blocks.emplace(vec3(x, y, z), blockName);
	updateChunk = true;
}

void Chunk::Draw(Camera& camera)
{
	glm::mat4 model = glm::mat4(1.0f);
	if (updateChunk)
		ChunkUpdate();
	BindData();
	shadingProgram->Use();
	shadingProgram->SetData("projection", camera.Projection);
	shadingProgram->SetData("view", camera.GetViewMatrix());
	shadingProgram->SetData("model", model);
	Model::Draw();
}

bool Chunk::FindAdjacent(vec3 position) {
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
	for (int i = 0; i < 4; i++) {
	//	tmp.vertices[i].Position = ToWorldPosition(pos);
		tmp.vertices[i].Position = tmp.vertices[i].Position + glm::vec3(chunkPosition.x * chunkSize, 0, chunkPosition.y * chunkSize);
		tmp.vertices[i].Position.x = pos.x;
		tmp.vertices[i].Position.y = pos.y;
		tmp.vertices[i].Position.z = pos.z;
	}

	return tmp;
}

glm::vec3 Chunk::ToWorldPosition(vec3 pos)
{
	return glm::vec3(chunkPosition.x * chunkSize + pos.x, 0+pos.y, chunkPosition.y * chunkSize+pos.z);
}



