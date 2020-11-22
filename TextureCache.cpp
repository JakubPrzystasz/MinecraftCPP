#include "TextureCache.h"

TextureCache::TextureCache(std::string fileNamePath)
{
	if (fileNamePath.length() > 0)
	{
		this->fileNamePath = fileNamePath;
		this->inMemory = false;
		this->data = nullptr;
	}
	else
		std::cout << "ERROR::TEXTURE::NO FILE SPECIFIED" << std::endl;
}

TextureCache::~TextureCache()
{
	FreeResource();
}

int TextureCache::LoadResource()
{
	
	if (!inMemory) {
		
		data = stbi_load(fileNamePath.c_str(), (int*)(&width), (int*)(&height), (int*)(&colorChannels), 0);
		
		if (data)
		{
			inMemory = true;
			return 0;
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED TO LOAD TEXTURE: "<< fileNamePath << std::endl;
			return -1;
		}
	}

	return 0;
}

int TextureCache::FreeResource()
{
	if (!inMemory) {
		return 0;
	}
	else
	{
		free(data);
		inMemory = false;
		return 0;
	}
}

GLuint TextureCache::GetWith()
{
	return width;
}

GLuint TextureCache::GetHeight()
{
	return height;
}

GLuint TextureCache::GetChannels()
{
	return colorChannels;
}
