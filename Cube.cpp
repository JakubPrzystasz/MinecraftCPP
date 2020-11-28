#include "Cube.h"

Face Cube::FrontFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)), //right top
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3 // right top -> right bottom -> left top
);

Face Cube::BackFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::TopFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::BottomFace = Face(
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::RightFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::LeftFace = Face(
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)),//right top
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)),//right bottom
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),//left bottom
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

void Cube::BindFace(const Face* face)
{
	this->AddIndices((GLuint*)face->indices, 6);
	this->AddVertices((Vertex*)face->vertices, 4);
}

void Cube::Bind()
{
	for (const Face& face : Faces) {
		this->BindFace(&face);
	}
}
