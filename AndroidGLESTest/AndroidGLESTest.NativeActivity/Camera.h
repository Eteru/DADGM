#pragma once

#include "Math.h"
#include "GameLoopObject.h"
#include "SceneObject.h"
#include <GLES2\gl2.h>

class Camera : public GameLoopObject
{
public:
	Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat aspectRatio, GLfloat moveSpeed = 44.9f, GLfloat rotateSpeed = 0.5f, GLfloat cnear = 0.2f, GLfloat cfar = 1000.f, GLfloat fov = 45.F);
	~Camera();

	inline Vector3 GetPosition()
	{
		return m_transform.GetPosition();
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


	void MoveTPS(const int x, const int y);

	void RotateOYTPS(const float rads);
	void MoveOX(int dir);
	void MoveOY(int dir);
	void MoveOZ(int dir);

	void RotateOX(int dir);
	void RotateOY(int dir);
	void RotateOZ(int dir);

	void SetFollowingObject(GameLoopObject *obj, const float radius);

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

	Vector2 WorldToScreen(const Vector3 &worldPos);

	int m_width;
	int m_height;
private:
	Matrix m_viewMatrix;
	Matrix m_worldMatrix;
	Matrix m_R;
	Matrix m_P;
	
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
	GLfloat m_radius;

	GameLoopObject *m_followedObject;
};

