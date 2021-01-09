#include "Texture.h"


Texture::Texture(std::string fileNamePath,GLuint index)
{
	target = GL_TEXTURE_2D;
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	filterMin = GL_NEAREST;
	filterMag = GL_NEAREST;
	this->fileNamePath = fileNamePath;
	Init(index);
}

Texture::~Texture()
{
	this->Unbind(this->target);
	if (this->id > 0)
		glDeleteTextures(1, &this->id);
}

void Texture::Bind()
{
	glActiveTexture(textureUnit);
	glBindTexture(target, id);
}

void Texture::Unbind(GLuint target)
{
	glBindTexture(target, 0);
}

void Texture::Unbind()
{
	glBindTexture(target, 0);
}

unsigned int Texture::GetId()
{
	return id;
}

void Texture::Init(GLuint TextureUnitIndex)
{
	if (id != 0)
		return;

	this->textureUnit = static_cast<GLenum>(GL_TEXTURE0 + static_cast<GLenum>(TextureUnitIndex));
	glGenTextures(1, &id);
	
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

	if (fileNamePath == "")
		return;

	if (LoadResource() == 0) {
		if(colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "ERROR::TEXTURE::UNABLE TO READ TEXTURE" << std::endl;
		return;
	}

	//FreeResource();

	Unbind();

}
