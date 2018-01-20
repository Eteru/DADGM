#pragma once
#include "Math.h"
#include "TimeLerp.h"

#include <vector>


class Transform
{
public:

	Transform(const Vector3 &pos = Vector3(0.f), const Vector3 &rot = Vector3(0.f), const Vector3 &scale = Vector3(1.f), bool relative = true);

	void SetParent(const Transform &parent);

	Vector3 GetLocalPos() const;
	Vector3 GetLocalRot() const;
	Vector3 GetLocalScale() const;

	Vector3 GetWorldPos() const;
	Vector3 GetWorldRot() const;
	Vector3 GetWorldScale() const;

	Vector3 GetWorldLerpPos();
	Vector3 GetWorldLerpRot();
	Vector3 GetWorldLerpScale();

	Vector3 GetLocalLerpPos();
	Vector3 GetLocalLerpRot();
	Vector3 GetLocalLerpScale();


	/* Parent values are always WORLD */
	void SetParentPos(const Vector3 val);
	/* Parent values are always WORLD */
	void SetParentRot(const Vector3 val);
	/* Parent values are always WORLD */
	void SetParentScale(const Vector3 val);

	/* LOCAL value is modified if m_relative is true. Otherwise WORLD */
	void SetPos(const Vector3 val);
	/* LOCAL value is modified if m_relative is true. Otherwise WORLD */
	void SetRot(const Vector3 val);
	/* LOCAL value is modified if m_relative is true. Otherwise WORLD */
	void SetScale(const Vector3 val);

	void SaveCurrentParams();

	void ResetLerps();
	void StepLerps(float time);

	void ComputeWorld();

	Matrix GetModel();
	Matrix GetLerpModel();

	bool m_relative;

private:

	class TransformParam
	{
	public:
		TransformParam();


		void Init(Vector3 local, Vector3 world, Vector3 parent, bool op);
		Vector3 GetLocalLerp();
		Vector3 GetWorldLerp();

		void SaveParam();
		void ResetLerps();
		void StepLerps(const float time);
		void ComputeWorld();

		bool m_op; //false = addition, true = multiplication

		Vector3 m_currentLocal;
		Vector3 m_currentWorld;
		Vector3 m_currentParentWorld;

	private:
		Vec3TimeLerp m_localLerp;
		Vec3TimeLerp m_worldLerp;


		Vector3 m_lastLocal;
		Vector3 m_lastWorld;
	};

	enum ParamIDS { POSITION = 0, ROTATION = 1, SCALE = 2, NUM = 3};

	std::vector<TransformParam> m_params;
};
