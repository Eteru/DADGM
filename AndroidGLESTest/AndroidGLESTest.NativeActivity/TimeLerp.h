#pragma once

#include "Math.h"


class Vec3TimeLerp
{
public:
	Vec3TimeLerp() {}
	Vec3TimeLerp(Vector3 start, Vector3 end);

	Vector3 GetValue();
	Vector3 Step(const GLfloat t);

private:
	Vector3 m_start;
	Vector3 m_end;
	GLfloat m_crtCoef;
};

class FloatTimeLerp
{
public:
	FloatTimeLerp(GLfloat start, GLfloat end);

	GLfloat Step(const GLfloat t);
	GLfloat GetValue();


private:
	GLfloat m_start;
	GLfloat m_end;
	GLfloat m_crtCoef;
};
