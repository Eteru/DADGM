
#include "Camera.h"
#include "SceneManager.h"

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat cnear, GLfloat cfar, GLfloat fov, GLfloat deltaTime)
	: m_position(position), m_target(target), m_up(up), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed), m_near(cnear), m_far(cfar), m_fov(fov), m_deltaTime(deltaTime),
	m_default_position(position), m_default_target(target), m_default_up(up), m_object_to_follow_id("")
{
	UpdateWorldView();

	const engine *eng = SceneManager::GetInstance()->GetEngine();
	m_P.SetPerspective(m_fov, static_cast<GLfloat>(eng->width) / eng->height, m_near, m_far);
}

Camera::~Camera()
{
}

void Camera::MoveOX(int dir)
{
	Vector3 direction = m_target - m_position;
	Vector3 forward = -(m_up.Cross(direction)).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed * m_deltaTime : -forward * m_moveSpeed * m_deltaTime;
	m_position += deplasare;
	m_target += deplasare;

	UpdateWorldView();
}

void Camera::MoveOY(int dir)
{
	Vector3 forward = -(m_up - m_position).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed * m_deltaTime : -forward * m_moveSpeed * m_deltaTime;
	m_position += deplasare;
	m_up += deplasare;

	UpdateWorldView();
}

void Camera::MoveOZ(int dir)
{
	Vector3 forward = -(m_target - m_position).Normalize();
	Vector3 deplasare = dir == 1 ? forward * m_moveSpeed * m_deltaTime : -forward * m_moveSpeed * m_deltaTime;
	m_position += deplasare;
	m_target += deplasare;

	UpdateWorldView();
}

void Camera::RotateOX(int dir)
{
	Vector4 up4 = Vector4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationX(dir * m_rotateSpeed * m_deltaTime) * m_worldMatrix;
	m_up = Vector3(up4.x, up4.y, up4.z).Normalize();

	Vector4 localTarget = Vector4(0.f, 0.f, -(m_target - m_position).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * Matrix().SetRotationX(dir * m_rotateSpeed * m_deltaTime) * m_worldMatrix;

	m_target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	UpdateWorldView();
}

void Camera::RotateOY(int dir)
{
	Vector4 localTarget = Vector4(0.f, 0.f, -(m_target - m_position).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * Matrix().SetRotationY(dir * m_rotateSpeed * m_deltaTime) * m_worldMatrix;

	m_target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	UpdateWorldView();
}

void Camera::RotateOZ(int dir)
{
	Vector4 up4 = Vector4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationZ(dir * m_rotateSpeed * m_deltaTime) * m_worldMatrix;
	m_up = Vector3(up4.x, up4.y, up4.z).Normalize();

	UpdateWorldView();
}

void Camera::SetFollowingObject(std::string id, float x, float z)
{
	m_object_to_follow_id = id;
	m_xz_offset = Vector2(x, z);
}

void Camera::RestoreDefaults()
{
	m_position = m_default_position;
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
	if ("" != m_object_to_follow_id) {
		const SceneObject *obj = SceneManager::GetInstance()->GetSceneObject(m_object_to_follow_id);
		Vector3 obj_pos = obj->GetPosition();
		m_position.x = obj_pos.x - m_xz_offset.x;
		m_position.z = obj_pos.z - m_xz_offset.y;

		m_target = (obj_pos - m_position).Normalize();

		LOGD("New %s\n", ToString().c_str());
	}
}

void Camera::Update()
{
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
	return "Camera: Position: " + m_position.ToString() + 
		"Target: " + m_target.ToString() +
		"Up: " + m_up.ToString();
}

std::string Camera::GetClassName()
{
	return "Camera";
}


void Camera::UpdateWorldView()
{
	m_zAxis = -(m_target - m_position).Normalize();
	m_yAxis = m_up.Normalize();
	m_xAxis = m_zAxis.Cross(m_yAxis).Normalize();

	m_R.m[0][0] = m_xAxis.x; m_R.m[0][1] = m_xAxis.y;  m_R.m[0][2] = m_xAxis.z;  m_R.m[0][3] = 0.0f;
	m_R.m[1][0] = m_yAxis.x;  m_R.m[1][1] = m_yAxis.y; m_R.m[1][2] = m_yAxis.z;  m_R.m[1][3] = 0.0f;
	m_R.m[2][0] = m_zAxis.x;  m_R.m[2][1] = m_zAxis.y;  m_R.m[2][2] = m_zAxis.z; m_R.m[2][3] = 0.0f;
	m_R.m[3][0] = 0.0f;  m_R.m[3][1] = 0.0f;  m_R.m[3][2] = 0.0f;  m_R.m[3][3] = 1.0f;

	Vector3 neg_pos = -m_position;
	m_worldMatrix = m_R * Matrix().SetTranslation(m_position);
	m_viewMatrix = Matrix().SetTranslation(neg_pos) * m_R.Transpose();
}
