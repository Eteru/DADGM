#include "ObjectContactActions.h"
#include "GameLoopObject.h"
#include "Robot.h"
#include "Projectile.h"
#include "MapCell.h"
#include "PhysicsBody.h"
#include "RigidCollisionResponse.h"


void ObjectContactActions::OnContact(BVIntersections::ContactInfo contactInfo)
{
	GameLoopObject *obj1 = static_cast<PhysicsBody *>(contactInfo.m_o1)->m_linkedObject;
	GameLoopObject *obj2 = static_cast<PhysicsBody *>(contactInfo.m_o2)->m_linkedObject;

	if (nullptr == obj1 || nullptr == obj2)
	{
		return;
	}

	std::string name1 = obj1->GetClassName();
	std::string name2 = obj2->GetClassName();

	if (name1 == "Robot" && name2 == "Robot")
	{
		OnContact(static_cast<Robot *>(obj1), static_cast<Robot *>(obj2), contactInfo);
		return;
	}

	if (name1 == "Robot" && name2 == "Projectile")
	{
		OnContact(static_cast<Robot *>(obj1), static_cast<Projectile *>(obj2), contactInfo);
		return;
	}

	if (name2 == "Robot" && name1 == "Projectile")
	{
		OnContact(static_cast<Robot *>(obj2), static_cast<Projectile *>(obj1), contactInfo);
		return;
	}

	if (name1 == "Robot" && name2 == "MapCell")
	{
		OnContact(static_cast<Robot *>(obj1), static_cast<MapCell *>(obj2), contactInfo);
		return;
	}

	if (name2 == "Robot" && name1 == "MapCell")
	{
		OnContact(static_cast<Robot *>(obj2), static_cast<MapCell *>(obj1), contactInfo);
		return;
	}

	if (name1 == "Projectile" && name2 == "MapCell")
	{
		OnContact(static_cast<Projectile *>(obj1), static_cast<MapCell *>(obj2), contactInfo);
		return;
	}

	if (name2 == "Projectile" && name1 == "MapCell")
	{
		OnContact(static_cast<Projectile *>(obj2), static_cast<MapCell *>(obj1), contactInfo);
		return;
	}
}

void ObjectContactActions::OnContact(Robot *robot1, Robot *robot2, BVIntersections::ContactInfo contactInfo)
{
	RigidCollisionResponse::ApplyImpulses(contactInfo, 1.f);

	robot1->TakeDamage(robot2->m_armor->m_stats.at(StatType::DAMAGE).GetValue());
	robot2->TakeDamage(robot1->m_armor->m_stats.at(StatType::DAMAGE).GetValue());
}

void ObjectContactActions::OnContact(Robot *robot, Projectile *proj, BVIntersections::ContactInfo contactInfo)
{
	if (robot->m_team == proj->m_team)
		return;


	robot->TakeDamage(proj->m_stats.at(StatType::DAMAGE).GetValue());
	robot->m_physicsBody->ApplyLinearImpulse(Math::Normalize(robot->m_transform.GetWorldPos() - proj->m_transform.GetWorldPos()) * proj->m_stats.at(StatType::KNOCKBACK).GetValue());

	proj->GetParent()->DestroyRec();

	//proj->GetParent()->DestroyComponents(proj->GetClassName());
	//proj->Destroy();
// 	delete proj;
// 	proj = nullptr;
}

void ObjectContactActions::OnContact(Robot *robot, MapCell *mapCell, BVIntersections::ContactInfo contactInfo)
{
	RigidCollisionResponse::ApplyImpulses(contactInfo, 0.5f);
}

void ObjectContactActions::OnContact(Projectile *proj, MapCell *mapCell, BVIntersections::ContactInfo contactInfo)
{
	RigidCollisionResponse::ApplyImpulses(contactInfo, 1.f);

	proj->m_stats[StatType::BOUNCES].AddFlat(-1.f);

	if (proj->m_stats.at(StatType::BOUNCES).GetValue() <= 0)
	{
		proj->GetParent()->DestroyRec();
	}
}
