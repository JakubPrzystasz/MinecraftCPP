#include "Texture.h"


Texture::Texture(std::string fileNamePath)
{
	this->fileNamePath = fileNamePath;
	
	glGenTextures(1, &id);
	glBindTexture(target, id);

	if (!LoadResource()) {
		glTexImage2D(target, 0, GL_RGB, GetWith(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "UNABLE TO READ TEXTURE" << std::endl;
		return;
	}

	FreeResource();

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
	glActiveTexture(index);
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
