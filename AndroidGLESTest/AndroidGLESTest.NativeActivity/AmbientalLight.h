#pragma once

#include "Math.h"

class AmbientalLight
{
public:
	AmbientalLight();
	AmbientalLight(float ratio, Vector3 color);
	virtual ~AmbientalLight();

	void SetValue(float ratio, Vector3 & color);

	inline const Vector3 & GetColor() const
	{
		return m_color;
	}

	inline const float GetRatio() const
	{
		return m_ratio;
	}

private:
	float m_ratio;
	Vector3 m_color;
};

