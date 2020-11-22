#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;
std::map<std::string, Texture*> ResourceManager::Textures;
std::map<std::string, Shader*> ResourceManager::Shaders;
std::map<std::string, ShadingProgram*> ResourceManager::ShadingPrograms;
std::map<std::string, Model*> ResourceManager::Models;

void ResourceManager::LoadTexture(std::string TextureName)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    Texture *texture = new Texture(TextureName);
    RS->Textures.insert(std::pair<std::string,Texture*>(TextureName,texture));
}

void ResourceManager::LoadShader(std::string ShaderName, GLenum type)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    Shader* shader = new Shader(ShaderName,type);
    RS->Shaders.insert(std::pair<std::string, Shader*>(ShaderName, shader));
}

ResourceManager* ResourceManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return instance;
}

Texture* ResourceManager::GetTexture(std::string TextureName)
{
    if ((Textures.count(TextureName) <= 0)) {
        ResourceManager::LoadTexture(TextureName);
    }
    if ((Textures.count(TextureName) <= 0)) {
        std::cout << "UNABLE TO LOAD TEXTURE" << std::endl;
        return nullptr;
    }
    Texture* texture = Textures.at(TextureName);
    return texture;
}

Shader* ResourceManager::GetShader(std::string ShaderName,GLenum type)
{
    if ((Shaders.count(ShaderName) <= 0)){
        ResourceManager::LoadShader(ShaderName, type);
    }
    if ((Shaders.count(ShaderName) <= 0)) {
        std::cout << "UNABLE TO LOAD SHADER" << std::endl;
        return nullptr;
    }
    Shader* shader = Shaders.at(ShaderName);
    shader->LoadResource();
    return shader;
}

void ResourceManager::AddShadingProgram(std::string name, std::string vertexFileNamePath, std::string fragmentFileNamePah)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    ShadingProgram* shadingProgram = new ShadingProgram(vertexFileNamePath, fragmentFileNamePah);
    RS->ShadingPrograms.insert(std::pair<std::string, ShadingProgram*>(name, shadingProgram));
}

ShadingProgram* ResourceManager::GetShadingProgram(std::string name)
{
    if (!(ShadingPrograms.count(name) <= 0)) {
        return ShadingPrograms.at(name);
    }
    return nullptr;
}

void ResourceManager::AddModel(const std::string Name, Model* model)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    Model* model_ = new Model(model);
    RS->Models.insert(std::pair<std::string, Model*>(Name,model_));
}

Model* ResourceManager::GetModel(const std::string Name)
{
    if (!(Models.count(Name) <= 0)) {
        return Models.at(Name);
    }
    return nullptr;
}

void ResourceManager::FreeResources()
{
    for (auto const& x : Textures)
    {
        x.second->~Texture();
    }

    for (auto const& x : ShadingPrograms)
    {
        x.second->~ShadingProgram();
    }

    for (auto const& x : Shaders)
    {
        x.second->~Shader();
    }

    for (auto const& x : Models)
    {
        x.second->~Model();
    }
}
