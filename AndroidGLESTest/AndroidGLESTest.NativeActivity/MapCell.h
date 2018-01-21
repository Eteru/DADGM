#pragma once

#include "GameLoopObject.h"

class MapCell : public GameLoopObject
{

public:
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;
};