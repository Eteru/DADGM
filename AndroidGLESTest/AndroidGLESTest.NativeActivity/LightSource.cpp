
#include "LightSource.h"
#include "SceneManager.h"

LightSource::LightSource(float shininess, float diff_coef, float spec_coef, Vector3 diff_color, Vector3 spec_color, std::string id)
	: m_shininess(shininess), m_diffuse_coef(diff_coef), m_specular_coef(spec_coef), m_diffuse_color(diff_color),
	m_specular_color(spec_color), m_spot_angle(0.f), m_assoc_obj_id(id), m_position(Vector3(0,0,0)), m_type(NO_LIGHT), m_direction(Vector3(0, 0, 0))
{
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
	m_position = pos;
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
	SceneObject *so = SceneManager::GetInstance()->GetSceneObject(m_assoc_obj_id);

	if (nullptr != so) {
		m_position = so->GetPosition();
	}

	return m_position;
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
