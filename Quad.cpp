#include "Quad.h"

Face Quad::FrontFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)), //right top
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3 // right top -> right bottom -> left top
);

Face Quad::BackFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Quad::TopFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Quad::BottomFace = Face(
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Quad::RightFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Quad::LeftFace = Face(
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

void Quad::AddFace(Face* face)
{
	this->AddIndices(face->indices, 6);
	this->AddVertices(face->vertices, 4);
}
