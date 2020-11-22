#include "DataCache.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

/// <summary>
/// Class handling operations over GLSL Shader file 
/// </summary>
class ShaderCache:
	protected DataCache
{
private:
    /// <summary>
    /// If property is true, the resoruce is stored in memory 
    /// </summary>
    bool inMemory = false;

public:

    /// <summary>
    /// Constructor for ShaderCache class, file name is required
    /// </summary>
    /// <param name="fileNamePath">Path to resource file</param>
    ShaderCache(std::string fileNamePath);

    /// <summary>
    /// Tell compiler that this is default constructor
    /// </summary>
    ShaderCache() = default;

    /// <summary>
    /// Removes resources from memory (if exist in it), then destroys an object
    /// </summary>
    ~ShaderCache();

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
};

