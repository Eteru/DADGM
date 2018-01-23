#pragma once
#include "GameLoopObject.h"

class BoundingSphere : public GameLoopObject
{
public:
	BoundingSphere();
	BoundingSphere(GLfloat radius);
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	GLfloat m_radius;

	virtual void Draw() override;

private:
};