#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <map>

#include "Texture.h"
#include "ShadingProgram.h"
#include "Camera.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	Vertex(glm::vec3 pos, glm::vec2 tex) {
		Position = pos;
		TexCoords = tex;
	}
	Vertex() {};
};

class Model
{
public:

	Model();
	Model(Model* model);

	GLuint VBO = 0, VAO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	ShadingProgram *shadingProgram;
	std::map<std::string, Texture*> Textures;

	void SetShadingProgram(ShadingProgram *sp);
	void AddTexture(std::string name, Texture* tex);

	void SetVertices(std::vector<Vertex>& vertices);
	void SetVertices(std::vector<glm::vec3>& vertices);
	void SetVertices(GLfloat *vertices, GLuint length);

	void SetTexturePosition(GLfloat* texPos, GLuint length);

	void SetIndicies(std::vector<GLuint>& indices);
	void SetIndicies(GLuint *indices, GLuint length);
	void AddIndices(GLuint* indices, GLuint length);
	void AddVertex(Vertex* vert);
	void AddVertices(Vertex* vert, GLuint length);

	/// <summary>
	/// Bind model data to GLContext
	/// </summary>
	void BindData();

	/// <summary>
	/// Initialize object's VAO,ABO,EBO
	/// </summary>
	void Init();

	/// <summary>
	/// Draw object
	/// </summary>
	void Draw();
};

