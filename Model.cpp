#include "Model.h"

Model::Model()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

Model::Model(Model* model)
{
	this->EBO = model->EBO;
	this->VAO = model->VAO;
	this->VBO = model->VBO;
	this->indices = std::vector<GLuint>(model->indices);
	this->vertices = std::vector<Vertex>(model->vertices);
}

void Model::SetShadingProgram(ShadingProgram *sp)
{
	shadingProgram = sp;
}

void Model::AddTexture(std::string name, Texture* tex)
{
	Textures[name] = tex;
}

void Model::SetVertices(std::vector<glm::vec3>& vertices)
{
	this->vertices.clear();
	Vertex tmp;
	for (auto& i : vertices) {
		tmp.Normal = glm::vec3(0);
		tmp.TexCoords = glm::vec2(0);
		tmp.Position = i;
		this->vertices.push_back(tmp);
	}
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
			tmp.y = vertices[i+1];
			tmp.z = vertices[i+2];
			verts.push_back(tmp);
		}
		SetVertices(verts);
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

void Model::BindData()
{

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// Vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex texture pos
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Model::Draw()
{
	if (vertices.size() < 1 || VBO == 0)
		std::cout << "ERROR:MODEL::NO DATA "<< std::endl;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
