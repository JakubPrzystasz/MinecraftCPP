#pragma once
#include <array>
#include "Model.h"

struct FaceTexture {
	GLuint Column;
	GLuint Row;
	FaceTexture(GLuint col, GLuint row) {
		Column = col;
		Row = row;
	}
};

enum class FaceName{
	Front,
	Back,
	Top,
	Bottom,
	Left,
	Right
};

struct Face {
	Vertex vertices[4];
	GLuint indices[6];
	Face(Vertex vert1, Vertex vert2, Vertex vert3, Vertex vert4, GLuint ind1, GLuint ind2, GLuint ind3, GLuint ind4, GLuint ind5, GLuint ind6) {
		vertices[0] = vert1;
		vertices[1] = vert2;
		vertices[2] = vert3;
		vertices[3] = vert4;
		indices[0] = ind1;
		indices[1] = ind2;
		indices[2] = ind3;
		indices[3] = ind4;
		indices[4] = ind5;
		indices[5] = ind6;
	}
	Face() {};
};

enum class BlockName {
	Air,
	Grass,
	Dirt,
	Cobble,
	Stone
};


class Cube : public Model {
private:
	GLuint faceTextureSize;
	GLuint textureWidth;
public:
	Cube();

	static Face FrontFace; 
	static Face BackFace; 
	static Face TopFace; 
	static Face BottomFace; 
	static Face RightFace; 
	static Face LeftFace; 

	std::map<FaceName, Face> Faces;
	std::map<FaceName, FaceTexture> FaceTexturesIndex;
	
	/// <summary>
	/// Bind desired face
	/// </summary>
	/// <param name="face"></param>
	void BindFace(const Face* face);

	/// <summary>
	/// Bind all faces 
	/// </summary>
	void BindFaces();

	Face GetFaceTexture(Face& face, FaceTexture fTex);
	
	void SetFaceTexture(Face& face, FaceTexture fTex);
};
