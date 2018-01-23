#include "RigidCollisionResponse.h"

void RigidCollisionResponse::ApplyImpulses(const BVIntersections::ContactInfo &contact)
{

	static const float restitution = 0.9f;


	Vector3 p1 = contact.m_o1->m_transform.GetWorldPos();
	Vector3 p2 = contact.m_o2->m_transform.GetWorldPos();

	Vector3 v1 = contact.m_o1->m_linearVelEngine + contact.m_o1->m_linearVelImpact;
	Vector3 v2 = contact.m_o2->m_linearVelEngine + contact.m_o2->m_linearVelImpact;

	Vector3 r1 = contact.m_p - p1;
	Vector3 r2 = contact.m_p - p2;

	Vector3 om1 = contact.m_o1->m_angularVelEngine + contact.m_o1->m_angularVelImpact;
	Vector3 om2 = contact.m_o2->m_angularVelEngine + contact.m_o2->m_angularVelImpact;

	Vector3 vp1 = v1 + Math::Cross(om1, r1);
	Vector3 vp2 = v2 + Math::Cross(om2, r2);

	Vector3 vr = vp2 - vp1;

	Vector3 n = contact.m_n;

	float invTens1 = 1.f / contact.m_o1->m_inertia;
	float invTens2 = 1.f / contact.m_o2->m_inertia;

	Vector3 crs1 = Math::Cross(r1, contact.m_n);
	Vector3 term1 = invTens1 * Math::Cross(crs1, r1);

	Vector3 crs2 = Math::Cross(r2, contact.m_n);
	Vector3 term2 = invTens2 * Math::Cross(crs2, r2);

	float m1 = contact.m_o1->m_mass;
	float m2 = contact.m_o2->m_mass;

	float im1 = 1.f / m1;
	float im2 = 1.f / m2;

	float jr = Math::Dot(-(1 + restitution) * vr, contact.m_n) / (im1 + im2 + Math::Dot(term1 + term2, contact.m_n));

	Vector3 jrVec = contact.m_n * jr;

	if (!contact.m_o1->m_kinematic)
	{
		contact.m_o1->m_linearVelImpact -= jrVec * im1;
		contact.m_o1->m_angularVelImpact = om1 - jr * invTens1 * crs1;
	}

	if (!contact.m_o2->m_kinematic)
	{
		contact.m_o2->m_linearVelImpact += jrVec * im2;
		contact.m_o2->m_angularVelImpact = om2 + jr * invTens2 * crs2;
	}
}
