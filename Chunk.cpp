#include "Chunk.h"

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
	auto rs = ResourceManager::GetInstance();

	glm::mat4 _model = glm::mat4(1.0f);
	glm::mat4 model;
	GLuint faces = 0;
	for (auto const& __block : blocks)
	{
		faces = 0;
		//Front
		if (!FindAdjacent(__block.pos + glm::vec3(0, 0, -1))) {
			faces++;
		}
		//Back
		if (!FindAdjacent(__block.pos + glm::vec3(0, 0, 1))) {
			faces++;
		}
		//Top
		if (!FindAdjacent(__block.pos + glm::vec3(0, 1, 0))) {
			faces++;
		}
		//Bottom
		if (!FindAdjacent(__block.pos + glm::vec3(0, -1, 0))) {
			faces++;
		}
		//Left
		if (!FindAdjacent(__block.pos + glm::vec3(-1, 0, 0))) {
			faces++;
		}
		//Right
		if (!FindAdjacent(__block.pos + glm::vec3(1, 0, 0))) {
			faces++;
		}

		if (faces > 0) {
			Cube _block = Cube(*rs->GetBlock(__block.block));

			model = glm::translate(_model, __block.pos);
			_block.shadingProgram->Use();
			_block.shadingProgram->SetData("projection", camera.Projection);
			_block.shadingProgram->SetData("view", camera.GetViewMatrix());
			_block.shadingProgram->SetData("model", model);
			_block.Draw();
		}
	}
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



