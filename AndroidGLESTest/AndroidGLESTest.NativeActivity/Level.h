#pragma once
#include "GameLoopObject.h"

class Level : public GameLoopObject
{
public:


	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

};
