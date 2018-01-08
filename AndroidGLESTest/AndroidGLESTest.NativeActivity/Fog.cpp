
#include "Fog.h"

Fog::Fog()
	: m_r(0.f), m_R(0.f), m_color(Vector3(0,0,0))
{
}

Fog::Fog(float r, float R, Vector3 & color)
	: m_r(r), m_R(R), m_color(color)
{
}

Fog::~Fog()
{
}

void Fog::SetValues(float r, float R, const Vector3 & color)
{
	m_r = r;
	m_R = R;
	m_color = color;
}

float Fog::ComputeAlpha(const float distance) const
{
	if (distance < m_r) {
		return 0.f;
	} // No need to check over m_R, using clamp in shader

	return (distance - m_r) / (m_R - m_r);
}
