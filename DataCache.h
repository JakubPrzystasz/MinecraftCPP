#pragma once
#include <string>

/// <summary>
/// Base class for all resources 
/// </summary>
class DataCache
{
protected:
	/// <summary>
	/// Stores file name path
	/// </summary>
	std::string fileNamePath = "";
	
	/// <summary>
	/// Stores size of file
	/// </summary>
	unsigned long dataSize = 0;
	
	/// <summary>
	/// Points to file stored in memory
	/// </summary>
	void* data = nullptr;

	/// <summary>
	/// Virtual function 
	/// </summary>	
	virtual int LoadResource() = 0;

	/// <summary>
	/// Virtual function 
	/// </summary>	
	virtual int FreeResource() = 0;
};

