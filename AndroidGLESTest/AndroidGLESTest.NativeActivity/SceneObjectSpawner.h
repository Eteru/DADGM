#pragma once

#include "Structs.h"
#include "SceneObject.h"

#include <map>

class SceneObjectSpawner
{
public:
	SceneObjectSpawner(const std::string & model_id, const std::string & shader_id);
	virtual ~SceneObjectSpawner();

	SceneObject *SpawnObject(const Vector3 pos, const std::vector<std::string> & texture_ids);

private:
	Model *m_model;
	Shader *m_shader;
};

