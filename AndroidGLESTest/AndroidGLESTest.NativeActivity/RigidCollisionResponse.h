#pragma once

#include "BVIntersections.h"

namespace RigidCollisionResponse
{
	void ApplyImpulses(const BVIntersections::ContactInfo &contact, const float restitution);
}
