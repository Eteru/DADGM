#include "Camera.h"
#include "SceneManager.h"

#include "DeltaTime.h"

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat aspectRatio, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat cnear, GLfloat cfar, GLfloat fov)
	: m_target(target), m_up(up), m_aspect(aspectRatio), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed), m_near(cnear), m_far(cfar), m_fov(fov),
	m_default_position(position), m_default_target(target), m_default_up(up)
{

	m_transform = Transform(position, Vector3(0.f), Vector3(1.f), false);

	m_followedObject = nullptr;
	UpdateWorldView();

	m_P.SetPerspective(m_fov, aspectRatio, m_near, m_far);
}

Camera::~Camera()
{
}

void Camera::RotateOYTPS(const float rads)
{	
	Matrix rot = Matrix().SetTranslation(-m_target.x, -m_target.y, -m_target.z) * Matrix().SetRotationY(rads) * Matrix().SetTranslation(m_target);

	Vector4 v(m_transform.GetWorldPos());	
	m_transform.SetPos(v * rot);
}

void Camera::SetFollowingObject(GameLoopObject *obj, const float radius)
{
	m_followedObject = obj;

	//m_transform.m_pos += m_offset;

	m_radius = radius;
	m_target = obj->m_transform.GetWorldPos();	
}

void Camera::RestoreDefaults()
{
	m_transform = Transform(m_default_position);

	m_target = m_default_target;
	m_up = m_default_up;


	UpdateWorldView();
}

void Camera::FixedUpdate()
{
}

void Camera::Update()
{
	if (nullptr != m_followedObject)
	{
		Vector3 obj_pos = m_followedObject->m_transform.GetWorldLerpPos();
		
		m_target = obj_pos;

		Vector3 dir = m_transform.GetWorldPos() - obj_pos;

		if (dir.Length() == 0)
		{
			dir = Vector3(-1, -1, -1).Normalize();
		}
		else
		{
			dir = dir.Normalize();
		}


		Vector3 newPos = obj_pos + dir * m_radius;
		newPos.y = GameConstants::CAMERA_HEIGHT;

		m_transform.SetPos(newPos);
	}

	UpdateWorldView();
}

std::string Camera::ToString()
{
	return "Position: " + m_transform.GetWorldLerpPos().ToString() + ", target: " + m_target.ToString() + ", up: " + m_up.ToString();
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
	Vector3 actualPos = m_transform.GetWorldPos();

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
