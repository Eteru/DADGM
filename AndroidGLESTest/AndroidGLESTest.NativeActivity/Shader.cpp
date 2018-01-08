
#include "Shader.h"
#include <cstdlib>

Shader::Shader() : m_loaded(false), m_sr(nullptr)
{
}

Shader::Shader(ShaderResource * sr) : m_loaded(false), m_sr(sr)
{
	m_id = atoi(sr->id.c_str());
}

Shader::~Shader()
{
}

bool Shader::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_sr) {
		return false;
	}

	if (0 > m_shaders.Init(&m_sr->vs_path[0], &m_sr->fs_path[0])) {
		return false;
	}

	m_loaded = true;
	return true;
}