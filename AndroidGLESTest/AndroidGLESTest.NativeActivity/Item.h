#pragma once
#include <vector>

#include "GameLoopObject.h"
#include "Stats.h"

class Buff
{	
public:
	std::vector<StatModifier> m_modifiers;
	float m_duration;
	TimePointNano m_expirationTime;
};

class Item
{
public:
	Item();
	~Item();

	std::string m_name;
	std::string m_description;
	std::vector<StatModifier> m_modifiers;
};

class ActiveItem : public Item, public GameLoopObject, public StatHolderObject
{
public:

	virtual void FixedUpdate() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	virtual void Activate();

	bool m_isAvailable;
	float m_cooldown;

	Buff m_buff;
private:
	TimePointNano m_timeWhenReady;
};

class Armor : public GameLoopObject, public Item, public StatHolderObject
{

public:
	virtual std::string ToString() override;


	virtual std::string GetClassName() override;

};

class Weapon : public ActiveItem
{

public:
	virtual void FixedUpdate() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;
	virtual void Activate() override;

};