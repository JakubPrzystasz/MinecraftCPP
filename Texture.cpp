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

void Texture::Bind(GLenum index)
{
	this->index = index;
	glActiveTexture(GL_TEXTURE0+index);
	glBindTexture(target, id);
}

void Texture::Bind()
{
	Bind(GL_TEXTURE0);
}

void Texture::Unbind(GLuint target)
{
	glBindTexture(target, 0);
}

GLuint Texture::GetId()
{
	return id;
}

void Texture::Init()
{

	glGenTextures(1, &id);
	glBindTexture(target, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

	if (LoadResource() == 0) {
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "ERROR::TEXTURE::UNABLE TO READ TEXTURE" << std::endl;
		return;
	}

	FreeResource();

}
