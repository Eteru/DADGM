
#include "SceneObjectSpawner.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "UniqueID.h"

SceneObjectSpawner::SceneObjectSpawner(const std::string & model_id, const std::string & shader_id)
{
	ResourceManager *rm = ResourceManager::GetInstance();

	m_model = rm->LoadModel(model_id);
	m_shader = rm->LoadShader(shader_id);
}

SceneObjectSpawner::~SceneObjectSpawner()
{
	// resources are eliminated by the managers
}

VisualBody * SceneObjectSpawner::SpawnObject(const Vector3 pos, const std::vector<std::string>& texture_ids)
{
	if (nullptr == m_model ||
		nullptr == m_shader)
	{
		return nullptr;
	}

	VisualBody *so = new VisualBody(pos, Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 0.5f), "SpawnedObject", true);

	so->SetModel(m_model);
	so->SetShader(m_shader);

	ResourceManager *rm = ResourceManager::GetInstance();
	for (const std::string & id : texture_ids)
	{
		Texture *tex = rm->LoadTexture(id);

		if (nullptr != tex)
		{
			so->AddTexture(tex);
		}
	}

	so->SetID(UniqueID::GetID(so->GetClassName()));

	SceneManager::GetInstance()->AddComponent(so);

	return so;
}
