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

	void EquipItem(const std::string itemName, const size_t slot, std::vector<StatModifier> extraModifiers = std::vector<StatModifier>());
	void UnequipItem(const size_t slot);

	bool IsItemAvailable(const std::string itemName) const;
	void ActivateItem(const std::string itemName);

	int MapDistanceToTarget() const;




	PhysicsBody * GetPhysicsBody() const { return m_physicsBody; }
	void SetPhysicsBody(PhysicsBody * val) { m_physicsBody = val; }

	Armor * GetArmor() const { return m_armor; }
	void SetArmor(Armor *armor) { m_armor = armor; }
	Weapon * GetWeapon() const { return m_weapon; }
	void SetWeapon(Weapon *weapon) { m_weapon = weapon; }



	size_t m_team;
	Robot *m_target;

	std::string m_name;
	std::string m_description;
	Armor *m_armor;
	Weapon *m_weapon;
	PhysicsBody *m_physicsBody;
	MapManager *m_mapManager;

private:
	Item *FindPassive(const std::string name) const;
	ActiveItem *FindActive(const std::string name) const;

	void AcquireTarget();

	void ManageBuffs();

	void ApplyBuff(Buff buff);

	void ApplyModifiers(const std::vector<StatModifier> &mods);
	void RemoveModifiers(const std::vector<StatModifier> &mods);

	std::vector<Buff> m_activeBuffs;

	std::vector<Item *> m_equippedItems;
	size_t m_inventorySize;

	std::vector<Item *> m_passiveItems;
	std::vector<ActiveItem *> m_activeItems;
};