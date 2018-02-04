#pragma once
#include "GameLoopObject.h"
#include "Item.h"

class MovementController;
class PhysicsBody;
class MapManager;

class Robot : public GameLoopObject, public StatHolderObject
{

public:

	Robot();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;


	void TakeDamage(const float value);
	bool IsDead();
	int MapDistanceToTarget() const;
	void AcquireTarget();

	PhysicsBody * GetPhysicsBody() const { return m_physicsBody; }
	void SetPhysicsBody(PhysicsBody * val) { m_physicsBody = val; }

	Armor * GetArmor() const { return m_armor; }
	void SetArmor(Armor *armor) { m_armor = armor; }
	Weapon * GetWeapon() const { return m_weapon; }
	void SetWeapon(Weapon *weapon) { m_weapon = weapon; }

	Item *FindPassive(const std::string name) const;
	ActiveItem *FindActive(const std::string name) const;

	std::vector<Item *> m_passiveItems;
	std::vector<ActiveItem *> m_activeItems;

	size_t m_team;
	Robot *m_target;

	std::string m_name;
	std::string m_description;
	Armor *m_armor;
	Weapon *m_weapon;
	PhysicsBody *m_physicsBody;
	MapManager *m_mapManager;

private:


};