#pragma once

#include <cmath>
#include <GLES2\gl2.h>

#include <string>

//Vector2

class Vector2
{
public:
	//Constructors
	Vector2() : x(0.0f), y(0.0f){}
	Vector2(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
	Vector2(GLfloat * pArg) : x(pArg[0]), y(pArg[1]) {}
	Vector2(const Vector2 & vector) : x(vector.x), y(vector.y) {}
	Vector2(const GLfloat uniformValue) : x(uniformValue), y(uniformValue) {}

	//Vector's operations
	GLfloat Length();
	Vector2 & Normalize();
	Vector2 operator + (Vector2 & vector);
	Vector2 & operator += (Vector2 & vector);
	Vector2 operator - ();
	Vector2 operator - (Vector2 & vector);
	Vector2 & operator -= (Vector2 & vector);
	Vector2 operator * (GLfloat k);
	Vector2 & operator *= (GLfloat k);
	Vector2 operator / (GLfloat k);
	Vector2 & operator /= (GLfloat k);
	Vector2 & operator = (const Vector2 & vector);
	Vector2 Modulate(Vector2 & vector);
	GLfloat Dot(Vector2 & vector);

	std::string ToString();

	//access to elements
	GLfloat operator [] (unsigned int idx);

	//data members
	GLfloat x;
	GLfloat y;
};

//Vector3

class Vector4;

class Vector3
{
public:
	//Constructors
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
	Vector3(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]) {}
	Vector3(const Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
	Vector3(const Vector4 & vector);
	Vector3(const GLfloat uniformValue) : x(uniformValue), y(uniformValue), z(uniformValue) {}
	
	//Vector's operations
	GLfloat Length();
	Vector3 & Normalize();
	//Vector3 operator + (Vector3 & vector);
	Vector3 & operator += (const Vector3 & vector);
	Vector3 & operator -= (const Vector3 & vector);
	Vector3 & operator *= (const Vector3 & vector);
	Vector3 & operator /= (const Vector3 & vector);

	Vector3 & operator += (const GLfloat k);
	Vector3 & operator -= (const GLfloat k);
	Vector3 & operator *= (const GLfloat k);
	Vector3 & operator /= (const GLfloat k);

	Vector3 operator - ();
	//Vector3 operator - (Vector3 & vector);
	//Vector3 operator * (GLfloat k);
	//Vector3 operator + (GLfloat k);
	//Vector3 operator - (GLfloat k);
	//Vector3 operator / (GLfloat k);

	Vector3 & operator = (const Vector3 & vector);


	Vector3 Modulate(Vector3 & vector);
	GLfloat Distance(const Vector3 & vector);
	GLfloat Dot(Vector3 & vector);
	Vector3 Cross(Vector3 & vector);

	std::string ToString();

	//access to elements
	GLfloat operator [] (unsigned int idx);

	// data members
	GLfloat x;
	GLfloat y;
	GLfloat z;

};

inline Vector3 operator + (Vector3 lhs, const Vector3 &rhs)
{
	lhs += rhs;
	return lhs;
}

inline Vector3 operator - (Vector3 lhs, const Vector3 &rhs)
{
	lhs -= rhs;
	return lhs;
}

inline Vector3 operator * (Vector3 lhs, const Vector3 &rhs)
{
	lhs *= rhs;
	return lhs;
}

inline Vector3 operator / (Vector3 lhs, const Vector3 &rhs)
{
	lhs /= rhs;
	return lhs;
}

inline Vector3 operator + (Vector3 lhs, const GLfloat &rhs)
{
	lhs += rhs;
	return lhs;
}

inline Vector3 operator - (Vector3 lhs, const GLfloat &rhs)
{
	lhs -= rhs;
	return lhs;
}

inline Vector3 operator * (Vector3 lhs, const GLfloat &rhs)
{
	lhs *= rhs;
	return lhs;
}

inline Vector3 operator / (Vector3 lhs, const GLfloat &rhs)
{
	lhs /= rhs;
	return lhs;
}

//Vector4

class Matrix;

class Vector4
{
public:
	//Constructors
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z), w(1.0f) {}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]), w(pArg[3]) {}
	Vector4(const Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0f){}
	Vector4(const Vector3 & vector, GLfloat _w) : x(vector.x), y(vector.y), z(vector.z), w(_w) {}
	Vector4(const Vector4 & vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
	Vector4(const GLfloat uniformValue) : x(uniformValue), y(uniformValue), z(uniformValue), w(uniformValue) {}

	//Vector's operations
	GLfloat Length();
	Vector4 & Normalize();
	Vector4 operator + (Vector4 & vector);
	Vector4 & operator += (Vector4 & vector);
	Vector4 operator - ();
	Vector4 operator - (Vector4 & vector);
	Vector4 & operator -= (Vector4 & vector);
	Vector4 operator * (GLfloat k);
	Vector4 & operator *= (GLfloat k);
	Vector4 operator / (GLfloat k);
	Vector4 & operator /= (GLfloat k);
	Vector4 & operator = (const Vector4 & vector);
	Vector4 Modulate(Vector4 & vector);
	GLfloat Dot(Vector4 & vector);

	//matrix multiplication
	Vector4 operator * ( Matrix & m );

	//access to elements
	GLfloat operator [] (unsigned int idx);

	//data members
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;	
};

