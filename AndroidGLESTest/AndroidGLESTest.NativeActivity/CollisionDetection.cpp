#include "CollisionDetection.h"

std::vector<BVIntersections::ContactInfo> CollisionDetection::DetectCollisions(std::vector<GameLoopObject*> &allBodies)
{
	std::vector<BVIntersections::ContactInfo> result;


	// Naive method works - we only have tens of objects
	for (int i = 0; i < allBodies.size(); ++i)
	{	
		PhysicsBody *pb1 = static_cast<PhysicsBody *>(allBodies[i]);

		for (int j = 0; j < i; ++j)
		{
			PhysicsBody *pb2 = static_cast<PhysicsBody *>(allBodies[j]);

			BVIntersections::ContactInfo contact;

			if (BVIntersections::TestIntersection(pb1, pb2, contact))
			{
				result.push_back(contact);
			}
		}
	}

	return result;
}
