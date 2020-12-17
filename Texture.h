#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "TextureCache.h"

/*
	Texture Class, handles operations over Textures in GLContext
*/
class Texture:
	public TextureCache
{
private:
	/// <summary>
	/// Unique identifier of texture 
	/// </summary>
	GLuint id = 0;

	/// <summary>
	/// Specifies target texture ( 1D array, 2D, CUBEMAP)
	/// </summary>
	GLuint target = GL_TEXTURE_2D;

	/// <summary>
	/// Wrap mode in S ( X axis)
	/// </summary>
	GLuint wrapS = GL_REPEAT;

	/// <summary>
	/// Wrap mode in T ( Y axis)
	/// </summary>
	GLuint wrapT = GL_REPEAT;

	/// <summary>
	/// Texture interpolation mode when scalling down
	/// </summary>
	GLuint filterMin = GL_NEAREST;

	/// <summary>
	/// Texture interpolation mode when scalling up
	/// </summary>
	GLuint filterMag = GL_NEAREST;

	GLenum index = GL_TEXTURE0;
	
public:
	/// <summary>
	/// Constructor for Texture class, file name is required
	/// </summary>
	/// <param name="fileNamePath">Texture file name path</param>
	Texture(std::string fileNamePath);

	/// <summary>
	/// Destructor for Texture class
	/// </summary>
	~Texture();

	/// <summary>
	/// Bind texture to GL context
	/// </summary>
	void Bind(GLenum index);

	/// <summary>
	/// Bind texture to GL context to 0 index
	/// </summary>
	void Bind();

	/// <summary>
	/// Remove  texture form GL context
	/// </summary>
	/// <param name="target">Texture target eg. GL_TEXTURE_2D</param>
	static void Unbind(GLuint target);

	/// <summary>
	/// Returns ID of Texure for GLcontext
	/// </summary>
	/// <returns>Uniqure id of texture</returns>
	GLuint GetId();


	//Initialize texture
	void Init();
};

