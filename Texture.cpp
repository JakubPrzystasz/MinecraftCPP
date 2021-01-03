#include "Texture.h"


Texture::Texture(std::string fileNamePath)
{
	this->fileNamePath = fileNamePath;
}

Texture::~Texture()
{
	this->Unbind(this->target);
	if (this->id > 0)
		glDeleteTextures(1, &this->id);
}

void Texture::Bind()
{
	glActiveTexture(index);
	glBindTexture(target, id);
}

void Texture::Unbind(GLuint target)
{
	glBindTexture(target, 0);
}

unsigned int Texture::GetId()
{
	return id;
}

void Texture::Init(GLuint index)
{
	if (id != 0)
		return;

	this->index = GL_TEXTURE0 + index;
	glGenTextures(1, &id);
	glActiveTexture(this->index);
	glBindTexture(target, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

	if (fileNamePath == "")
		return;

	if (LoadResource() == 0) {
		if(colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if(colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "ERROR::TEXTURE::UNABLE TO READ TEXTURE" << std::endl;
		return;
	}

	FreeResource();

}
