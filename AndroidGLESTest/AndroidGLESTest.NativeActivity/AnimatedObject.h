#pragma once
#include "SceneObject.h"

class AnimatedObject :
	public SceneObject
{
public:
	AnimatedObject(Vector3 pos, Vector3 rot, Vector3 scale, bool depth_test, std::string id, float displacement);
	~AnimatedObject();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	float m_displacement_max;
	float m_time;
};

