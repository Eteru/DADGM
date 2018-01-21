#include "BVIntersections.h"

bool BVIntersections::TestIntersection(GameLoopObject *o1, GameLoopObject *o2, ContactInfo &result)
{
	if (nullptr == o1 || nullptr == o2)
	{
		PrintUtils::PrintI("Null object");
		return false;
	}

	if (nullptr == o1->GetParent() || nullptr == o2->GetParent())
	{
		PrintUtils::PrintI("Null parent");
		return false;
	}

	if ("PysicsBody" != o1->GetParent()->GetClassName() || "PysicsBody" != o2->GetParent()->GetClassName())
	{
		PrintUtils::PrintI("No PhysicsBody attached");
		return false;
	}

	if (o1->GetClassName() == "BoundingBox" && o2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(o1), dynamic_cast<BoundingBox *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingSphere" && o2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(dynamic_cast<BoundingSphere *>(o1), dynamic_cast<BoundingSphere *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingBox" && o2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(o1), dynamic_cast<BoundingSphere *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingSphere" && o2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(o2), dynamic_cast<BoundingSphere *>(o1), result);
	}

	PrintUtils::PrintI("Unknown bounding volumes");
	return false;
}

bool BVIntersections::TestIntersection(PhysicsBody *o1, PhysicsBody *o2, ContactInfo &result)
{
	if (nullptr == o1 || nullptr == o2)
	{
		PrintUtils::PrintI("Null object");
		return false;
	}

	if (!o1->m_kinematic && !o2->m_kinematic)
	{
		return false;
	}

	GameLoopObject *bv1;
	GameLoopObject *bv2;

	bv1 = o1->FindComponentPrefix("Bounding");
	bv2 = o2->FindComponentPrefix("Bounding");

	if (nullptr == bv1 || nullptr == bv2)
	{
		PrintUtils::PrintI("No bounding volumes found");
		return false;
	}

	if (bv1->GetClassName() == "BoundingBox" && bv2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(bv1), dynamic_cast<BoundingBox *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingSphere" && bv2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(dynamic_cast<BoundingSphere *>(bv1), dynamic_cast<BoundingSphere *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingBox" && bv2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(bv1), dynamic_cast<BoundingSphere *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingSphere" && bv2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(dynamic_cast<BoundingBox *>(bv2), dynamic_cast<BoundingSphere *>(bv1), result);
	}

	
	PrintUtils::PrintI("Unknown bounding volumes");
	return false;
}

Vector3 BVIntersections::ClosestPoint(Vector3 p, BoundingBox *box)
{
	Vector3 minCoords = box->m_transform.GetWorldPos() - box->m_halfExtents;
	Vector3 maxCoords = box->m_transform.GetWorldPos() + box->m_halfExtents;

	GLfloat resultX = std::min(std::max(p.x, minCoords.x), maxCoords.x);
	GLfloat resultY = std::min(std::max(p.y, minCoords.y), maxCoords.y);
	GLfloat resultZ = std::min(std::max(p.z, minCoords.z), maxCoords.z);

	return Vector3(resultX, resultY, resultZ);	
}

bool BVIntersections::TestIntersection(BoundingBox *box1, BoundingBox *box2, ContactInfo &result)
{
	if (std::abs(box1->m_transform.GetWorldPos().x - box2->m_transform.GetWorldPos().x) > box1->m_halfExtents.x + box2->m_halfExtents.x) return false;
	if (std::abs(box1->m_transform.GetWorldPos().y - box2->m_transform.GetWorldPos().y) > box1->m_halfExtents.y + box2->m_halfExtents.y) return false;
	if (std::abs(box1->m_transform.GetWorldPos().z - box2->m_transform.GetWorldPos().z) > box1->m_halfExtents.z + box2->m_halfExtents.z) return false;

	result.m_o1 = dynamic_cast<PhysicsBody *>(box1->GetParent());
	result.m_o2 = dynamic_cast<PhysicsBody *>(box2->GetParent());
	result.m_n = (box2->m_transform.GetWorldPos() - box1->m_transform.GetWorldPos()).Normalize();
	result.m_p = box1->m_transform.GetWorldPos() + result.m_n * box1->m_halfExtents;

	return true;
}

bool BVIntersections::TestIntersection(BoundingSphere *sphere1, BoundingSphere *sphere2, ContactInfo &result)
{
	if (sphere1->m_transform.GetWorldPos().Distance(sphere2->m_transform.GetWorldPos()) < sphere1->m_radius + sphere2->m_radius)
	{

		result.m_o1 = dynamic_cast<PhysicsBody *>(sphere1->GetParent());
		result.m_o2 = dynamic_cast<PhysicsBody *>(sphere2->GetParent());
		result.m_n = (sphere2->m_transform.GetWorldPos() - sphere1->m_transform.GetWorldPos()).Normalize();
		result.m_p = sphere1->m_transform.GetWorldPos() + result.m_n * sphere1->m_radius;
		return true;
	}

	return false;
}

bool BVIntersections::TestIntersection(BoundingBox *box, BoundingSphere *sphere, ContactInfo &result)
{
	Vector3 closestPointOnBox = ClosestPoint(sphere->m_transform.GetWorldPos(), box);

	Vector3 v = closestPointOnBox - sphere->m_transform.GetWorldPos();

	if (Math::Dot(v, v) <= sphere->m_radius * sphere->m_radius)
	{
		result.m_o1 = dynamic_cast<PhysicsBody *>(sphere->GetParent());;
		result.m_o2 = dynamic_cast<PhysicsBody *>(box->GetParent());;
		result.m_n = (closestPointOnBox - sphere->m_transform.GetWorldPos()).Normalize();
		result.m_p = sphere->m_transform.GetWorldPos() + result.m_n * sphere->m_radius;

		return true;
	}

	return false;
}
