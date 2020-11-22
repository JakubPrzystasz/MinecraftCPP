#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "DataCache.h"
#include "stb_image.h"

/// <summary>
/// Class handling operations over texture file 
/// </summary>
class TextureCache :
    protected DataCache
{
private:

    /// <summary>
    /// If property is true, the resoruce is stored in memory 
    /// </summary>
    bool inMemory = false;
    
    /// <summary>
    /// Stores actual width of texture
    /// </summary>
    int width = 0;

    /// <summary>
    /// Stores actual height of texture
    /// </summary>
    int height = 0;

    /// <summary>
    /// Stores number of color channels of texture file
    /// </summary>
    int colorChannels = 0;

public:
    /// <summary>
    /// Constructor for TextureCache class, file name is required
    /// </summary>
    /// <param name="fileNamePath">Path to resource file</param>
    TextureCache(std::string fileNamePath);
    
    /// <summary>
    /// Tell compiler that this is default constructor
    /// </summary>
    TextureCache() = default;
    
    /// <summary>
    /// Removes resources from memory (if exist in it), then destroys an object
    /// </summary>
    ~TextureCache();

    /// <summary>
    /// Handles file specified in property fileNamePath, then
    /// reads its data to memory pointed by data property.
    /// </summary>
    /// <returns>0 when succeeds, other value when fails</returns>
    int LoadResource();
    
    /// <summary>
    /// Removes data from memory
    /// </summary>
    /// <returns>0 when succeeds, other value when fails</returns>
    int FreeResource();

    /// <returns>Returns width of the texture</returns>
    GLuint GetWith();

    /// <returns>Returns width of the texture</returns>
    GLuint GetHeight();

    /// <returns>Returns number of color channels of the texture</returns>
    GLuint GetChannels();

    friend class Texture;
};

