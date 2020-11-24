#pragma once
#include "Model.h"

struct face {
	GLfloat vertices[12];
	GLfloat texturePos[8];
	GLuint indices[6];
};

class Quad: public Model {
private:

	const face frontFace =
	{
		//vert [12]
		//x      y      z
		 0.5f,  0.5f, 0.0f,   // right top
		 0.5f, -0.5f, 0.0f,   // right bottom
		-0.5f, -0.5f, 0.0f,   // left bottom
		-0.5f,  0.5f, 0.0f,   // left top
		//texture [8]
		//s    t
		1.0f, 1.0f,//0   	  // right top
		0.0f, 0.0f,//1  	  // right bottom
		1.0f, 0.0f,//2   	  // left bottom
		0.0f, 1.0f,//3        // left top
		//indices [6]
		1,2,3,0,1,3
	};

public:
	void SetFaces();
	void SetFace(face face_);
};
