#pragma once
#include <unordered_map>

enum class MovementActionType { STAND_STILL = 0, EXPLORE, KEEP_DISTANCE, RUN_AWAY, NUM_MOVEMENT_ACTION_TYPE };

const std::unordered_map<std::string, MovementActionType> XMLMovementActionMap = {
	{"StandStill", MovementActionType::STAND_STILL},
{ "Explore", MovementActionType::EXPLORE },
{ "KeepDistance", MovementActionType::KEEP_DISTANCE},
{ "RunAway", MovementActionType::RUN_AWAY}
};

class MovementController;
class MapManager;
class GameLoopObject;

class AIMovementActions
{
public:
	static void SetDestination(MovementActionType type, MovementController *movementController, GameLoopObject *target, MapManager *mapManager);
private:
	static void Explore(MovementController *movementController, MapManager *mapManager);
	static void KeepDistance(MovementController *movementController, MapManager *mapManager, GameLoopObject *target);
	static void RunAway(MovementController *movementController, MapManager *mapManager, GameLoopObject *target);
	static void StandStill(MovementController *movementController);
};

// class AIAction : public GameLoopObject
// {
// 
// public:
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 
// 	//size_t m_priority;
// };
// 
// class ItemAction : public AIAction
// {
// public:
// 
// 	virtual void Execute() = 0;
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 
// 	ActiveItem *m_item;
// 
// };

// class MapManager;
// class PhysicsBody;
// class MovementController;
// 
// class MovementAction : public AIAction
// {
// public:
// 	MovementAction(MovementController *movementController, MapManager *mapManager);
// 
// 	static MovementAction * GetAction(const MovementActionType type);
// 
// 	bool IsFinished();
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 
// 	MovementController *m_movementController;
// 	MapManager *m_mapManager;
// 	Vector2 m_target;
// };
// 
// class Explore : public MovementAction
// {
// public:
// 	Explore(MovementController *movementController, MapManager *mapManager);
// 
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 
// 
// 	virtual void FixedUpdate() override;
// 
// };
// 
// class KeepDistance : public MovementAction
// {
// public:
// 
// 	KeepDistance(const Vector2 target, const int distance, MovementController *movementController, MapManager *mapManager);
// 
// 	int m_distance;
// 
// 	virtual std::string ToString() override;
// 
// 
// 	virtual std::string GetClassName() override;
// 	virtual void DebugDraw() override;
// 	virtual void FixedUpdate() override;
// 
// 
// private:
// 	void UpdateClosestPoint();
// 	std::vector<Vector2> m_allPoints;
// 	Vector2 m_closestPoint;
// 
// };
// 
// class StandStill : public MovementAction
// {
// 
// public:
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 	virtual void FixedUpdate() override;
// 
// };
// 
// class RunAway : public MovementAction
// {
// 
// public:
// 	RunAway(const Vector2 target, MovementController *movementController, MapManager *mapManager);
// 	virtual std::string ToString() override;
// 	virtual std::string GetClassName() override;
// 	virtual void FixedUpdate() override;
// 
// };