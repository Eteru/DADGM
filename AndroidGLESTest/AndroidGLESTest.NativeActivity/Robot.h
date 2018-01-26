#pragma once
#include "GameLoopObject.h"
#include "Item.h"

class MovementController;
class PhysicsBody;
class MapManager;

class Robot : public GameLoopObject
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


	void MoveTowards(const Vector2 pos);


	MapManager * GetMapManager() const { return m_mapManager; }
	void SetMapManager(MapManager * val) { m_mapManager = val; }
	PhysicsBody * GetPhysicsBody() const { return m_physicsBody; }
	void SetPhysicsBody(PhysicsBody * val) { m_physicsBody = val; }

	Item * GetArmor() const { return m_armor; }
	void SetArmor(Item *armor) { m_armor = armor; }
	Item * GetWeapon() const { return m_weapon; }
	void SetWeapon(Item *weapon) { m_weapon = weapon; }
private:

	Item *m_armor;
	Item *m_weapon;
	MovementController *m_movementController;
	MapManager *m_mapManager;
	PhysicsBody *m_physicsBody;
};