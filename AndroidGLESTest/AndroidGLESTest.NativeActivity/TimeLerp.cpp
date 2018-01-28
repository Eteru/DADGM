#include "TimeLerp.h"

Vec3TimeLerp::Vec3TimeLerp(Vector3 start, Vector3 end)
{
	m_start = start;
	m_end = end;
	m_crtCoef = 0.f;
	m_value = Math::Lerp(m_start, m_end, m_crtCoef);
}

Vector3 Vec3TimeLerp::GetValue() const
{
	return m_value;
}

Vector3 Vec3TimeLerp::Step(const GLfloat t)
{
	m_crtCoef += t;

	m_value = Math::Lerp(m_start, m_end, m_crtCoef);
	return m_value;
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

	m_value = Math::Lerp(m_start, m_end, m_crtCoef);
	return m_value;
}

GLfloat FloatTimeLerp::GetValue() const
{
	return m_value;
}
