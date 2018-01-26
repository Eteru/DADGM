#include "Transform.h"
#include "DeltaTime.h"
#include "PrintUtils.h"


Transform::Transform(const Vector3 &pos /*= Vector3(0.f)*/, const Vector3 &rot /*= Vector3(0.f)*/, const Vector3 &scale /*= Vector3(1.f)*/, bool relative /*= true*/)
{
	m_params.resize(ParamIDS::NUM);

	m_params[POSITION].Init(Vector3(0.f), pos, Vector3(0.f), false);
	m_params[ROTATION].Init(Vector3(0.f), rot, Vector3(0.f), false);
	m_params[SCALE].Init(Vector3(1.f), scale, Vector3(1.f), true);


	m_relative = relative;

	SetPos(pos);
	SetRot(rot);
	SetScale(scale);
}

void Transform::SetParent(const Transform &parent)
{
	if (m_relative)
	{
		m_params[POSITION].m_currentParentWorld = parent.GetWorldPos();
		m_params[ROTATION].m_currentParentWorld = parent.GetWorldRot();
		m_params[SCALE].m_currentParentWorld = parent.GetWorldScale();
	}
}

Vector3 Transform::GetLocalPos() const
{
	return m_params[POSITION].m_currentLocal;
}

Vector3 Transform::GetLocalRot() const
{
	return m_params[ROTATION].m_currentLocal;
}

Vector3 Transform::GetLocalScale() const
{
	return m_params[SCALE].m_currentLocal;
}

Vector3 Transform::GetWorldPos() const
{
	return m_params[POSITION].m_currentWorld;
}

Vector3 Transform::GetWorldRot() const
{
	return m_params[ROTATION].m_currentWorld;
}

Vector3 Transform::GetWorldScale() const
{
	return m_params[SCALE].m_currentWorld;
}

Vector3 Transform::GetWorldLerpPos()
{
	return m_params[POSITION].GetWorldLerp();
}

Vector3 Transform::GetWorldLerpRot()
{
	return m_params[ROTATION].GetWorldLerp();
}

Vector3 Transform::GetWorldLerpScale()
{
	return m_params[SCALE].GetWorldLerp();
}

Vector3 Transform::GetLocalLerpPos()
{
	return m_params[POSITION].GetLocalLerp();
}

Vector3 Transform::GetLocalLerpRot()
{
	return m_params[ROTATION].GetLocalLerp();
}

Vector3 Transform::GetLocalLerpScale()
{
	return m_params[SCALE].GetLocalLerp();
}

void Transform::SetParentPos(const Vector3 val)
{
	m_params[POSITION].m_currentParentWorld = val;
}

void Transform::SetParentRot(const Vector3 val)
{
	m_params[ROTATION].m_currentParentWorld = val;
}

void Transform::SetParentScale(const Vector3 val)
{
	m_params[SCALE].m_currentParentWorld = val;
}

void Transform::SetPos(const Vector3 val)
{
	if (m_relative)
	{
		m_params[POSITION].m_currentLocal = val;
	}
	else
	{
		m_params[POSITION].m_currentWorld = val;
	}
}

void Transform::SetRot(const Vector3 val)
{
	if (m_relative)
	{
		m_params[ROTATION].m_currentLocal = val;
	}
	else
	{
		m_params[ROTATION].m_currentWorld = val;
	}
}

void Transform::SetScale(const Vector3 val)
{
	if (m_relative)
	{
		m_params[SCALE].m_currentLocal = val;
	}
	else
	{
		m_params[SCALE].m_currentWorld = val;
	}
}

void Transform::SaveCurrentParams()
{
	for (int i = 0; i < m_params.size(); ++i)
	{
		m_params[i].SaveParam();
	}
}

void Transform::ResetLerps()
{
	for (int i = 0; i < m_params.size(); ++i)
	{
		m_params[i].ResetLerps();
	}
}

void Transform::StepLerps(float time)
{
	for (int i = 0; i < m_params.size(); ++i)
	{
		m_params[i].StepLerps(time);
	}
}

void Transform::ComputeWorld()
{
	m_params[SCALE].m_currentWorld = m_params[SCALE].m_currentParentWorld * m_params[SCALE].m_currentLocal;
	m_params[ROTATION].m_currentWorld = m_params[ROTATION].m_currentParentWorld + m_params[ROTATION].m_currentLocal;
	
	m_params[POSITION].m_currentWorld = Math::RotateAroundOrigin(m_params[SCALE].m_currentParentWorld * m_params[POSITION].m_currentLocal, -m_params[ROTATION].m_currentParentWorld) + m_params[POSITION].m_currentParentWorld;
}

