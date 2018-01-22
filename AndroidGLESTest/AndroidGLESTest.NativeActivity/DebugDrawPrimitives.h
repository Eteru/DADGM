#pragma once

#include "ResourceManager.h"

class DebugDrawPrimitives
{
public:
	virtual ~DebugDrawPrimitives();

	static void SetDebugSettings(DebugSettings ds);

	static void DrawLine(Vector3 begin, Vector3 end, const Vector4 & color);
	static void DrawCube(Vector3 pos, Vector3 rot, Vector3 scale, const Vector4 & color);
	static void DrawSphere(Vector3 pos, Vector3 rot, Vector3 scale, const Vector4 & color);

	static const Vector4 COLOR_RED;
	static const Vector4 COLOR_GREEN;
	static const Vector4 COLOR_BLUE;
	static const Vector4 COLOR_YELLOW;

private:
	static bool m_init;
	static GLuint m_line_vbo, m_line_ibo;
	static DebugSettings m_settings;

	static void Draw(GLint vbo, GLint ibo, GLint iboCount, Matrix & M, const Vector4 & color);
	
protected:
	DebugDrawPrimitives();
};

