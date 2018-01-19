
#include "LightSource.h"
#include "SceneManager.h"

LightSource::LightSource(float shininess, float diff_coef, float spec_coef, Vector3 diff_color, Vector3 spec_color)
	: m_shininess(shininess), m_diffuse_coef(diff_coef), m_specular_coef(spec_coef), m_diffuse_color(diff_color),
	m_specular_color(spec_color), m_spot_angle(0.f), m_type(NO_LIGHT), m_direction(Vector3(0, 0, 0))
{
	m_followedObject = nullptr;
	m_transform.m_lerp = false;
}

LightSource::~LightSource()
{
}

void LightSource::SetType(std::string type)
{
	if ("point" == type) {
		m_type = POINT_LIGHT;
	}
	else if ("directional" == type) {
		m_type = DIRECTIONAL_LIGHT;
	}
	else if ("spot" == type) {
		m_type = SPOT_LIGHT;
	}
}

void LightSource::SetPosition(Vector3 pos)
{
	m_transform.m_pos = pos;
}

void LightSource::SetDirection(Vector3 dir)
{
	m_direction = dir;
}

void LightSource::SetSpotAngle(float angle)
{
	m_spot_angle = angle;
}

Vector3 LightSource::GetPosition()
{
	return m_transform.GetPosition();
}

Vector3 LightSource::GetDirection()
{
	return m_direction;
}

Vector3 LightSource::GetSpecularLight() const
{
	return m_specular_color;
}

Vector3 LightSource::GetDiffuseLight() const
{
	return m_diffuse_color;
}

float LightSource::GetSpecularCoefficient() const
{
	return m_specular_coef;
}

float LightSource::GetDiffuseCoefficient() const
{
	return m_diffuse_coef;
}

float LightSource::GetShininess() const
{
	return m_shininess;
}

float LightSource::GetSpotAngle() const
{
	return m_spot_angle;
}

LightSource::LightType LightSource::GetType() const
{
	return m_type;
}

void LightSource::Update()
{
	if (nullptr != m_followedObject)
	{
		VisualBody *so = dynamic_cast<VisualBody *>(m_followedObject);
		m_transform.m_pos = so->m_transform.GetPosition();
	}
}

std::string LightSource::ToString()
{
	/// TODO: cba
	return std::string("TODO LightSource string");
}

std::string LightSource::GetClassName()
{
	return "LightSource";
}
