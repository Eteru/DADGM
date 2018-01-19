#pragma once
#include "GameLoopObject.h"
#include "Math.h"

class BoundingBox :
	public GameLoopObject
{
public:
	BoundingBox();
	BoundingBox(Vector3 halfExtents);

	virtual ~BoundingBox();
	virtual std::string ToString() override;

	virtual std::string GetClassName() override;

	Vector3 m_halfExtents;
private:
};
