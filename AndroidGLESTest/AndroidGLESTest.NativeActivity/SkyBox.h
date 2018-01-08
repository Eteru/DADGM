#pragma once
#include "SceneObject.h"
class SkyBox :
	public SceneObject
{
public:
	SkyBox(Vector3 pos, Vector3 rot, Vector3 scale, std::string id,
		float offset, float size);
	~SkyBox();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual bool Collides(SceneObject * obj) override;
	virtual bool Contains(const Vector3 & point) override;

private:
	float m_offsetY;
	float m_size;
	float m_half_size;
};

