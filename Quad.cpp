#include "Quad.h"

Face Quad::FrontFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	1, 2, 3, 0, 1, 3
);

Face Quad::BackFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	1, 2, 3, 0, 1, 3
);

Face Quad::TopFace = Face(
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	3, 2, 0, 1, 3, 0
);

Face Quad::BottomFace = Face(
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	3, 2, 0, 1, 3, 0
);

Face Quad::RightFace = Face(
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	3, 2, 0, 1, 3, 0
);

Face Quad::LeftFace = Face(
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f)),
	3, 2, 0, 1, 3, 0
);

void Quad::AddFace(Face* face)
{
	this->AddIndices(face->indices, 6);
	this->AddVertices(face->vertices, 4);
}
