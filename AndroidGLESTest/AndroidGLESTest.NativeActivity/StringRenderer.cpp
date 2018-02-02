
#include "StringRenderer.h"
#include "ResourceManager.h"

Texture *StringRenderer::m_font_texture = nullptr;
float StringRenderer::m_block_w = 16;
float StringRenderer::m_block_h = 16;
UiElement StringRenderer::m_quad = UiElement();
std::unordered_map<char, Vector2> StringRenderer::m_symbols_pos;
uint32_t StringRenderer::m_w = 0;
uint32_t StringRenderer::m_h = 0;



StringRenderer::StringRenderer()
{
}


StringRenderer::~StringRenderer()
{
}

bool StringRenderer::Init(const std::string & tex_id, uint32_t w, uint32_t h)
{
	m_font_texture = ResourceManager::GetInstance()->LoadTexture(tex_id);
	if (nullptr == m_font_texture)
		return false;

	m_w = w;
	m_h = h;
	m_block_w /= w;
	m_block_h /= h;

	m_quad = UiElement(0.f, 0.f, m_block_w, m_block_h, {});

	std::vector<GLfloat> texcoords = {
		0.f, 0.f,
		0.f, 1.f / 16.f,
		1.f / 16.f, 0.f,
		1.f / 16.f, 1.f / 16.f,
	};

	m_quad.SetTexCoords(texcoords);

	std::string characters = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz";

	int col = 0, row = 12;
	for (char c : characters)
	{
		m_symbols_pos[c] = Vector2(col / 16.f, row / 16.f);

		++col;
		if (16 == col)
		{
			col = 0;
			--row;
		}
	}

	m_quad.Init();
	m_quad.SetShader("5");

	return true;
}

void StringRenderer::DrawText(float top, float left, uint32_t font_size, const Vector3 & color, const std::string & text)
{
	glUseProgram(m_quad.GetShader()->GetProgramID());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_font_texture->GetTextureType(), m_font_texture->GetID());

	// scale matrix doesnt work ......
	GLfloat screenTop = Math::ChangeInterval(0, m_h, -1.f, 1.f, top);
	GLfloat screenLeft = Math::ChangeInterval(0, m_w, -1.f, 1.f, left);
	Matrix S = Matrix().SetScale(font_size, font_size, 1.f);

	for (const char c : text)
	{
		if (c != ' ')
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_quad.GetVBO());

			Shaders objShader = m_quad.GetShader()->GetShaderData();
			if (objShader.positionAttribute != -1)
			{
				glEnableVertexAttribArray(objShader.positionAttribute);
				glVertexAttribPointer(objShader.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_quad.GetTexVBO());

			if (objShader.uvAttribute != -1)
			{
				glEnableVertexAttribArray(objShader.uvAttribute);
				glVertexAttribPointer(objShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}

			if (objShader.matrixUniform != -1)
			{
				Matrix M = S * Matrix().SetTranslation(screenLeft, screenTop, 0);
				glUniformMatrix4fv(objShader.matrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(M.m));
			}

			if (objShader.uvOffsetUniform != -1)
			{
				Vector2 tc = m_symbols_pos[c];
				glUniform2fv(objShader.uvOffsetUniform, 1, &tc.x);
			}

			if (objShader.colorUniform != -1)
			{
				glUniform4fv(objShader.colorUniform, 1, &color.x);
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		screenLeft += (m_block_h * font_size * 0.5);
	}
}
