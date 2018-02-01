#pragma once
#include "Texture.h"
#include "UiElement.h"

#include <unordered_map>

class StringRenderer
{
public:
	StringRenderer();
	virtual ~StringRenderer();

	static bool Init(const std::string & tex_id, uint32_t w, uint32_t h);
	static void DrawText(float top, float left, uint32_t font_size, const std::string & text);

private:
	static GLuint m_tex_vbo;
	static Texture * m_font_texture;
	static float m_block_w;
	static float m_block_h;

	static UiElement m_quad;
	static std::unordered_map<char, Vector2> m_symbols_pos;
};

