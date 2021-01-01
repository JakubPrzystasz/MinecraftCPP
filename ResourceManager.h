#pragma once
#include <map>
#include <string>
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "ShadingProgram.h"
#include "Model.h"
#include "Cube.h"


/// <summary>
/// Resource Manager is singleton class, that manages all resources for application
/// </summary>
class ResourceManager
{
private:
	/// <summary>
	/// Hide constructor of class
	/// </summary>
	ResourceManager() {};

    /// <summary>
    /// Single instance of class
    /// </summary>
    static ResourceManager* instance;

    /// <summary>
    ///  Holds all textures
    /// </summary>
    static std::map<std::string, Texture*> Textures;

    /// <summary>
    ///  Holds all shaders
    /// </summary>
    static std::map<std::string, Shader*> Shaders;

    /// <summary>
    ///  Holds all shading programs
    /// </summary>
    static std::map<std::string, ShadingProgram*> ShadingPrograms;

    /// <summary>
    ///  Holds all shading programs
    /// </summary>
    static std::map<std::string, Model*> Models;

    static std::map<BlockName, Cube*> Blocks;

    /// <summary>
    /// Load specified texture from file
    /// </summary>
    /// <param name="TextureName">Name of texture file</param>
    static void LoadTexture(const std::string TextureName);


    /// <summary>
    /// Load specified shader from file
    /// </summary>
    /// <param name="ShaderName">Name of shader file</param>
    static void LoadShader(const std::string ShaderName,const GLenum type);

public:

    /// <summary>
    /// Do not allow to copy an object
    /// </summary>
    /// <param name="object"></param>
    ResourceManager(ResourceManager& object) = delete;

    /// <summary>
    /// Do not allow to assign a instance to new object
    /// </summary>
    /// <param name=""></param>
    void operator=(const ResourceManager&) = delete;
    
    /// <summary>
    /// On first run, creates static object of class 
    /// every next use of this function returns pointer to the same object.
    /// </summary>
    /// <returns>Pointer to Singleton instance of ResourceManager</returns>
    static ResourceManager* GetInstance();

    /// <summary>
    /// Loads texture from file to program memory, ready to use in GL context
    /// </summary>
    /// <param name="TextureName">Name of texture file</param>
    /// <returns></returns>
    static Texture* GetTexture(const std::string TextureName);

    /// <summary>
    /// Loads shader from file to program memory, ready to use in GL context
    /// </summary>
    /// <param name="ShaderName">Name of shader file</param>
    /// <returns></returns>
    static Shader* GetShader(const std::string ShaderName, const GLenum type);

    /// <summary>
    /// Add shading program to map
    /// </summary>
    /// <param name="name">Name of shading program</param>
    /// <param name="vertexFileNamePath">File name of vertex shader</param>
    /// <param name="fragmentFileNamePah">File name of fragment shader</param>
    static void AddShadingProgram(std::string name, std::string vertexFileNamePath, std::string fragmentFileNamePah);
    
    /// <summary>
    /// Return shading program
    /// </summary>
    /// <param name="name">Name of shading program </param>
    static ShadingProgram* GetShadingProgram(std::string name);

    /// <summary>
    /// Add model to map
    /// </summary>
    /// <param name="Name"></param>
    /// <param name="model"></param>
    static void AddModel(const std::string Name, Model* model);

    /// <summary>
    /// Loads shader from file to program memory, ready to use in GL context
    /// </summary>
    /// <param name="ShaderName">Name of shader file</param>
    /// <returns></returns>
    static Model* GetModel(const std::string Name);

    /// <summary>
    /// Add block to map
    /// </summary>
    /// <param name="Name"></param>
    /// <param name="model"></param>
    static void AddBlock(BlockName blockName, FaceTexture front, FaceTexture back, FaceTexture top, FaceTexture bottom, FaceTexture left, FaceTexture right);

    /// <summary>
    /// Loads block
    /// </summary>
    /// <param name="ShaderName">Name of shader file</param>
    /// <returns></returns>
    static Cube* GetBlock(BlockName blockName);

    /// <summary>
    /// Removes all data from memory
    /// </summary>
    static void FreeResources();

};

