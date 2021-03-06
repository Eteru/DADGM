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
		return TestIntersection(static_cast<BoundingBox *>(o1), static_cast<BoundingBox *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingSphere" && o2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(static_cast<BoundingSphere *>(o1), static_cast<BoundingSphere *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingBox" && o2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(static_cast<BoundingBox *>(o1), static_cast<BoundingSphere *>(o2), result);
	}

	if (o1->GetClassName() == "BoundingSphere" && o2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(static_cast<BoundingBox *>(o2), static_cast<BoundingSphere *>(o1), result);
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

	if (o1->m_kinematic && o2->m_kinematic)
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
		return TestIntersection(static_cast<BoundingBox *>(bv1), static_cast<BoundingBox *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingSphere" && bv2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(static_cast<BoundingSphere *>(bv1), static_cast<BoundingSphere *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingBox" && bv2->GetClassName() == "BoundingSphere")
	{
		return TestIntersection(static_cast<BoundingBox *>(bv1), static_cast<BoundingSphere *>(bv2), result);
	}

	if (bv1->GetClassName() == "BoundingSphere" && bv2->GetClassName() == "BoundingBox")
	{
		return TestIntersection(static_cast<BoundingBox *>(bv2), static_cast<BoundingSphere *>(bv1), result);
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
	Vector3 dir = (box2->m_transform.GetWorldPos() - box1->m_transform.GetWorldPos());

	if (0 == Math::Length(dir))
	{
		return false;
	}

	if (std::abs(box1->m_transform.GetWorldPos().x - box2->m_transform.GetWorldPos().x) > box1->m_halfExtents.x + box2->m_halfExtents.x) return false;
	if (std::abs(box1->m_transform.GetWorldPos().y - box2->m_transform.GetWorldPos().y) > box1->m_halfExtents.y + box2->m_halfExtents.y) return false;
	if (std::abs(box1->m_transform.GetWorldPos().z - box2->m_transform.GetWorldPos().z) > box1->m_halfExtents.z + box2->m_halfExtents.z) return false;

	result.m_o1 = static_cast<PhysicsBody *>(box1->GetParent());
	result.m_o2 = static_cast<PhysicsBody *>(box2->GetParent());
	result.m_n = dir.Normalize();
	result.m_p = box1->m_transform.GetWorldPos() + result.m_n * box1->m_halfExtents;

	return true;
}

bool BVIntersections::TestIntersection(BoundingSphere *sphere1, BoundingSphere *sphere2, ContactInfo &result)
{
	if (sphere1->m_transform.GetWorldPos().Distance(sphere2->m_transform.GetWorldPos()) < sphere1->m_radius + sphere2->m_radius)
	{
		Vector3 dir = sphere2->m_transform.GetWorldPos() - sphere1->m_transform.GetWorldPos();

		if (0 == Math::Length(dir))
		{
			return false;
		}

		result.m_o1 = static_cast<PhysicsBody *>(sphere1->GetParent());
		result.m_o2 = static_cast<PhysicsBody *>(sphere2->GetParent());
		result.m_n = dir.Normalize();
		result.m_p = sphere1->m_transform.GetWorldPos() + result.m_n * sphere1->m_radius;
		return true;
	}

	return false;
}

bool BVIntersections::TestIntersection(BoundingBox *box, BoundingSphere *sphere, ContactInfo &result)
{
	Vector3 closestPointOnBox = ClosestPoint(sphere->m_transform.GetWorldPos(), box);

	Vector3 v = closestPointOnBox - sphere->m_transform.GetWorldPos();
	float dist = v.Length();

	Vector3 dir = box->m_transform.GetWorldPos() - sphere->m_transform.GetWorldPos();
	if (dist == 0.f && Math::Length(dir) > 0)
	{
		result.m_o1 = static_cast<PhysicsBody *>(sphere->GetParent());
		result.m_o2 = static_cast<PhysicsBody *>(box->GetParent());
		result.m_n = Math::Normalize(dir);

		result.m_o1->m_transform.SetPos(result.m_o1->m_transform._GetLastWorldPos());

		result.m_p = sphere->m_transform.GetWorldPos() + result.m_n * (sphere->m_radius);
		return true;
	}
	else if (dist < sphere->m_radius && dist > 0)
	{	
		result.m_o1 = static_cast<PhysicsBody *>(sphere->GetParent());
		result.m_o2 = static_cast<PhysicsBody *>(box->GetParent());
		result.m_n = v.Normalize();
		result.m_p = closestPointOnBox;

		result.m_o1->m_transform.SetPos(result.m_o1->m_transform.GetWorldPos() - result.m_n * (sphere->m_radius - dist));

		return true;
	}

	return false;
}
