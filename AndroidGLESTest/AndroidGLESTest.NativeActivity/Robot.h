#pragma once

#include "VisualBody.h"
#include "PhysicsBody.h"

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

private:
};