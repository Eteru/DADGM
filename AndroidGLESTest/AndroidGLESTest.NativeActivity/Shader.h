#pragma once

#include <string>
#include "Structs.h"
#include "Shaders.h"

class Shader
{
public:
	Shader();
	Shader(ShaderResource *sr);
	~Shader();

	bool Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLint GetProgramID() const
	{
		return m_shaders.program;
	}

	inline Shaders GetShaderData() const
	{
		return m_shaders;
	}

private:
	bool m_loaded;
	GLuint m_id;

	Shaders m_shaders;

	ShaderResource *m_sr;
};