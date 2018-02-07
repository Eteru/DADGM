#include "Item.h"

#include "Robot.h"
#include "Projectile.h"
#include "UniqueID.h"
#include "SceneObjectSpawner.h"
#include "MapManager.h"
#include "DebugDrawPrimitives.h"

Item::Item()
{

}

Item::~Item()
{

}

std::string Item::ToString()
{
	return std::string("TODO Item string");
}

std::string Item::GetClassName()
{
	return std::string("Item");
}

void ActiveItem::FixedUpdate()
{
	if (Now() > m_timeWhenReady)
	{
		m_isAvailable = true;
	}
}

std::string ActiveItem::ToString()
{
	return std::string("TODO ActiveItem string");
}

std::string ActiveItem::GetClassName()
{
	return std::string("ActiveItem");
}

void ActiveItem::Activate()
{
	if (m_cooldown > 0)
	{
		m_timeWhenReady = Now() + std::chrono::nanoseconds(static_cast<long long>(m_cooldown * 1000000000));
		m_isAvailable = false;
	}
}

void ActiveItem::Init()
{
	m_isAvailable = false;
}

std::string Armor::ToString()
{
	return std::string("TODO Armor string");
}

std::string Armor::GetClassName()
{
	return std::string("Armor");
}

void Weapon::FixedUpdate()
{
	if (Now() > m_timeWhenReady)
	{
		m_isAvailable = true;
	}


	float lerpStep = 0.f;

	if (nullptr != m_robot->m_target)
	{
		m_aimTarget = GetInterceptTarget();

		Vector3 dir = m_aimTarget - m_robot->m_transform.GetWorldPos();

		if (Math::Length(dir) != 0)
		{
			m_localRotTarget = Math::GetAlignmentRotation(GameConstants::DEFAULT_FORWARD, Math::Normalize(dir)) - m_robot->m_transform.GetWorldRot();
			lerpStep = 0.2f;
		}
		else
		{
			m_localRotTarget = Vector3(0.f);
		}
	}
	else
	{
		m_localRotTarget = Vector3(0.f);
		lerpStep = 0.05f;
	}

	if (Math::Distance(m_localRotTarget, m_transform.GetLocalRot()) > Math::PI / 10.f)
	{
		Vector3 rotStep = Math::Lerp(m_transform.GetLocalRot(), m_localRotTarget, lerpStep);
		m_transform.SetRot(rotStep);
	}
	else
	{
		m_transform.SetRot(m_localRotTarget);
	}

}

std::string Weapon::ToString()
{
	return std::string("TODO Weapon string");
}

std::string Weapon::GetClassName()
{
	return std::string("Weapon");
}

void Weapon::Activate()
{
	//ActiveItem::Activate();

	if (!m_isAvailable || nullptr == m_robot->m_target)
		return;
	
	///TODO Fire action

	Fire();


	m_stats.at(StatType::AMMO).AddFlat(-1);

	if (m_stats.at(StatType::AMMO).GetValue() == 0.f)
	{
		m_stats.at(StatType::AMMO).AddFlat(m_stats.at(StatType::AMMO_MAX).GetValue());
		m_timeWhenReady = Now() + std::chrono::nanoseconds(static_cast<long long>(m_stats.at(StatType::RELOAD_TIME).GetValue() * 1000000000.f));
		m_isAvailable = false;
	}
	else
	{
		m_timeWhenReady = Now() + std::chrono::nanoseconds(static_cast<long long>(1000000000.f / (m_stats.at(StatType::FIRE_RATE).GetValue())));
		m_isAvailable = false;
	}


}

void Weapon::Init()
{
	m_stats[StatType::AMMO] = Stat(m_stats.at(StatType::AMMO_MAX).GetValue());
}

void Weapon::DebugDraw()
{	
	if (nullptr == m_robot->m_target)
		return;

	DebugDrawPrimitives::DrawLine(m_transform.GetWorldPos(), m_aimTarget, DebugDrawPrimitives::COLOR_PURPLE);
}

void Weapon::Fire()
{
	Projectile *proj = SceneObjectSpawner::SpawnProjectile(m_transform.GetWorldPos() + m_transform.GetForward() * m_transform.GetWorldScale().z, m_aimTarget, m_robot->m_team,
		m_stats.at(StatType::DAMAGE).GetValue(), m_stats.at(StatType::LIFE_TIME).GetValue(), m_stats.at(StatType::BOUNCES).GetValue(), false, m_stats.at(StatType::LINEAR_TOP).GetValue(),
		m_stats.at(StatType::KNOCKBACK).GetValue());

	AddComponent(proj->m_physicsBody);

	m_robot->m_mapManager->m_projectiles.push_back(proj);
}

Vector3 Weapon::GetInterceptTarget()
{
	Vector3 relPos = m_robot->m_target->m_transform.GetWorldPos() - m_robot->m_transform.GetWorldPos();
	Vector3 relVel = m_robot->m_target->m_physicsBody->m_linearVelEngine - m_robot->m_physicsBody->m_linearVelEngine;
	
	return m_robot->m_target->m_transform.GetWorldPos() + InterceptTime(m_stats.at(StatType::LINEAR_TOP).GetValue() * GameConstants::PHYSICS_TIME_STEP, relPos, relVel) * relVel * GameConstants::PHYSICS_TIME_STEP;
}

float Weapon::InterceptTime(float projVel, Vector3 relPos, Vector3 relVel)
{

	static const float eps = 0.000001f;

	float velsq = relVel.Length();
	velsq *= velsq;

	if (velsq < eps)
		return 0.f;

	float a = velsq - projVel * projVel;

	float c = relPos.Length();
	c *= c;

	float b = 2.f * Math::Dot(relVel, relPos);

	if (std::abs(a) < eps)
	{
		return std::max(0.f, -c / b);
	}

	float delta = b * b - 4.f * a * c;


	if (delta > 0.f)
	{
		float t1 = (-b + std::sqrt(delta)) / (2.f * a);
		float t2 = (-b - std::sqrt(delta)) / (2.f * a);

		if (t1 > 0)
		{
			if (t2 > 0)
				return std::min(t1, t2);

			return t1;
		}
		return std::max(t2, 0.f);
	}

	if (delta < 0.f)
		return 0.f;

	return std::max(-b / (2.f * a), 0.f);
}
