#include "Texture.h"


Texture::Texture(std::string fileNamePath)
{
	target = GL_TEXTURE_2D;
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	filterMin = GL_NEAREST;
	filterMag = GL_NEAREST;
	this->fileNamePath = fileNamePath;
	Init();
	InitImage();
}

Texture::Texture(FT_Face glyphFace)
{
	target = GL_TEXTURE_2D;
	wrapS = GL_CLAMP_TO_EDGE;
	wrapT = GL_CLAMP_TO_EDGE;
	filterMin = GL_LINEAR;
	filterMag = GL_LINEAR;
	this->fileNamePath = fileNamePath;
	Init();
	InitFont(glyphFace);
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


void Texture::Bind(GLuint TextureUnitIndex)
{
	textureUnit = static_cast<GLenum>(static_cast<GLenum>(GL_TEXTURE0) + static_cast<GLenum>(TextureUnitIndex));
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

void Texture::Init() {
	if (id != 0)
		return;

	glGenTextures(1, &id);

	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
}

void Texture::InitImage()
{
	if (fileNamePath == "")
		return;

	if (LoadResource() == 0) {
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "ERROR::TEXTURE::UNABLE TO READ TEXTURE" << std::endl;
		return;
	}

	FreeResource();

	Unbind();

}

void Texture::InitFont(FT_Face glyphFace)
{

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		glyphFace->glyph->bitmap.width,
		glyphFace->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		glyphFace->glyph->bitmap.buffer
	);

	Unbind();

}

