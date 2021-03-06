#include "Cube.h"

Face Cube::FrontFace = Face(
	Vertex(glm::vec3(1, 1, 1), glm::vec2(1.0f, 1.0f), glm::vec3(0, 0, 1)), //right top
	Vertex(glm::vec3(1, 0, 1), glm::vec2(1.0f, 0.0f), glm::vec3(0, 0, 1)),//right bottom
	Vertex(glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f), glm::vec3(0, 0, 1)),//left bottom
	Vertex(glm::vec3(0, 1, 1), glm::vec2(0.0f, 1.0f), glm::vec3(0, 0, 1)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3 // right top -> right bottom -> left top
);

Face Cube::BackFace = Face(
	Vertex(glm::vec3(1, 1, 0), glm::vec2(0.0f, 1.0f), glm::vec3(0, 0, -1)),//left top
	Vertex(glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f), glm::vec3(0, 0, -1)),//left bottom
	Vertex(glm::vec3(0, 0, 0), glm::vec2(1.0f, 0.0f), glm::vec3(0, 0, -1)),//right bottom
	Vertex(glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f), glm::vec3(0, 0, -1)),//right top
	3, 2, 1, // right bottom -> left bottom -> left top 
	3, 1, 0  // right top -> right bottom -> left top
);

Face Cube::TopFace = Face(
	Vertex(glm::vec3(1, 1, 0), glm::vec2(1.0f, 1.0f), glm::vec3(0, 1, 0)),//right top
	Vertex(glm::vec3(1, 1, 1), glm::vec2(1.0f, 0.0f), glm::vec3(0, 1, 0)),//right bottom
	Vertex(glm::vec3(0, 1, 1), glm::vec2(0.0f, 0.0f), glm::vec3(0, 1, 0)),//left bottom
	Vertex(glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f), glm::vec3(0, 1, 0)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::BottomFace = Face(
	Vertex(glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f), glm::vec3(0, -1, 0)),//right top
	Vertex(glm::vec3(1, 0, 1), glm::vec2(0.0f, 0.0f), glm::vec3(0, -1, 0)),//right bottom
	Vertex(glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f), glm::vec3(0, -1, 0)),//left bottom
	Vertex(glm::vec3(0, 0, 0), glm::vec2(1.0f, 1.0f), glm::vec3(0, -1, 0)),//left top
	3, 2, 1, // right bottom -> left bottom -> left top 
	3, 1, 0  // right top -> right bottom -> left top
);

Face Cube::RightFace = Face(
	Vertex(glm::vec3(1, 1, 0), glm::vec2(1.0f, 1.0f), glm::vec3(1, 0, 0)),//right top
	Vertex(glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f), glm::vec3(1, 0, 0)),//right bottom
	Vertex(glm::vec3(1, 0, 1), glm::vec2(0.0f, 0.0f), glm::vec3(1, 0, 0)),//left bottom
	Vertex(glm::vec3(1, 1, 1), glm::vec2(0.0f, 1.0f), glm::vec3(1, 0, 0)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Face Cube::LeftFace = Face(
	Vertex(glm::vec3(0, 1, 1), glm::vec2(1.0f, 1.0f), glm::vec3(-1, 0, 0)),//right top
	Vertex(glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f), glm::vec3(-1, 0, 0)),//right bottom
	Vertex(glm::vec3(0, 0, 0), glm::vec2(0.0f, 0.0f), glm::vec3(-1, 0, 0)),//left bottom
	Vertex(glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f), glm::vec3(-1, 0, 0)),//left top
	1, 2, 3, // right bottom -> left bottom -> left top 
	0, 1, 3  // right top -> right bottom -> left top
);

Cube::Cube()
{
	faceTextureSize = 128;
	textureWidth = 2048;
	Faces.insert(std::pair<FaceName, Face>(FaceName::Front, FrontFace));
	Faces.insert(std::pair<FaceName, Face>(FaceName::Back, BackFace));
	Faces.insert(std::pair<FaceName, Face>(FaceName::Top, TopFace));
	Faces.insert(std::pair<FaceName, Face>(FaceName::Bottom, BottomFace));
	Faces.insert(std::pair<FaceName, Face>(FaceName::Left, LeftFace));
	Faces.insert(std::pair<FaceName, Face>(FaceName::Right, RightFace));

	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Front, FaceTexture(0, 0)));
	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Back, FaceTexture(0, 0)));
	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Top, FaceTexture(0, 0)));
	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Bottom, FaceTexture(0, 0)));
	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Left, FaceTexture(0, 0)));
	FaceTexturesIndex.insert(std::pair<FaceName, FaceTexture>(FaceName::Right, FaceTexture(0, 0)));

}

void Cube::BindFace(const Face* face)
{
	this->AddIndices((GLuint*)face->indices, 6);
	this->AddVertices((Vertex*)face->vertices, 4);
}

void Cube::BindFaces()
{
	for (auto& face : Faces) {
		this->BindFace(&face.second);
	}
}

Face Cube::GetFaceTexture(Face& face, FaceTexture fTex)
{
	Face _face = Face(face);
	SetFaceTexture(_face, fTex);
	return _face;
}

std::string Cube::GetBlockName(BlockName block)
{
	switch (block) {
	case BlockName::Air:
		return std::string("Air");
	case BlockName::Grass:
		return std::string("Grass");
	case BlockName::Dirt:
		return std::string("Dirt");
	case BlockName::Cobble:
		return std::string("Cobble");
	case BlockName::Stone:
		return std::string("Stone");
	case BlockName::Wood:
		return std::string("Wood");
	case BlockName::Leave:
		return std::string("Leaves");
	case BlockName::Sand:
		return std::string("Sand");
	case BlockName::Workbench:
		return std::string("Workbench");
	case BlockName::Plank:
		return std::string("Plank");
	case BlockName::TNT:
		return std::string("TNT");
	case BlockName::Glass:
		return std::string("Glass");
	}
	return std::string("");
}

bool Cube::IsTransparent(BlockName block)
{
	switch (block) {
	case BlockName::Air:
	case BlockName::Leave:
	case BlockName::Glass:
		return true;
	default:
		return false;
	}
}

void Cube::SetFaceTexture(Face& face, FaceTexture fTex)
{
	GLfloat pixelSize = 1.f / textureWidth;
	GLfloat texSize = pixelSize * faceTextureSize;
	glm::vec2 base = glm::vec2(texSize * fTex.Column, texSize * fTex.Row);
	face.vertices[0].TexCoords = base + texSize * face.vertices[0].TexCoords;
	face.vertices[1].TexCoords = base + texSize * face.vertices[1].TexCoords;
	face.vertices[2].TexCoords = base + texSize * face.vertices[2].TexCoords;
	face.vertices[3].TexCoords = base + texSize * face.vertices[3].TexCoords;
}
