#include "Camera.h"
#include "SceneManager.h"

#include "DeltaTime.h"

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat aspectRatio, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat cnear, GLfloat cfar, GLfloat fov)
	: m_target(target), m_up(up), m_aspect(aspectRatio), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed), m_near(cnear), m_far(cfar), m_fov(fov),
	m_default_position(position), m_default_target(target), m_default_up(up)
{
	m_transform.m_pos = position;
	m_transform.m_lerp = false;
	m_followedObject = nullptr;
	UpdateWorldView();

	m_P.SetPerspective(m_fov, aspectRatio, m_near, m_far);
}

Camera::~Camera()
{
}

void Camera::MoveOX(int dir)
{
	Vector3 actualPos = m_transform.GetPosition();
	Vector3 direction = m_target - actualPos;
	Vector3 forward = -(m_up.Cross(direction)).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed : -forward * m_moveSpeed;
	m_transform.m_pos += deplasare;
	m_target += deplasare;

	UpdateWorldView();
}

void Camera::MoveOY(int dir)
{
	Vector3 actualPos = m_transform.GetPosition();
	Vector3 forward = -(m_up - actualPos).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed : -forward * m_moveSpeed;
	m_transform.m_pos += deplasare;
	m_up += deplasare;

	UpdateWorldView();
}

void Camera::MoveOZ(int dir)
{
	Vector3 forward = -(m_target - m_transform.m_pos).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed : -forward * m_moveSpeed;
	m_transform.m_pos += deplasare;
	m_target += deplasare;

	UpdateWorldView();
}