Matrix Transform::GetModel()
{
	Vector3 actualPos = GetWorldPos();
	Vector3 actualRot = GetWorldRot();
	Vector3 actualScale = GetWorldScale();

	Matrix P = Matrix().SetTranslation(actualPos);
	Matrix S = Matrix().SetScale(actualScale);

	Matrix Rx = Matrix().SetRotationX(actualRot.x);
	Matrix Ry = Matrix().SetRotationY(actualRot.y);
	Matrix Rz = Matrix().SetRotationZ(actualRot.z);

	return S * (Rx * Ry * Rz) * P;

}

Matrix Transform::GetLerpModel()
{
	Vector3 actualPos = GetWorldLerpPos();
	Vector3 actualRot = GetWorldLerpRot();
	Vector3 actualScale = GetWorldLerpScale();

	Matrix P = Matrix().SetTranslation(actualPos);
	Matrix S = Matrix().SetScale(actualScale);

	Matrix Rx = Matrix().SetRotationX(actualRot.x);
	Matrix Ry = Matrix().SetRotationY(actualRot.y);
	Matrix Rz = Matrix().SetRotationZ(actualRot.z);

	return S * (Rx * Ry * Rz) * P;
}

// Matrix Transform::GetParentModel()
// {
// 	Vector3 actualPos = m_params[POSITION].m_currentParentWorld;
// 	Vector3 actualRot = m_params[ROTATION].m_currentParentWorld;
// 	Vector3 actualScale = m_params[SCALE].m_currentParentWorld;
// 
// 	Matrix P = Matrix().SetTranslation(actualPos);
// 	Matrix S = Matrix().SetScale(actualScale);
// 
// 	Matrix Rx = Matrix().SetRotationX(actualRot.x);
// 	Matrix Ry = Matrix().SetRotationY(actualRot.y);
// 	Matrix Rz = Matrix().SetRotationZ(actualRot.z);
// 
// 	return S * (Rx * Ry * Rz) * P;
// }
// 
// Matrix Transform::GetLocalModel()
// {
// 	Vector3 actualPos = m_params[POSITION].m_currentLocal;
// 	Vector3 actualRot = m_params[ROTATION].m_currentLocal;
// 	Vector3 actualScale = m_params[SCALE].m_currentLocal;
// 
// 	Matrix P = Matrix().SetTranslation(actualPos);
// 	Matrix S = Matrix().SetScale(actualScale);
// 
// 	Matrix Rx = Matrix().SetRotationX(actualRot.x);
// 	Matrix Ry = Matrix().SetRotationY(actualRot.y);
// 	Matrix Rz = Matrix().SetRotationZ(actualRot.z);
// 
// 	return S * (Rx * Ry * Rz) * P;
// }

Transform::TransformParam::TransformParam()
{
	m_op = false;

}

void Transform::TransformParam::Init(Vector3 local, Vector3 world, Vector3 parent, bool op)
{
	m_currentLocal = local;
	m_currentWorld = world;
	m_currentParentWorld = parent;

	m_lastLocal = local;
	m_lastWorld = world;

	m_op = op;

	ResetLerps();
}

Vector3 Transform::TransformParam::GetLocalLerp()
{
	return m_localLerp.GetValue();
}

Vector3 Transform::TransformParam::GetWorldLerp()
{
	return m_worldLerp.GetValue();
}

void Transform::TransformParam::SaveParam()
{
	m_lastLocal = m_currentLocal;
	m_lastWorld = m_currentWorld;
}

void Transform::TransformParam::ResetLerps()
{
	m_localLerp = Vec3TimeLerp(m_lastLocal, m_currentLocal);
	m_worldLerp = Vec3TimeLerp(m_lastWorld, m_currentWorld);
}

void Transform::TransformParam::StepLerps(const float time)
{
	m_localLerp.Step(time);
	m_worldLerp.Step(time);
}

void Transform::TransformParam::ComputeWorld()
{
	m_currentWorld = m_op ? (m_currentParentWorld * m_currentLocal) : (m_currentParentWorld + m_currentLocal);
}
