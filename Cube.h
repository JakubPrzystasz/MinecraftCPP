#pragma once
#include <array>
#include "Model.h"

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
};

class Cube : public Model {
public:
	static Face FrontFace; 
	static Face BackFace; 
	static Face TopFace; 
	static Face BottomFace; 
	static Face RightFace; 
	static Face LeftFace; 
	std::array<Face, 6> Faces = {FrontFace,BackFace,TopFace,BottomFace,RightFace,LeftFace};
	
	/// <summary>
	/// Bind desired face
	/// </summary>
	/// <param name="face"></param>
	void BindFace(const Face* face);

	/// <summary>
	/// Bind all faces 
	/// </summary>
	void Bind();
};
