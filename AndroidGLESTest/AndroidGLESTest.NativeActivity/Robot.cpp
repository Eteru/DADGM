#include "Robot.h"
#include "SceneObjectSpawner.h"
#include "UniqueID.h"
#include "BoundingSphere.h"

Robot::Robot()
{

}

void Robot::Init()
{
	
}

void Robot::FixedUpdate()
{
	//PrintUtils::PrintD("Fixed pos: " + m_transform.GetWorldPos().ToString());
// 	if (nullptr != m_physicsBody)
// 	{
// 		m_transform.SetPos(m_physicsBody->m_transform.GetLocalPos());
// 	}
}

void Robot::Update()
{
// 	//PrintUtils::PrintD("pos: " + m_transform.GetWorldPos().ToString());
// 
// 	if (nullptr != m_visualBody && nullptr != m_physicsBody)
// 	{
// 		m_visualBody->m_transform.SetPos(m_physicsBody->m_transform.GetLocalLerpPos());
// 	}
}

void Robot::Draw()
{
	
}

void Robot::Destroy()
{
	
}

std::string Robot::ToString()
{
	return std::string("TODO Robot string");
}

std::string Robot::GetClassName()
{
	return std::string("Robot");
}
