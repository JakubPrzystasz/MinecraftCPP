#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <map>
#include "Vectors.h"
#include "Texture.h"
#include "ShadingProgram.h"
#include "Camera.h"

class Chunk;

struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	Vertex(glm::vec3 pos, glm::vec2 tex) {
		Position = pos;
		TexCoords = tex;
	}
	Vertex(vec3 pos, glm::vec2 tex) {
		Position = glm::vec3(pos.x,pos.y,pos.z);
		TexCoords = tex;
	}
	Vertex(vec3 pos, vec2 tex) {
		Position = glm::vec3(pos.x, pos.y, pos.z);
		TexCoords = glm::vec2(tex.x,tex.y);
	}
	Vertex(glm::vec3 pos, vec2 tex) {
		Position = pos;
		TexCoords = glm::vec2(tex.x, tex.y);
	}
	Vertex() {
		Position = glm::vec3(0);
		TexCoords = glm::vec2(0);
	};
};

class Model
{
public:

	Model();
	Model(Model* model);
	~Model();

	GLuint VBO, VAO, EBO;

	Chunk* chunk;

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
	void AddVertex(Vertex vert);
	void AddVertices(Vertex* vert, GLuint length);

	void ClearVertexData();

	/// <summary>
	/// Bind model data to GLContext
	/// </summary>
	void BindData();

	/// <summary>
	/// Initialize object's VAO,VBO,EBO
	/// </summary>
	void Init();

	/// <summary>
	/// Draw object
	/// </summary>
	void Draw();
};

