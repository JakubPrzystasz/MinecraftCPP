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
	auto block = new BlockPos;
	block->block = blockName;
	block->pos = glm::vec3(x, y, z);
	this->blocks.push_back(*block);
	delete block;
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

bool Chunk::FindAdjacent(glm::vec3 position) {
	for (auto const& block : blocks)
	{
		if (position.x == block.pos.x && \
			position.y == block.pos.y && \
			position.z == block.pos.z) {
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
	for (auto const& __block : blocks)
	{
		if (__block.pos.y == 0)
			continue;

		_block = Cube(*rs->GetBlock(__block.block));
		//Front
		if (!FindAdjacent(__block.pos + glm::vec3(0, 0, 1))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Front]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Back
		if (!FindAdjacent(__block.pos + glm::vec3(0, 0, -1))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Back]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Top
		if (!FindAdjacent(__block.pos + glm::vec3(0, 1, 0))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Top]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Bottom
		if (!FindAdjacent(__block.pos + glm::vec3(0, -1, 0))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Bottom]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Left
		if (!FindAdjacent(__block.pos + glm::vec3(-1, 0, 0))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Left]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
		//Right
		if (!FindAdjacent(__block.pos + glm::vec3(1, 0, 0))) {
			faces++;
			tmp = AddPosToFace(__block.pos, _block.Faces[FaceName::Right]);
			AddIndices(tmp.indices, 6);
			AddVertices(tmp.vertices, 4);
		}
	}

	updateChunk = false;
}

Face Chunk::AddPosToFace(glm::vec3 pos, Face& face)
{
	
	Face tmp = Face(face);
	for (int i = 0; i < 4; i++) {
		tmp.vertices[i].Position = tmp.vertices[i].Position + pos + glm::vec3(chunkPosition.x * chunkSize, 0, chunkPosition.y * chunkSize);
	}

	return tmp;
}

glm::vec3 Chunk::ToWorldPosition(glm::vec3 pos)
{
	glm::vec3 tmp = glm::vec3(pos);
	tmp = tmp + glm::vec3(chunkPosition.x * chunkSize, 0, chunkPosition.y * chunkSize);
	return tmp;
}



