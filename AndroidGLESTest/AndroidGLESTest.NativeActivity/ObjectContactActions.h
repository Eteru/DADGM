#pragma once
#include "BVIntersections.h"

class GameLoopObject;
class Robot;
class Projectile;
class MapCell;


class ObjectContactActions
{
public:

	static void OnContact(BVIntersections::ContactInfo contactInfo);

private:
	static void OnContact(Robot *robot1, Robot *robot2, BVIntersections::ContactInfo contactInfo);
	static void OnContact(Robot *robot, Projectile *proj, BVIntersections::ContactInfo contactInfo);
	static void OnContact(Robot *robot, MapCell *mapCell, BVIntersections::ContactInfo contactInfo);
	static void OnContact(Projectile *proj, MapCell *mapCell, BVIntersections::ContactInfo contactInfo);
};