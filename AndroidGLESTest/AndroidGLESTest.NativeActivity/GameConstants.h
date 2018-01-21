#pragma once
#include "Math.h"

namespace GameConstants
{
	// FixedUpdate de 30 ori pe secunda
	const float PHYSICS_TIME_STEP_MS = 1000.f / 30.f;
	const float PHYSICS_TIME_STEP = PHYSICS_TIME_STEP_MS / 1000.f;

	const Vector3 DEFAULT_FORWARD(0.f, 0.f, -1.f);
	const Vector3 DEFAULT_RIGHT(1.f, 0.f, 0.f);
	const Vector3 DEFAULT_UP(0.f, 1.f, 0.f);

	const float CELL_SIZE = 1.f;
	const float GROUND_HEIGHT = 0.f;
	const float WALL_HEIGHT = CELL_SIZE;
	const float CAMERA_HEIGHT = 10.f * CELL_SIZE;

}
