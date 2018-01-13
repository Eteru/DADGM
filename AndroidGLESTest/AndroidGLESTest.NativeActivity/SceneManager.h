#pragma once

#include <map>
#include <string>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Fog.h"
#include "AmbientalLight.h"
#include "LightSource.h"

class SceneManager
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	void SetEngine(engine *eng);
	bool LoadFromFile(std::string filepath);
	void Update();
	void Draw();
	void CleanUp();

	inline const Fog & GetFog() const
	{
		return m_fog;
	}

	inline const AmbientalLight & GetAmbientalLight() const
	{
		return m_ambiental_light;
	}

	inline const std::map<std::string, LightSource *> & GetLights() const
	{
		return m_lights;
	}

	inline LightSource *GetLightSource(std::string id)
	{
		if (m_lights.find(id) == m_lights.end()) {
			return nullptr;
		}

		return m_lights[id];
	}

	inline Camera *GetActiveCamera()
	{
		return m_cameras[m_active_camera];
	}

	inline SceneObject *GetSceneObject(std::string id)
	{
		if (m_objects.find(id) == m_objects.end()) {
			return nullptr;
		}

		return m_objects[id];
	}

	inline const engine *GetEngine() const
	{
		return m_engine;
	}

private:
	static SceneManager *m_instance;

	engine *m_engine;
	Vector3 m_background_color;
	std::map<std::string, Camera *> m_cameras;
	std::map<std::string, SceneObject *> m_objects;
	std::map<std::string, LightSource *> m_lights;
	std::string m_active_camera;

	Fog m_fog;
	AmbientalLight m_ambiental_light;

	SceneManager();
};

