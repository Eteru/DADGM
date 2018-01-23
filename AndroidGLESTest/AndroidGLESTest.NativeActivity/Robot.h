#pragma once
#include "GameLoopObject.h"

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
private:

	
	MovementController *m_movementController;
	MapManager *m_mapManager;
	PhysicsBody *m_physicsBody;
};