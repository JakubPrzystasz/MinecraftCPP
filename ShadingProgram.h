#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

/// <summary>
/// This class contains methods that manages
/// single shading program in GL context
/// </summary>
class ShadingProgram
{
private:
	/// <summary>
	/// Holds vertex shader 
	/// </summary>
	Shader* vert;

	/// <summary>
	/// Holds fragment shader
	/// </summary>
	Shader* frag;

	/// <summary>
	/// Checks if compilation succeeds, 
	/// </summary>
	/// <returns></returns>
	int CheckCompileError();

	/// <summary>
	/// Unique id of shading program
	/// </summary>
	GLuint id;
public:

	/// <summary>
	/// Constructor of ShadingProgram class
	/// </summary>
	/// <param name="vertexFileNamePath">File name of vertex shader</param>
	/// <param name="fragmentFileNamePah">File name of fragment shader</param>
	ShadingProgram(std::string vertexFileNamePath, std::string fragmentFileNamePah);

	/// <summary>
	/// Removes resources from memory (if exist in it), then destroys an object
	/// </summary>
	~ShadingProgram();

	/// <summary>
	/// Bind shading program to GL context
	/// </summary>
	void Use();

	GLuint GetId();

	/// <summary>
	/// Set variable for shading program
	/// </summary>
	/// <param name="name">Name of uniform</param>
	/// Bool
	void SetData(const GLchar* name, const bool& value);
	/// Float
	void SetData(const GLchar* name, const float& value);
	/// Integer
	void SetData(const GLchar* name, const int& value);
	/// Vector2(float)
	void SetData(const GLchar* name, const float& x, const float& y);
	void SetData(const GLchar* name, const glm::vec2& vector);
	/// Vector3(float)
	void SetData(const GLchar* name, const float& x, const float& y, const float& z);
	void SetData(const GLchar* name, const glm::vec3& vector);
	/// Vector4(float)
	void SetData(const GLchar* name, const float& x, const float& y, const float& z, const float& w);
	void SetData(const GLchar* name, const glm::vec4& vector);
	/// Matrix3(float)
	void SetData(const GLchar* name, const glm::mat3& matrix);
	/// Matrix4(float)
	void SetData(const GLchar* name, const glm::mat4& matrix);
	/// Texture
	void SetData(const GLchar* name, const GLuint &value);

};

