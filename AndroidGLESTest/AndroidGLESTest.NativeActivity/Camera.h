#pragma once

#include "Math.h"
#include <GLES2\gl2.h>

class Camera
{
public:
	Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat moveSpeed = 44.9f, GLfloat rotateSpeed = 0.5f, GLfloat cnear = 0.2f, GLfloat cfar = 1000.f, GLfloat fov = 45.F, GLfloat deltaTime = 0.05f);
	~Camera();

	inline void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	inline void SetTarget(Vector3 target)
	{
		m_target = target;
	}

	inline void SetUp(Vector3 up)
	{
		m_up = up;
	}

	inline void SetMoveSpeed(GLfloat moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	inline void SetRotateSpeed(GLfloat rotateSpeed)
	{
		m_rotateSpeed = rotateSpeed;
	}

	inline void SetDeltaTime(GLfloat deltaTime)
	{
		m_deltaTime = deltaTime;
	}

	inline Vector3 GetPosition()
	{
		return m_position;
	}

	inline Vector3 GetTarget()
	{
		return m_target;
	}

	inline Vector3 GetUp()
	{
		return m_up;
	}

	inline GLfloat GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	inline GLfloat GetRotateSpeed() const
	{
		return m_rotateSpeed;
	}

	inline GLfloat GetDeltaTime() const
	{
		return m_deltaTime;
	}

	inline Matrix GetView()
	{
		return m_viewMatrix;
	}

	inline Matrix GetProjection()
	{
		return m_P;
	}

	void MoveOX(int dir);
	void MoveOY(int dir);
	void MoveOZ(int dir);

	void RotateOX(int dir);
	void RotateOY(int dir);
	void RotateOZ(int dir);

	void UpdateWorldView();
	void RestoreDefaults();

private:
	Matrix m_viewMatrix;
	Matrix m_worldMatrix;
	Matrix m_R;
	Matrix m_P;

	Vector3 m_position;
	Vector3 m_target;
	Vector3 m_up;

	Vector3 m_default_position;
	Vector3 m_default_target;
	Vector3 m_default_up;

	Vector3 m_xAxis;
	Vector3 m_yAxis;
	Vector3 m_zAxis;

	GLfloat m_moveSpeed;
	GLfloat m_rotateSpeed;
	GLfloat m_near;
	GLfloat m_far;
	GLfloat m_fov;
	GLfloat m_deltaTime;
};

