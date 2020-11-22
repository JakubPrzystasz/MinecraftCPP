#pragma once
#include "Shader.h"


int Shader::CheckCompileError()
{
	GLchar infoLog[1024];
	int result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION FAILED: " << infoLog << std::endl;
	}
	return 0;
}

Shader::Shader(std::string fileNamePath, GLenum type)
{
	if (fileNamePath.length() > 0)
	{
		this->fileNamePath = fileNamePath;
		this->type = type;
		this->id = glCreateShader(type);
	}
	else {
		std::cout << "ERROR::SHADER::NO FILE SPECIFIED " << std::endl;
	}
}

Shader::~Shader()
{
	if(id > 0)
		glDeleteShader(id);
	if (this->FreeResource() != 0)
		std::cout << "ERROR::SHADER::FAILED TO FREE RESOURCE" << std::endl;
}

int Shader::CompileShader()
{
	if (LoadResource() == 0) {
		GetId();
		const char* tmp = (const char*)data;
		glShaderSource(this->id, 1, &tmp, NULL);
		glCompileShader(id);
		if (CheckCompileError() == 0)
			return 0;
	}
	else {
		return -1;
	}
	return -1;
}



GLuint Shader::GetId(void)
{
	return id;
}
