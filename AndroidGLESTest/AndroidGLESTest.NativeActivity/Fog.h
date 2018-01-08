#pragma once

#include "Math.h"

class Fog
{
public:
	Fog();
	Fog(float r, float R, Vector3 & color);
	virtual ~Fog();

	void SetValues(float r, float R, const Vector3 & color);
	float ComputeAlpha(const float distance) const;

	inline Vector3 GetColor() const
	{
		return m_color;
	}

private:
	float m_r;
	float m_R;
	Vector3 m_color;
};

