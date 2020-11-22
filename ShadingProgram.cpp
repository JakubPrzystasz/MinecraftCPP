#include "ShadingProgram.h"
#include "ResourceManager.h"

int ShadingProgram::CheckCompileError()
{
	GLchar infoLog[1024];
	int result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM:: LINKING ERROR " << "\n" << infoLog << "\n -- --------------------------------- " << std::endl;
	}
	return 0;
}

ShadingProgram::ShadingProgram(std::string vertexFileNamePath, std::string fragmentFileNamePah)
{
	if (vertexFileNamePath.length() > 0 && fragmentFileNamePah.length() > 0) {
		ResourceManager* RS = ResourceManager::GetInstance();
		frag = RS->GetShader(fragmentFileNamePah,GL_FRAGMENT_SHADER);
		vert = RS->GetShader(vertexFileNamePath, GL_VERTEX_SHADER);

		id = glCreateProgram();
		frag->CompileShader();
		vert->CompileShader();

		glAttachShader(id, frag->GetId());
		glAttachShader(id, vert->GetId());
		glLinkProgram(id);
		CheckCompileError();
	}
	else {
		std::cout << "ERROR::SHADER::NO FILE SPECIFIED" << std::endl;
	}
}

ShadingProgram::~ShadingProgram()
{
	glDeleteProgram(id);
}

void ShadingProgram::Use()
{
	glUseProgram(id);
}

GLuint ShadingProgram::GetId()
{
	return id;
}

void ShadingProgram::SetData(const GLchar* name, const bool& value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void ShadingProgram::SetData(const GLchar* name, const float& value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void ShadingProgram::SetData(const GLchar* name, const int& value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void ShadingProgram::SetData(const GLchar* name, const float& x, const float& y)
{
	glUniform2f(glGetUniformLocation(id, name), x, y);
}

void ShadingProgram::SetData(const GLchar* name, const glm::vec2& vector)
{
	glUniform2f(glGetUniformLocation(id, name), vector.x, vector.y);
}

void ShadingProgram::SetData(const GLchar* name, const float& x, const float& y, const float& z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void ShadingProgram::SetData(const GLchar* name, const glm::vec3& vector)
{
	glUniform3f(glGetUniformLocation(id, name), vector.x, vector.y, vector.z);
}

void ShadingProgram::SetData(const GLchar* name, const float& x, const float& y, const float& z, const float& w)
{
	glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

void ShadingProgram::SetData(const GLchar* name, const glm::vec4& vector)
{
	glUniform4f(glGetUniformLocation(id, name), vector.x, vector.y, vector.z, vector.w);
}

void ShadingProgram::SetData(const GLchar* name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShadingProgram::SetData(const GLchar* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShadingProgram::SetData(const GLchar* name, const GLuint& value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

