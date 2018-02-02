#include "Item.h"

Item::Item()
{

}

Item::~Item()
{

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
	ActiveItem::FixedUpdate();

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
	ActiveItem::Activate();


}
