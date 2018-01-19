#pragma once
#include "PhysicsBody.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

class BVIntersections
{
public:

	class ContactInfo
	{
	public:
		ContactInfo() : m_o1(nullptr), m_o2(nullptr), m_n(Vector3(0.f)), m_p(Vector3(0.f)) {}
		PhysicsBody * m_o1;
		PhysicsBody * m_o2;
		Vector3 m_n;
		Vector3 m_p;
	};

	static bool TestIntersection(PhysicsBody *o1, PhysicsBody *o2, ContactInfo &result);

private:


	static bool TestIntersection(GameLoopObject *o1, GameLoopObject *o2, ContactInfo &result);

	static bool TestIntersection(BoundingBox *box, BoundingSphere *sphere, ContactInfo &result);
	static bool TestIntersection(BoundingSphere *sphere1, BoundingSphere *sphere2, ContactInfo &result);
	static bool TestIntersection(BoundingBox *box1, BoundingBox *box2, ContactInfo &result);

	static Vector3 ClosestPoint(Vector3 p, BoundingBox *box);
};
