#pragma once




namespace DeltaTime
{
	// FixedUpdate de 30 ori pe secunda
	const float PHYSICS_TIME_STEP_MS = 1000.f / 30.f;

	const float PHYSICS_TIME_STEP = PHYSICS_TIME_STEP_MS / 1000.f;

	static float m_dt;

	float GetDt();
	void SetDt(float ovalue);
}