void Camera::RotateOX(int dir)
{
	Vector4 up4 = Vector4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationX(dir * m_rotateSpeed) * m_worldMatrix;
	m_up = Vector3(up4.x, up4.y, up4.z).Normalize();

	Vector4 localTarget = Vector4(0.f, 0.f, -(m_target - m_transform.m_pos).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * Matrix().SetRotationX(dir * m_rotateSpeed) * m_worldMatrix;

	m_target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	UpdateWorldView();
}

void Camera::RotateOY(int dir)
{
	Vector4 localTarget = Vector4(0.f, 0.f, -(m_target - m_transform.m_pos).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * Matrix().SetRotationY(dir * m_rotateSpeed) * m_worldMatrix;

	m_target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	UpdateWorldView();
}

void Camera::RotateOYTPS(const float rads)
{	
	Matrix rot = Matrix().SetTranslation(-m_target.x, -m_target.y, -m_target.z) * Matrix().SetRotationY(rads) * Matrix().SetTranslation(m_target);

	Vector4 v(m_transform.m_pos);	
	m_transform.m_pos = v * rot;
}

void Camera::RotateOZ(int dir)
{
	Vector4 up4 = Vector4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationZ(dir * m_rotateSpeed) * m_worldMatrix;
	m_up = Vector3(up4.x, up4.y, up4.z).Normalize();

	UpdateWorldView();
}

void Camera::MoveTPS(const int x, const int y)
{
// 	Vector3 actualPos = m_transform.m_pos;
// 
// 	float radius = (m_target - m_offset).Length();
// 
// 	m_offset.x = m_target.x + radius * std::cos(m_phi) * std::sin(m_theta);
// 	m_offset.y = m_target.y + radius * std::sin(m_phi) * std::sin(m_theta);
// 	m_offset.z = m_target.z + radius * std::cos(m_theta);

//  	Vector3 actualPos = m_offset;
// 	Vector2 targetScreen = WorldToScreen(m_target);
// 
// 	Vector3 targetMouse(x - targetScreen.x, y - targetScreen.y, 0);
// 
// 	Vector3 targetCam = actualPos - m_target;
// 
// 	Vector3 rotatedTarget = Math::RotateTowards(targetCam, targetMouse, 1.1f);
// 
// 	float radius = (m_target - actualPos).Length();
// 
// 	Vector3 finalDisp = m_target + radius * rotatedTarget.Normalize();
// 	m_offset = Vector3(finalDisp.x, m_offset.y, finalDisp.z);
}

void Camera::SetFollowingObject(GameLoopObject *obj, const float radius)
{
	m_followedObject = obj;

	//m_transform.m_pos += m_offset;

	m_radius = radius;
	m_target = obj->m_transform.m_pos;

}

void Camera::RestoreDefaults()
{
	m_transform = Transform(m_default_position);

	m_target = m_default_target;
	m_up = m_default_up;


	UpdateWorldView();
}

void Camera::Init()
{
	// Not sure anything should happen here
}

void Camera::FixedUpdate()
{
	if (nullptr != m_followedObject)
	{
		Vector3 dir = m_transform.m_pos - m_followedObject->m_transform.m_pos;
		if (dir.Length() == 0)
		{
			dir = Vector3(-1, -1, -1).Normalize();
		}
		else
		{
			dir = dir.Normalize();
		}

		float oldY = m_transform.m_pos.y;

		m_transform.m_pos = m_followedObject->m_transform.m_pos + dir * m_radius;
		m_transform.m_pos.y = oldY;
	}
}

void Camera::Update()
{
	if (nullptr != m_followedObject)
	{
		Vector3 obj_pos = dynamic_cast<SceneObject*>(m_followedObject)->m_transform.GetPosition();
		
		m_target = obj_pos;
	}

	UpdateWorldView();
}

void Camera::Draw()
{
	// No camera draw required, only if we draw something for debug
}

void Camera::Destroy()
{
	// nothing to destroy
}

std::string Camera::ToString()
{
	return "Position: " + m_transform.m_pos.ToString() + ", target: " + m_target.ToString() + ", up: " + m_up.ToString();
}

std::string Camera::GetClassName()
{
	return "Camera";
}

void Camera::OnTouchUp(const int x, const int y)
{
	//RestoreDefaults();
// 	m_theta = 0.f;
// 	m_phi = 0.f;
}

void Camera::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y)
{
	int dX = x - xPrev;
	int dY = y - yPrev;


	if (nullptr != m_followedObject)
	{
		const float PI = 3.14159;


		RotateOYTPS(2 * PI * dX / m_width);
	}

	// 	if (0 != dX)
	// 	{
	// 		MoveOX(dX > 0 ? 1 : -1);
	// 	}
	// 
	// 	if (0 != dY)
	// 	{
	// 		MoveOY(dY > 0 ? 1 : -1);
	// 	}
}

Vector2 Camera::WorldToScreen(const Vector3 &worldPos)
{

	Vector3 dots(Math::Dot(worldPos, Vector3(0.f, 1.f, 0.f)), Math::Dot(worldPos, Vector3(0.f, 0.f, 1.f)), Math::Dot(worldPos, Vector3(1.f, 0.f, 0.f)));

	Vector2 center = Vector2(m_width * 0.5f, m_height * 0.5f);


	GLfloat x = center.x * (1 - (dots.x / m_fov / dots.z));
	GLfloat y = center.y * (1 - (dots.y / m_fov / dots.z));

	return Vector2(x, y);
}

void Camera::UpdateWorldView()
{
	Vector3 actualPos = m_transform.GetPosition();

	m_zAxis = (actualPos - m_target).Normalize();
// 	m_yAxis = m_up.Normalize();
// 	m_xAxis = m_zAxis.Cross(m_yAxis).Normalize();

	Vector3 crs = Math::Cross(Vector3(0, 1, 0), m_zAxis);
	m_xAxis = crs.Length() == 0? Vector3(1, 0, 0) : Math::Normalize(crs);
	m_yAxis = Math::Cross(m_zAxis, m_xAxis);

	m_R.m[0][0] = m_xAxis.x; m_R.m[0][1] = m_xAxis.y; m_R.m[0][2] = m_xAxis.z; m_R.m[0][3] = 0.0f;
	m_R.m[1][0] = m_yAxis.x; m_R.m[1][1] = m_yAxis.y; m_R.m[1][2] = m_yAxis.z; m_R.m[1][3] = 0.0f;
	m_R.m[2][0] = m_zAxis.x; m_R.m[2][1] = m_zAxis.y; m_R.m[2][2] = m_zAxis.z; m_R.m[2][3] = 0.0f;
	m_R.m[3][0] = 0.0f;		 m_R.m[3][1] = 0.0f;	  m_R.m[3][2] = 0.0f;      m_R.m[3][3] = 1.0f;

	Vector3 neg_pos = -actualPos;
	m_worldMatrix = m_R * Matrix().SetTranslation(actualPos);
	m_viewMatrix = Matrix().SetTranslation(neg_pos) * m_R.Transpose();

	m_up = m_yAxis;	
}
