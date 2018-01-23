#pragma once

#include "GameLoopObject.h"

class MovementController : public GameLoopObject
{
public:

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	void SetPath(std::vector<Vector2> path);
	Vector2 NextPoint();
	void ArrivedAtCheckpoint();
	bool IsEmpty();

private:
	std::vector<Vector2> m_path;
};
