#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include "ShaderCache.h"


/*
	Shader Class, handles operations over Shaders in GLContext
*/
class Shader :
	public ShaderCache
{
private:
	/// <summary>
	/// Stores type of shader held in memory
	/// </summary>
	GLenum type;

	/// <summary>
	/// Holds unique id of Shader in GLContext
	/// </summary>
	GLuint id = -1;

	/// <summary>
	/// Checks if compilation succeeds, 
	/// </summary>
	/// <returns></returns>
	int CheckCompileError();


	/// <summary>
	/// Create shader in GL Context
	/// </summary>
	/// <returns>0 when succeeds</returns>
	int CreateShader(void);
public:
	/// <summary>
	/// Constructor for Shader class, file name is required
	/// </summary>
	/// <param name="fileNamePath">Texture file name path</param>
	Shader(const std::string fileNamePath, const GLenum Shadertype);

	Shader() = default;

	/// <summary>
	/// Destructor for TextureCache class
	/// </summary>
	~Shader();

	/// <summary>
	/// Compile shader
	/// </summary>
	/// <returns>Returns 0 if succeeds</returns>
	int CompileShader();

	/// <summary>
	/// Returns ID of GL shader object
	/// </summary>
	/// <returns>GL Shader id</returns>
	GLuint GetId(void);

};

