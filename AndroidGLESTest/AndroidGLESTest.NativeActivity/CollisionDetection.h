#pragma once
#include "BVIntersections.h"

class CollisionDetection
{
public:
	static std::vector<BVIntersections::ContactInfo> DetectCollisions(std::vector<GameLoopObject*> &allBodies);
};
