
#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: m_bb_min(Vector3(0, 0, 0)), m_bb_max(Vector3(0, 0, 0))
{}

BoundingBox::BoundingBox(Vector3 min_p, Vector3 max_p)
	: m_bb_min(min_p), m_bb_max(max_p)
{}


BoundingBox::~BoundingBox()
{
}

bool BoundingBox::Contains(const Vector3 & point) const
{
	if (m_bb_min.x > point.x) return false;
	if (m_bb_min.y > point.y) return false;
	if (m_bb_min.z > point.z) return false;
	if (m_bb_max.x < point.x) return false;
	if (m_bb_max.y < point.y) return false;
	if (m_bb_max.z < point.z) return false;

	return true;
}

bool BoundingBox::Collides(const BoundingBox & rhs_bb) const
{
	if (m_bb_min.x > rhs_bb.m_bb_max.x) return false;
	if (m_bb_min.y > rhs_bb.m_bb_max.y) return false;
	if (m_bb_min.z > rhs_bb.m_bb_max.z) return false;
	if (m_bb_max.x < rhs_bb.m_bb_min.x) return false;
	if (m_bb_max.y < rhs_bb.m_bb_min.y) return false;
	if (m_bb_max.z < rhs_bb.m_bb_min.z) return false;

	return true;
}

void BoundingBox::Init()
{
}

void BoundingBox::FixedUpdate()
{
}

void BoundingBox::Update()
{
}

void BoundingBox::Draw()
{
	// this should be done just for debug purposes
	// no implementation yet
}

void BoundingBox::Destroy()
{
}

std::string BoundingBox::ToString()
{
	return "BoundingBox: Min values: " +
			m_bb_min.ToString() + " Max values: " +
			m_bb_max.ToString();
}

std::string BoundingBox::GetClassName()
{
	return "BoundingBox";
}
