#include "Transform.h"
#include "DeltaTime.h"
#include "PrintUtils.h"
#include "GameConstants.h"
#include "DebugDrawPrimitives.h"


Transform::Transform(const Vector3 &pos /*= Vector3(0.f)*/, const Vector3 &rot /*= Vector3(0.f)*/, const Vector3 &scale /*= Vector3(1.f)*/, bool relative /*= true*/)
{
	m_forward = GameConstants::DEFAULT_FORWARD;
	m_up = GameConstants::DEFAULT_UP;
	m_right = GameConstants::DEFAULT_RIGHT;

	m_params.resize(ParamIDS::NUM);

	m_params[POSITION].Init(pos, pos, pos, false);
	m_params[ROTATION].Init(rot, rot, rot, false);
	m_params[SCALE].Init(scale, scale, scale, true);

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

		ComputeWorld();

		m_params[POSITION].Init(m_params[POSITION].m_currentLocal, m_params[POSITION].m_currentWorld, m_params[POSITION].m_currentParentWorld, false);
		m_params[ROTATION].Init(m_params[ROTATION].m_currentLocal, m_params[ROTATION].m_currentWorld, m_params[ROTATION].m_currentParentWorld, false);
		m_params[SCALE].Init(m_params[SCALE].m_currentLocal, m_params[SCALE].m_currentWorld, m_params[SCALE].m_currentParentWorld, true);
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

Vector3 Transform::GetWorldLerpPos() const
{
	return m_params[POSITION].GetWorldLerp();
}

Vector3 Transform::GetWorldLerpRot() const
{
	return m_params[ROTATION].GetWorldLerp();
}

Vector3 Transform::GetWorldLerpScale()const
{
	return m_params[SCALE].GetWorldLerp();
}

Vector3 Transform::GetLocalLerpPos() const
{
	return m_params[POSITION].GetLocalLerp();
}

Vector3 Transform::GetLocalLerpRot() const
{
	return m_params[ROTATION].GetLocalLerp();
}

Vector3 Transform::GetLocalLerpScale() const
{
	return m_params[SCALE].GetLocalLerp();
}

Vector3 Transform::GetUp() const
{
	return m_up;
}

Vector3 Transform::GetRight() const
{
	return m_right;
}

Vector3 Transform::GetForward() const
{
	return m_forward;
}

Vector3 Transform::_GetLastWorldPos() const
{
	return m_params[POSITION].GetLastWorld();
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
		UpdateAxes();
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

	UpdateAxes();
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

void Transform::DebugDraw() const
{
// 	Vector3 wp = GetWorldPos();
// 	Vector3 ws = GetWorldScale();

	Vector3 wp = GetWorldLerpPos();
	Vector3 ws = GetWorldLerpScale();


	DebugDrawPrimitives::DrawLine(wp, wp + (m_right * ws.x * 2.f), DebugDrawPrimitives::COLOR_RED);
	DebugDrawPrimitives::DrawLine(wp, wp + (m_up * ws.y  * 2.f), DebugDrawPrimitives::COLOR_GREEN);
	DebugDrawPrimitives::DrawLine(wp, wp + (m_forward * ws.z * 2.f), DebugDrawPrimitives::COLOR_BLUE);
}

void Transform::UpdateAxes()
{

	Matrix rot = Math::GetRotationMatrix(-GetWorldLerpRot());

	Vector4 up(GameConstants::DEFAULT_UP, 0);
	Vector4 right(GameConstants::DEFAULT_RIGHT, 0);
	Vector4 forward(GameConstants::DEFAULT_FORWARD, 0);

	up = rot * up;
	right = rot * right;
	forward = rot * forward;

	m_up.x = up.x;
	m_up.y = up.y;
	m_up.z = up.z;

	m_right.x = right.x;
	m_right.y = right.y;
	m_right.z = right.z;

	m_forward.x = forward.x;
	m_forward.y = forward.y;
	m_forward.z = forward.z;

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

Vector3 Transform::TransformParam::GetLocalLerp() const
{
	return m_localLerp.GetValue();
}

Vector3 Transform::TransformParam::GetWorldLerp() const
{
	return m_worldLerp.GetValue();
}

Vector3 Transform::TransformParam::GetLastWorld() const
{
	return m_lastWorld;
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
