#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Model
{
public:

	Model();
	Model(Model* model);

	GLuint VBO = 0, VAO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;


	void SetVertices(std::vector<Vertex>& vertices);
	void SetVertices(std::vector<glm::vec3>& vertices);
	void SetVertices(GLfloat *vertices, GLuint length);

	void SetIndicies(std::vector<GLuint>& indices);
	void SetIndicies(GLuint *indices, GLuint length);

	void BindData();

	void Draw();
};

