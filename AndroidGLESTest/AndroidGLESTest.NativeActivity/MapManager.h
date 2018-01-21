#pragma once

#include "GameLoopObject.h"

class MapManager : public GameLoopObject
{

public:
	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

};