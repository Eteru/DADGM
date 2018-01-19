#include "TimeLerp.h"

Vec3TimeLerp::Vec3TimeLerp(Vector3 start, Vector3 end)
{
	m_start = start;
	m_end = end;
	m_crtCoef = 0.f;
}

Vector3 Vec3TimeLerp::GetValue()
{
	return Math::Lerp(m_start, m_end, m_crtCoef);
}

Vector3 Vec3TimeLerp::Step(const GLfloat t)
{
	m_crtCoef += t;

	return Math::Lerp(m_start, m_end, m_crtCoef);
}

FloatTimeLerp::FloatTimeLerp(GLfloat start, GLfloat end)
{
	m_start = start;
	m_end = end;
	m_crtCoef = 0.f;
}

GLfloat FloatTimeLerp::Step(const GLfloat t)
{
	m_crtCoef += t;

	return Math::Lerp(m_start, m_end, m_crtCoef);
}

GLfloat FloatTimeLerp::GetValue()
{
	return Math::Lerp(m_start, m_end, m_crtCoef);
}
