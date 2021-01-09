#define _CRT_SECURE_NO_WARNINGS
#include "ShaderCache.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderCache::ShaderCache(std::string fileNamePath)
{
    inMemory = false;
    dataSize = 0;
    data = nullptr;
    fileNamePath = "";

    if (fileNamePath.length() > 0)
        this->fileNamePath = fileNamePath;
    else
        std::cout << "ERROR::SHADER::NO FILE SPECIFIED" << std::endl;
}

ShaderCache::~ShaderCache()
{
	FreeResource();
}

int ShaderCache::LoadResource()
{
    if (inMemory == true)
        return 0;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCode;
    std::ifstream shaderFile;
    
    //Make shaderFile object can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open file
        shaderFile.open(this->fileNamePath);

        std::stringstream shaderStream;
        //Read file's content into stream
        shaderStream << shaderFile.rdbuf();

        //Close file handle
        shaderFile.close();

        //Convert stream into string
        shaderCode = shaderStream.str();

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FAILED TO LOAD FILE:"<< e.what() << std::endl;
        return -1;
    }

    data = new char[shaderCode.length() + 1];
    strcpy((char*)data,shaderCode.c_str());
    inMemory = true;

	return 0;
}

int ShaderCache::FreeResource()
{
    if (inMemory) {
        delete[]data;
        inMemory = false;
    }
    return 0;
}
