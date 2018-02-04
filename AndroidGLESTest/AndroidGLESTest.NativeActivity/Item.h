#pragma once
#include <vector>

#include "GameLoopObject.h"
#include "Stats.h"

class Buff
{	
public:
	Buff() : m_duration(0), m_expirationTime(Now()) {}
	std::vector<StatModifier> m_modifiers;
	float m_duration;
	TimePointNano m_expirationTime;
	std::string m_name;
};

class Robot;

class Item : public GameLoopObject
{
public:
	Item();
	~Item();

	Robot *m_robot;
	std::string m_name;
	std::string m_description;
	std::vector<StatModifier> m_modifiers;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

};

class ActiveItem : public Item, public StatHolderObject
{
public:

	virtual void FixedUpdate() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	virtual void Activate();

	bool m_isAvailable;
	float m_cooldown;

	Buff m_buff;

	virtual void Init() override;

protected:
	TimePointNano m_timeWhenReady;
};

class Armor : public Item, public StatHolderObject
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


	virtual void Init() override;


	virtual void DebugDraw() override;

private:
	void Fire();
	Vector3 GetInterceptTarget();

	static float InterceptTime(float projVel, Vector3 relPos, Vector3 relVel);

	Vector3 m_angular;
	Vector3 m_localRotTarget;
};