#include "World.h"

void World::DrawChunks(Camera& camera)
{
	for (int i = 0; i < Chunks.size(); i++) {
		Chunks[i].chunk.Draw(camera);
	}
}

void World::GenerateWorld()
{
	for (int i = 0; i < 4; i++) {
		float xpos, ypos;
		xpos = i / 16;
		ypos = i % 16;
		Chunk tmp = Chunk();
		tmp.Init();
		tmp.chunkPosition = glm::vec2(xpos,ypos);

		float grassHeight;
		float dirtHeight;

		int seed = time(NULL);
		for (int c = 0; c < 2; c++)
		{
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					grassHeight = stb_perlin_noise3_seed((float)(x + 16 * xpos) / 16.f, 0.f, (float)(z + 16 * ypos) / 16.f, 0, 0, 0, seed) * (-8) + 16;
					dirtHeight = stb_perlin_noise3_seed((float)(x + 16 * xpos) / 32.f, 0.f, (float)(z + 16 * ypos) / 32, 0, 0, 0, seed) * (-2) + 10;

					for (int y = 0; y < grassHeight; y++) {
						if (y < dirtHeight) {
							tmp.PutBlock(BlockName::Stone, x, y, z);
							continue;
						}
						if (y + 1 < grassHeight) {
							tmp.PutBlock(BlockName::Dirt, x, y, z);
							continue;
						}
						tmp.PutBlock(BlockName::Grass, x, y, z);
					}

				}
			}
		}

		Chunks.push_back(WorldChunk(glm::vec2(xpos, ypos), tmp));

	}
}
