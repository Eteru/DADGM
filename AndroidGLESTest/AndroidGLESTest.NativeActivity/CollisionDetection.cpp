#include "CollisionDetection.h"

std::vector<BVIntersections::ContactInfo> CollisionDetection::DetectCollisions(std::vector<GameLoopObject*> &allBodies)
{
	std::vector<BVIntersections::ContactInfo> result;


	// Naive method works - we only have tens of objects
	for (auto obj1 : allBodies)
	{	
		PhysicsBody *pb1 = dynamic_cast<PhysicsBody *>(obj1);
		for (auto obj2 : allBodies)
		{
			PhysicsBody *pb2 = dynamic_cast<PhysicsBody *>(obj2);

			if (pb1 == pb2)
				continue;

			BVIntersections::ContactInfo contact;

			if (BVIntersections::TestIntersection(pb1, pb2, contact))
			{
				result.push_back(contact);
			}
		}
	}

	return result;
}
