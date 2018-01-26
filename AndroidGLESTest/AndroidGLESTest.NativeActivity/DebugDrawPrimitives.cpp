
#include "DebugDrawPrimitives.h"
#include "SceneManager.h"

const Vector4 DebugDrawPrimitives::COLOR_RED = Vector4(1.f, 0.f, 0.f, 1.f);
const Vector4 DebugDrawPrimitives::COLOR_GREEN = Vector4(0.f, 1.f, 0.f, 1.f);
const Vector4 DebugDrawPrimitives::COLOR_BLUE = Vector4(0.f, 0.f, 1.f, 1.f);
const Vector4 DebugDrawPrimitives::COLOR_YELLOW = Vector4(1.f, 1.f, 0.f, 1.f);

bool DebugDrawPrimitives::m_init = false;
DebugSettings DebugDrawPrimitives::m_settings = {};

GLuint DebugDrawPrimitives::m_line_vbo = UINT_MAX;
GLuint DebugDrawPrimitives::m_line_ibo = UINT_MAX;

DebugDrawPrimitives::DebugDrawPrimitives()
{
}

DebugDrawPrimitives::~DebugDrawPrimitives()
{
}

void DebugDrawPrimitives::SetDebugSettings(DebugSettings ds)
{
	m_settings.cube_id = ds.cube_id;
	m_settings.sphere_id = ds.sphere_id;
	m_settings.shader_id = ds.shader_id;

	m_init = true;
}

void DebugDrawPrimitives::DrawLine(Vector3 begin, Vector3 end, const Vector4 & color)
{
	if (false == m_init)
	{
		return;
	}

	std::vector<Vertex> verts(2);
	verts[0].pos = begin;
	verts[1].pos = end;
	std::vector<GLushort> indices = { 0, 1 };

	if (UINT_MAX == m_line_vbo)
	{
		glGenBuffers(1, &m_line_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, &verts[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Generate and create ibo for filled mesh
		glGenBuffers(1, &m_line_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_line_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 2, &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 2, &verts[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	

	static Matrix identityMatrix = Matrix().SetIdentity();
	Draw(m_line_vbo, m_line_ibo, 2, identityMatrix, color);
}

void DebugDrawPrimitives::DrawAAB(const Vector3 pos, const Vector3 halfExtents, const Vector4 &color)
{
	DrawCube(pos, Vector3(0.f), halfExtents, color);
}

void DebugDrawPrimitives::DrawSphere(const Vector3 pos, const float radius, const Vector4 &color)
{
	DrawSphere(pos, Vector3(0.f), Vector3(radius), color);
}

void DebugDrawPrimitives::DrawCube(Vector3 pos, Vector3 rot, Vector3 scale, const Vector4 & color)
{
	if (false == m_init)
	{
		return;
	}

	Model *cube = ResourceManager::GetInstance()->GetModel(m_settings.cube_id);

	Matrix P = Matrix().SetTranslation(pos);

	//scale *= 0.5f;
	Matrix S = Matrix().SetScale(scale);

	Matrix Rx = Matrix().SetRotationX(rot.x);
	Matrix Ry = Matrix().SetRotationY(rot.y);
	Matrix Rz = Matrix().SetRotationZ(rot.z);

	Matrix M = S * (Rx * Ry * Rz) * P;

	Draw(cube->GetVBO(), cube->GetIBO(true), cube->GetIBOCount(true), M, color);
}

void DebugDrawPrimitives::DrawSphere(Vector3 pos, Vector3 rot, Vector3 scale, const Vector4 & color)
{
	if (false == m_init)
	{
		return;
	}

	Model *sphere = ResourceManager::GetInstance()->GetModel(m_settings.sphere_id);

	Matrix P = Matrix().SetTranslation(pos);
	
	Matrix S = Matrix().SetScale(scale);

	Matrix Rx = Matrix().SetRotationX(rot.x);
	Matrix Ry = Matrix().SetRotationY(rot.y);
	Matrix Rz = Matrix().SetRotationZ(rot.z);

	Matrix M = S * (Rx * Ry * Rz) * P;

	Draw(sphere->GetVBO(), sphere->GetIBO(true), sphere->GetIBOCount(true), M, color);
}

void DebugDrawPrimitives::Draw(GLint vbo, GLint ibo, GLint iboCount, Matrix & M, const Vector4 & color)
{
	Shader *s = ResourceManager::GetInstance()->GetShader(m_settings.shader_id);
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	Shaders objShader = s->GetShaderData();

	glUseProgram(s->GetProgramID());
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	if (objShader.positionAttribute != -1) {
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (objShader.colorUniform != -1) {
		glUniform4fv(objShader.colorUniform, 1, &color.x);
	}

	if (objShader.matrixUniform != -1) {
		Matrix MVP = M * cam->GetView() * cam->GetProjection();
		glUniformMatrix4fv(objShader.matrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(MVP.m));
	}

	glDrawElements(
		GL_LINES,      // mode
		iboCount,    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
