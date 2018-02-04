#pragma once

#include "GameLoopObject.h"

class PhysicsBody;
class MapManager;

class MovementController : public GameLoopObject
{
public:
	MovementController(PhysicsBody *physicsBody, MapManager *mapManager);

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;


	void ClearPath();
	void SetTarget(Vector2 target);
	void SetRandomTarget();

	bool IsEmpty();

	PhysicsBody * GetPhysicsBody() const { return m_physicsBody; }
	void SetPhysicsBody(PhysicsBody * val) { m_physicsBody = val; }
private:
	void SetPath(std::vector<Vector2> path);
	Vector2 NextPoint();
	void ArrivedAtCheckpoint();

	std::vector<Vector2> m_path;
	PhysicsBody *m_physicsBody;
	MapManager *m_mapManager;
	Vector2 m_target;
};
