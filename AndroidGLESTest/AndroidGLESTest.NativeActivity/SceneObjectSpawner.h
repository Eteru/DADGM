#pragma once

#include "Structs.h"
#include "VisualBody.h"

#include <map>

class SceneObjectSpawner
{
public:
	SceneObjectSpawner(const std::string & model_id, const std::string & shader_id);
	virtual ~SceneObjectSpawner();

	VisualBody *SpawnObject(const Vector3 pos, const std::vector<std::string> & texture_ids, GameLoopObject *parent);

private:
	Model *m_model;
	Shader *m_shader;
};

