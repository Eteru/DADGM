#pragma once

#include "Math.h"


class Vec3TimeLerp
{
public:
	Vec3TimeLerp() {}
	Vec3TimeLerp(Vector3 start, Vector3 end);

	Vector3 GetValue() const;
	Vector3 Step(const GLfloat t);

private:
	Vector3 m_value;
	Vector3 m_start;
	Vector3 m_end;
	GLfloat m_crtCoef;
};

class FloatTimeLerp
{
public:
	FloatTimeLerp(GLfloat start, GLfloat end);

	GLfloat Step(const GLfloat t);
	GLfloat GetValue() const;


private:
	GLfloat m_value;
	GLfloat m_start;
	GLfloat m_end;
	GLfloat m_crtCoef;
};
