#pragma once
#include "GameLoopObject.h"

class GameManager : public GameLoopObject
{
public:
	GameManager();
	virtual ~GameManager();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;

	virtual std::string GetClassName() override;

};