//Matrix 4 X 4


class Matrix
{
public:
	//constructors
	Matrix() {}
	Matrix(GLfloat val);
	Matrix(const Matrix & mat);

	// Matrix operations
	Matrix & SetZero();
	Matrix & SetIdentity();

	Matrix & SetRotationX(GLfloat angle);
	Matrix & SetRotationY(GLfloat angle);
	Matrix & SetRotationZ(GLfloat angle);
	Matrix & SetRotationAngleAxis( float angle, float x, float y, float z );

	Matrix & SetScale(GLfloat scale);
	Matrix & SetScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	Matrix & SetScale(GLfloat * pScale);
	Matrix & SetScale(Vector3 &scaleVec);

	Matrix & SetTranslation(GLfloat x, GLfloat y, GLfloat z);
	Matrix & SetTranslation(GLfloat *pTrans);
	Matrix & SetTranslation(Vector3 &vec);

	Matrix & SetPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);

	Matrix Transpose();
	Matrix Inverse();

	Matrix operator + (Matrix & mat);
	Matrix & operator += (Matrix & mat);
	Matrix operator - (Matrix & mat);
	Matrix &operator -= (Matrix & mat);

	Matrix operator * (const Matrix & mat);
	Matrix operator * (GLfloat k);
	Matrix & operator *= (GLfloat k);

	Vector4 operator * (Vector4 & vec);

	Matrix & operator = (const Matrix & mat);

	//data members
	GLfloat m[4][4];
};


class Math
{
public:

	static Vector3 RotateAround(const Vector3 &v, const Vector3 &axis, const float rads)
	{

		GLfloat cosA = std::cos(rads);
		GLfloat sinA = std::cos(rads);

		Vector3 crs = Cross(axis, v);

		return v * cosA + crs * sinA + (1 - cosA) * Dot(axis, v) * axis;
	}

	static Vector3 RotateTowards(Vector3 &v1, Vector3 &v2, const float rads)
	{
		Vector3 crs = Cross(v1, v2);

		return RotateAround(v1, Normalize(crs), rads);		
	}

	static float Clamp(const GLfloat value, const GLfloat min, const GLfloat max)
	{
		return value < min ? min : value > max ? max : value;
	}

	static GLfloat Lerp(const GLfloat a, const GLfloat b, const float t)
	{
		float tClamped = Clamp(t, 0.f, 1.f);
		return (1 - tClamped) * a + tClamped * b;
	}

	static Vector2 Lerp(const Vector2 &a, const Vector2 &b, const GLfloat t)
	{
		return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
	}

	static Vector3 Lerp(const Vector3 &a, const Vector3 &b, const GLfloat t)
	{
		return Vector3(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t));
	}	
	
	static GLfloat Length(const Vector2 v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	static GLfloat Length(const Vector3 v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	static GLfloat Dot(const Vector2 v1, const Vector2 v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static GLfloat Dot(const Vector3 v1, const Vector3 v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static Vector3 Cross(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	
	static GLfloat Distance(const Vector2 v1, const Vector2 v2)
	{
		return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	static GLfloat Distance(const Vector3 v1, const Vector3 v2)
	{
		return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	}

	static Vector3 Normalize(Vector3 &v)
	{
		return v.Normalize();
	}

private:
	Math() {}
};