#include "Transform.h"
#include "DeltaTime.h"
#include "PrintUtils.h"

Transform::Transform(const Vector3 &pos /*= Vector3(0.f)*/, const Vector3 &rot /*= Vector3(0.f)*/, const Vector3 &scale /*= Vector3(1.f)*/)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	m_lastPos = pos;
	m_lastRot = rot;
	m_lastScale = scale;

	m_lerp = true;
	m_relative = true;
}

void Transform::SaveCurrentParams()
{
	m_lastPos = m_pos;
	m_lastRot = m_rot;
	m_lastScale = m_scale;
}

void Transform::ResetLerps()
{
	m_posLerp = Vec3TimeLerp(m_lastPos, m_pos);
	m_rotLerp = Vec3TimeLerp(m_lastRot, m_rot);
	m_scaleLerp = Vec3TimeLerp(m_lastScale, m_scale);
}

void Transform::StepLerps(float time)
{
	m_posLerp.Step(time);
	m_rotLerp.Step(time);
	m_scaleLerp.Step(time);
}

Matrix Transform::GetModel()
{
	Vector3 actualPos = GetPosition();
	Vector3 actualRot = GetRotation();
	Vector3 actualScale = GetScale();

	Matrix P = Matrix().SetTranslation(actualPos);
	Matrix S = Matrix().SetScale(actualScale);
	
	Matrix Rx = Matrix().SetRotationX(actualRot.x);
	Matrix Ry = Matrix().SetRotationY(actualRot.y);
	Matrix Rz = Matrix().SetRotationZ(actualRot.z);

	return S * (Rx * Ry * Rz) * P;
}

Vector3 Transform::GetPosition()
{
	return m_lerp ? m_posLerp.GetValue() : m_pos;
}

Vector3 Transform::GetRotation()
{
	return m_lerp ? m_rotLerp.GetValue() : m_rot;
}

Vector3 Transform::GetScale()
{
	return m_lerp ? m_scaleLerp.GetValue() : m_scale;
}
