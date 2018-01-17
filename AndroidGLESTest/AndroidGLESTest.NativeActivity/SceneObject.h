#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Trajectory.h"
#include "Math.h"
#include "GameLoopObject.h"
#include "BoundingBox.h"
class SceneObject : public GameLoopObject
{
public:
	SceneObject(Vector3 pos, Vector3 rot, Vector3 scale, std::string name, bool depth_test);
	virtual ~SceneObject();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	void SetWired(bool is_wired);
	void SetBlend(bool use_blend);
	void SetModel(Model *model);
	void SetShader(Shader *shader);
	void SetTrajectory(Trajectory *trajectory);
	void AddTexture(Texture *texture);
	//void AddLightID(std::string id);

	inline const BoundingBox GetBB()
	{
		m_bb.SetMinBB(Vector3(Vector4(m_bb.GetMinBB(), 1.f) * m_M));
		m_bb.SetMaxBB(Vector3(Vector4(m_bb.GetMaxBB(), 1.f) * m_M));

		return m_bb;
	}

	inline const std::string & GetName() const
	{
		return m_name;
	}

	inline Vector3 GetPosition() const
	{
		return m_position;
	}

// 	virtual void Init();
// 	virtual void Update();
// 	virtual void Draw();
	virtual bool Collides(SceneObject * obj);
	virtual bool Contains(const Vector3 & point);



protected:
	bool m_depth_test;
	bool m_is_wired;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	Matrix m_M;
	Model *m_model;
	Shader *m_shader;
	Trajectory *m_trajectory;
	std::vector<Texture *> m_textures;
	//std::vector<std::string> m_light_ids;

	std::string m_name;
	BoundingBox m_bb;

	void SharedDrawElements();
	void GeneralUpdate();
};
