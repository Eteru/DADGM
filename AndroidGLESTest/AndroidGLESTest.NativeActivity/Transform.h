#pragma once
#include "Math.h"
#include "TimeLerp.h"


class Transform
{
public:

	Transform(const Vector3 &pos = Vector3(0.f), const Vector3 &rot = Vector3(0.f), const Vector3 &scale = Vector3(1.f));

	void SaveCurrentParams();

	void ResetLerps();
	void StepLerps(float time);

	Matrix GetModel();


	/*NOT a simple getter!*/
	Vector3 GetPosition();

	/*NOT a simple getter!*/
	Vector3 GetRotation();

	/*NOT a simple getter!*/
	Vector3 GetScale();

	Vector3 m_pos;
	Vector3 m_rot;
	Vector3 m_scale;
	bool m_lerp;
	bool m_relative;

private:

	Vec3TimeLerp m_posLerp;
	Vec3TimeLerp m_rotLerp;
	Vec3TimeLerp m_scaleLerp;

	Vector3 m_lastPos;
	Vector3 m_lastRot;
	Vector3 m_lastScale;


};
