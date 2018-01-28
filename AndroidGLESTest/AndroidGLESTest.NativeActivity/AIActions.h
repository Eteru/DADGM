#pragma once
#include "GameLoopObject.h"

///TODO movement actions shouldn't have execute, only fixed update

class AIAction : public GameLoopObject
{

public:
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	size_t m_priority;
};

class ItemAction : public AIAction
{
public:

	virtual void Execute() = 0;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

};

class MapManager;
class PhysicsBody;
class MovementController;

class MovementAction : public AIAction
{
public:
	MovementAction(MovementController *movementController, MapManager *mapManager);

	bool IsFinished();
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	MovementController *m_movementController;
	MapManager *m_mapManager;
	Vector2 m_target;
};

class Explore : public MovementAction
{
public:
	Explore(MovementController *movementController, MapManager *mapManager);

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;


	virtual void FixedUpdate() override;

};

class KeepDistance : public MovementAction
{
public:

	KeepDistance(const Vector2 target, const int distance, MovementController *movementController, MapManager *mapManager);

	int m_distance;

	virtual std::string ToString() override;


	virtual std::string GetClassName() override;
	virtual void DebugDraw() override;
	virtual void FixedUpdate() override;


private:
	void UpdateClosestPoint();
	std::vector<Vector2> m_allPoints;
	Vector2 m_closestPoint;

};