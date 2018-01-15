#pragma once

#include "Math.h"
#include "GameLoopObject.h"
#include <GLES2\gl2.h>

class Camera : public GameLoopObject
{
public:
	Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat aspectRatio, GLfloat moveSpeed = 44.9f, GLfloat rotateSpeed = 0.5f, GLfloat cnear = 0.2f, GLfloat cfar = 1000.f, GLfloat fov = 45.F);
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

	void SetFollowingObject(std::string id, float x, float z);

	void UpdateWorldView();
	void RestoreDefaults();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;
	virtual void OnTouchUp(const int x, const int y) override;
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) override;

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
	GLfloat m_aspect;

	// following object
	Vector2 m_xz_offset;
	std::string m_object_to_follow_id;
};

