#pragma once
#include "GameLoopObject.h"
#include "Math.h"

class BoundingBox :
	public GameLoopObject
{
public:
	BoundingBox();

	BoundingBox(Vector3 min_p, Vector3 max_p);

	virtual ~BoundingBox();
	
	bool Contains(const Vector3 & point) const;
	bool Collides(const BoundingBox & rhs_bb) const;

	inline Vector3 GetMinBB() const
	{
		return m_bb_min;
	}

	inline Vector3 GetMaxBB() const
	{
		return m_bb_max;
	}

	inline void SetMinBB(const Vector3 vec)
	{
		m_bb_min = vec;
	}

	inline void SetMaxBB(const Vector3 vec)
	{
		m_bb_max = vec;
	}

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;

	virtual std::string GetClassName() override;

private:
	Vector3 m_bb_min;
	Vector3 m_bb_max;
};
