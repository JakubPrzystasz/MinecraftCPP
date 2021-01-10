#include "Text.h"

void Text::Init() {

	auto rs = ResourceManager::GetInstance();

	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	// find path to font
	std::string font_name = "Fonts/Antonio-Bold.ttf";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			break;

		Character *tmpCharacter = new Character();
		tmpCharacter->Init();
		tmpCharacter->Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		tmpCharacter->Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		tmpCharacter->Advance = static_cast<unsigned int>(face->glyph->advance.x);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tmpCharacter->texture);
		glBindTexture(GL_TEXTURE_2D, tmpCharacter->texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		tmpCharacter->SetShadingProgram(rs->GetShadingProgram("text"));
		rs->AddCharacter((char)c, tmpCharacter);
	}


}

void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	auto rs = ResourceManager::GetInstance();
	rs->GetShadingProgram("text")->Use();
	rs->GetShadingProgram("text")->SetData("textColor", color);
	rs->GetShadingProgram("text")->SetData("glyph", 0);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
	glUniformMatrix4fv(glGetUniformLocation(rs->GetShadingProgram("text")->GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	
	for (char const & c:text)
	{
		Character *ch = rs->GetCharacter(c);
		if (ch == nullptr)
			continue;

		float xpos = x + ch->Bearing.x * scale;
		float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

		float w = ch->Size.x * scale;
		float h = ch->Size.y * scale;

		GLuint indicies[6] = { 0,1,3,0,3,2 };
		ch->ClearVertexData();
		ch->AddIndices(indicies, 6);

		ch->AddVertex(Vertex(glm::vec3(xpos, ypos + h, 0), glm::vec2(0, 0)));
		ch->AddVertex(Vertex(glm::vec3(xpos, ypos, 0), glm::vec2(0, 1)));
		ch->AddVertex(Vertex(glm::vec3(xpos + w, ypos + h, 0), glm::vec2(1, 0)));
		ch->AddVertex(Vertex(glm::vec3(xpos + w, ypos, 0), glm::vec2(1, 1)));

		glBindTexture(GL_TEXTURE_2D, ch->texture);

		ch->BindData();
		ch->Draw();

		x += (ch->Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

}
