#pragma once

#include <string>
#include "Math.h"

class LightSource
{
public:
	enum LightType { POINT_LIGHT = 0, DIRECTIONAL_LIGHT, SPOT_LIGHT, NO_LIGHT };
	LightSource(float shininess, float diff_coef, float spec_coef, Vector3 diff_color, Vector3 spec_color, std::string id);
	virtual ~LightSource();

	void SetType(std::string type);
	void SetPosition(Vector3 pos);
	void SetDirection(Vector3 dir);
	void SetSpotAngle(float angle);

	Vector3 GetPosition();
	Vector3 GetDirection();
	Vector3 GetSpecularLight() const;
	Vector3 GetDiffuseLight() const;
	float GetSpecularCoefficient() const;
	float GetDiffuseCoefficient() const;
	float GetShininess() const;
	float GetSpotAngle() const;
	LightType GetType() const;

private:
	LightType m_type;
	float m_shininess;
	float m_spot_angle;
	float m_diffuse_coef;
	float m_specular_coef;
	Vector3 m_diffuse_color;
	Vector3 m_specular_color;
	Vector3 m_position;
	Vector3 m_direction;

	std::string m_assoc_obj_id;
};

