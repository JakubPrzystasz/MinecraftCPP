#include "Model.h"

Model::Model()
{
	VBO = 0;
	VAO = 0;
	EBO = 0;
}

Model::Model(Model* model)
{
	this->EBO = model->EBO;
	this->VAO = model->VAO;
	this->VBO = model->VBO;
	this->indices = std::vector<GLuint>(model->indices);
	this->vertices = std::vector<Vertex>(model->vertices);
}

void Model::SetShadingProgram(ShadingProgram* sp)
{
	shadingProgram = sp;
}

void Model::AddTexture(std::string name, Texture* tex)
{
	Textures[name] = tex;
	shadingProgram->SetData(name.c_str(), tex->GetId());
}

void Model::SetVertices(std::vector<glm::vec3>& vertices)
{
	this->vertices.clear();
	Vertex tmp = Vertex();
	for (auto& i : vertices) {
		tmp.TexCoords = glm::vec2(0);
		tmp.Position = i;
		this->vertices.push_back(tmp);
	}
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	Textures.clear();
	indices.clear();
	vertices.clear();
}

void Model::SetVertices(GLfloat* vertices, GLuint length)
{
	//Assume that every vertex has 3 values in array
	if (length % 3 == 0) {
		this->vertices.clear();
		std::vector<glm::vec3> verts;
		glm::vec3 tmp;
		for (GLuint i = 0; i < length; i += 3) {
			tmp.x = vertices[i];
			tmp.y = vertices[i + 1];
			tmp.z = vertices[i + 2];
			verts.push_back(tmp);
		}
		SetVertices(verts);
	}
}


void Model::SetTexturePosition(GLfloat* texPos, GLuint length)
{
	for (GLuint i = 0; i < vertices.size(); i++) {
		if (i < length - 1) {
			vertices[i].TexCoords.x = texPos[i];
			vertices[i].TexCoords.y = texPos[i + 1];
		}
	}

}

void Model::SetIndicies(std::vector<GLuint>& indices)
{
	this->indices.clear();
	this->indices = std::vector<GLuint>(indices);
}

void Model::SetIndicies(GLuint* indices, GLuint length)
{
	this->indices.clear();
	this->indices = std::vector<GLuint>(indices, indices + length);
}

void Model::AddIndices(GLuint* indices, GLuint length)
{
	std::vector<GLuint> tmp = std::vector<GLuint>(indices, indices + length);
	GLuint countVertices = this->vertices.size();
	for (GLuint i = 0; i < tmp.size(); i++) {
		this->indices.push_back(tmp[i] + countVertices);
	}
}

void Model::AddVertex(Vertex* vert)
{
	Vertex tmp = Vertex(*vert);
	vertices.push_back(tmp);
}

void Model::AddVertex(Vertex vert)
{
	Vertex tmp = Vertex(vert);
	vertices.push_back(tmp);
}

void Model::AddVertices(Vertex* vert, GLuint length)
{
	for (GLuint i = 0; i < length; i++) {
		AddVertex(vert + i);
	}
}

void Model::ClearVertexData()
{
	indices.clear();
	vertices.clear();
}

void Model::BindData()
{
	if (vertices.size() == 0)
		return;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// Vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex texture pos
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Model::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Model::Draw()
{
	if (vertices.size() < 1 || VBO == 0)
		return;

	shadingProgram->Use();

	GLuint i = 0;
	for (auto& texture : Textures) {
		texture.second->Bind(i);
		shadingProgram->SetData(texture.first.c_str(), i++);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}
