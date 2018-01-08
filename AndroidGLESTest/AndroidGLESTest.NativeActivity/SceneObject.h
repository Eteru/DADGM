#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Trajectory.h"
#include "Math.h"

class SceneObject
{
public:
	SceneObject(Vector3 pos, Vector3 rot, Vector3 scale, bool depth_test, std::string id);
	virtual ~SceneObject();

	void SetWired(bool is_wired);
	void SetBlend(bool use_blend);
	void SetModel(Model *model);
	void SetShader(Shader *shader);
	void SetTrajectory(Trajectory *trajectory);
	void AddTexture(Texture *texture);
	void AddLightID(std::string id);

	inline const BoundingBox GetBB()
	{
		BoundingBox bb = m_model->GetBB();

		bb.bb_min = Vector3(Vector4(bb.bb_min, 1.0) * m_M);
		bb.bb_max = Vector3(Vector4(bb.bb_max, 1.0) * m_M);

		return bb;
	}

	inline const std::string & GetName() const
	{
		return m_id;
	}

	inline Vector3 GetPosition() const
	{
		return m_position;
	}

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual bool Collides(SceneObject * obj);
	virtual bool Contains(const Vector3 & point);

protected:
	bool m_depth_test;
	bool m_is_wired;
	std::string m_id;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	Matrix m_M;
	Model *m_model;
	Shader *m_shader;
	Trajectory *m_trajectory;
	std::vector<Texture *> m_textures;
	std::vector<std::string> m_light_ids;

	void SharedDrawElements();
	void GeneralUpdate();
};
