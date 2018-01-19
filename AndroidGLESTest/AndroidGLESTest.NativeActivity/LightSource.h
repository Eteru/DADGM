#pragma once

#include <string>
#include "Math.h"
#include "GameLoopObject.h"

class LightSource : public GameLoopObject
{
public:
	enum LightType { POINT_LIGHT = 0, DIRECTIONAL_LIGHT, SPOT_LIGHT, NO_LIGHT };
	LightSource(float shininess, float diff_coef, float spec_coef, Vector3 diff_color, Vector3 spec_color);
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

	virtual void Update() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	inline GameLoopObject * GetFollowedObject() const
	{ 
		return m_followedObject;
	}

	inline void SetFollowedObject(GameLoopObject * val)
	{
		m_followedObject = val;
	}

private:
	LightType m_type;
	float m_shininess;
	float m_spot_angle;
	float m_diffuse_coef;
	float m_specular_coef;
	Vector3 m_diffuse_color;
	Vector3 m_specular_color;	
	Vector3 m_direction;

	GameLoopObject *m_followedObject;
};

