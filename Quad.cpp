#include "Quad.h"

void Quad::SetFaces()
{
	SetFace(frontFace);
	BindData();
}

void Quad::SetFace(face face_)
{
	SetIndicies((GLuint*)face_.indices, 6);
	SetVertices((GLfloat*)face_.vertices, 12);
	SetTexturePosition((GLfloat*)face_.texturePos, 8);
}
