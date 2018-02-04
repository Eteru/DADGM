#include "Projectile.h"

Projectile::Projectile()
{

}

std::string Projectile::ToString()
{
	return std::string("TODO Projectile string");
}

std::string Projectile::GetClassName()
{
	return std::string("Projectile");
}

void Projectile::Init()
{
	m_timeOfDeath = Now() + std::chrono::nanoseconds(static_cast<long long>(m_stats.at(StatType::LIFE_TIME).GetValue() * 1000000000.f));
}

void Projectile::FixedUpdate()
{
	if (Now() > m_timeOfDeath)
	{
		m_parent->DestroyRec();
	}
}
