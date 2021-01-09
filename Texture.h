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
	GLuint id;

	/// <summary>
	/// Specifies target texture ( 1D array, 2D, CUBEMAP)
	/// </summary>
	GLuint target;

	/// <summary>
	/// Wrap mode in S ( X axis)
	/// </summary>
	GLuint wrapS;

	/// <summary>
	/// Wrap mode in T ( Y axis)
	/// </summary>
	GLuint wrapT;

	/// <summary>
	/// Texture interpolation mode when scalling down
	/// </summary>
	GLuint filterMin;

	/// <summary>
	/// Texture interpolation mode when scalling up
	/// </summary>
	GLuint filterMag;

	GLenum index;

	//Initialize texture
	void Init(GLuint index);
	
public:
	/// <summary>
	/// Constructor for Texture class, file name is required
	/// </summary>
	/// <param name="fileNamePath">Texture file name path</param>
	Texture(std::string fileNamePath, GLuint index);

	/// <summary>
	/// Destructor for Texture class
	/// </summary>
	~Texture();


	/// <summary>
	/// Bind texture to GL context
	/// </summary>
	void Bind();

	/// <summary>
	/// Remove  texture form GL context
	/// </summary>
	/// <param name="target">Texture target eg. GL_TEXTURE_2D</param>
	static void Unbind(GLuint target);
	
	void Unbind();

	/// <summary>
	/// Returns ID of Texure for GLcontext
	/// </summary>
	/// <returns>Uniqure id of texture</returns>
	GLuint GetId();
};

