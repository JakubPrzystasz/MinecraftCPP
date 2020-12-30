#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;
std::map<std::string, Texture*> ResourceManager::Textures;
std::map<std::string, Shader*> ResourceManager::Shaders;
std::map<std::string, ShadingProgram*> ResourceManager::ShadingPrograms;
std::map<std::string, Model*> ResourceManager::Models;
std::map<BlockName, Cube*> ResourceManager::Blocks;

void ResourceManager::LoadTexture(std::string TextureName)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    if (RS->Textures.count(TextureName) > 0)
        return;
    Texture *texture = new Texture(TextureName);
    RS->Textures.insert(std::pair<std::string,Texture*>(TextureName,texture));
}

void ResourceManager::LoadShader(std::string ShaderName, GLenum type)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    if (RS->Shaders.count(ShaderName) > 0)
        return;
    Shader* shader = new Shader(ShaderName,type);
    RS->Shaders.insert(std::pair<std::string, Shader*>(ShaderName, shader));
}

ResourceManager* ResourceManager::GetInstance()
{
    if (instance == nullptr)
        instance = new ResourceManager();
    return instance;
}

Texture* ResourceManager::GetTexture(std::string TextureName)
{
    if ((Textures.count(TextureName) <= 0))
        ResourceManager::LoadTexture(TextureName);
    Texture* texture = Textures.at(TextureName);
    return texture;
}

Shader* ResourceManager::GetShader(std::string ShaderName,GLenum type)
{
    if ((Shaders.count(ShaderName) <= 0))
        ResourceManager::LoadShader(ShaderName, type);
    Shader* shader = Shaders.at(ShaderName);
    shader->LoadResource();
    return shader;
}

void ResourceManager::AddShadingProgram(std::string name, std::string vertexFileNamePath, std::string fragmentFileNamePah)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    if (RS->Shaders.count(name) > 0)
        return;
    ShadingProgram* shadingProgram = new ShadingProgram(vertexFileNamePath, fragmentFileNamePah);
    RS->ShadingPrograms.insert(std::pair<std::string, ShadingProgram*>(name, shadingProgram));
}

ShadingProgram* ResourceManager::GetShadingProgram(std::string name)
{
    if (!(ShadingPrograms.count(name) <= 0))
        return ShadingPrograms.at(name);
    return nullptr;
}

void ResourceManager::AddModel(const std::string Name, Model* model)
{
    ResourceManager* RS = ResourceManager::GetInstance();
    if (RS->Models.count(Name) > 0)
        return;
    Model* model_ = new Model(model);
    RS->Models.insert(std::pair<std::string, Model*>(Name,model_));
}

Model* ResourceManager::GetModel(const std::string Name)
{
    if (!(Models.count(Name) <= 0))
        return Models.at(Name);
    return nullptr;
}

void ResourceManager::AddBlock(BlockName blockName, unsigned int faceTextureIndex[12])
{
    ResourceManager* RS = ResourceManager::GetInstance();
    if (RS->Blocks.count(blockName) > 0)
        return;
    auto block = new Cube();
    RS->Blocks.insert(std::pair<BlockName, Cube*>(blockName, block));
    RS->Blocks[blockName]->Init();
    RS->Blocks[blockName]->SetShadingProgram(RS->GetShadingProgram("block"));
    RS->Blocks[blockName]->AddTexture("face", RS->GetTexture("Textures/terrain.png"));
    RS->Blocks[blockName]->shadingProgram->Use();
    RS->Blocks[blockName]->Textures["face"]->Init();
    RS->Blocks[blockName]->shadingProgram->SetData("texture1", RS->Blocks[blockName]->Textures["face"]->GetId());
    RS->Blocks[blockName]->Textures["face"]->Bind(GL_TEXTURE1);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[0], faceTextureIndex[0], faceTextureIndex[1]);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[1], faceTextureIndex[2], faceTextureIndex[3]);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[2], faceTextureIndex[4], faceTextureIndex[5]);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[3], faceTextureIndex[6], faceTextureIndex[7]);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[4], faceTextureIndex[8], faceTextureIndex[9]);
    RS->Blocks[blockName]->SetFaceTexture(RS->Blocks[blockName]->Textures["face"], RS->Blocks[blockName]->Faces[5], faceTextureIndex[10], faceTextureIndex[11]);
}

Cube* ResourceManager::GetBlock(BlockName blockName)
{
    if (!(Blocks.count(blockName) <= 0))
        return Blocks.at(blockName);
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
