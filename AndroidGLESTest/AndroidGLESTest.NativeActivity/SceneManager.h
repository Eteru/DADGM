#pragma once

#include <map>
#include <string>
#include <iostream>

#include "VisualBody.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Fog.h"
#include "AmbientalLight.h"
#include "LightSource.h"

class SceneManager : public GameLoopObject
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	virtual void Destroy() override;
	void SetEngine(engine *eng);
	bool LoadFromFile(std::string filepath);

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	inline const Fog & GetFog() const
	{
		return m_fog;
	}

	inline const AmbientalLight & GetAmbientalLight() const
	{
		return m_ambiental_light;
	}

// 	inline const std::map<std::string, LightSource *> & GetLights() const
// 	{
// 		return m_lights;
// 	}
// 
// 	inline LightSource *GetLightSource(std::string id)
// 	{
// 		if (m_lights.find(id) == m_lights.end()) {
// 			return nullptr;
// 		}
// 
// 		return m_lights[id];
// 	}
// 
	inline Camera *GetActiveCamera()
	{
		return m_activeCamera;
	}

// 
// 	inline SceneObject *GetSceneObject(std::string id)
// 	{
// 		if (m_objects.find(id) == m_objects.end()) {
// 			return nullptr;
// 		}
// 
// 		return m_objects[id];
// 	}

	inline const engine *GetEngine() const
	{
		return m_engine;
	}



private:

	bool ParseBackgroundColor(rapidxml::xml_node<> *pRoot);
	bool ParseCameras(rapidxml::xml_node<> *pRoot);
	bool ParseCamera(rapidxml::xml_node<> *pCamera);

	bool ParseFog(rapidxml::xml_node<> *pRoot);
	bool ParseLights(rapidxml::xml_node<> *pRoot);
	bool ParseLight(rapidxml::xml_node<> *pLight);

	bool ParseObjects(rapidxml::xml_node<> *pRoot);
	bool ParseObject(rapidxml::xml_node<> *pObject);

	bool ParseLinks(rapidxml::xml_node<> *pRoot);
	bool ParseObjectLinks(rapidxml::xml_node<> *pNode);
	bool ParseLightLinks(rapidxml::xml_node<> *pNode);
	bool ParseCameraLinks(rapidxml::xml_node<> *pNode);

	static SceneManager *m_instance;

	engine *m_engine;
	Vector3 m_background_color;
	//std::map<std::string, Camera *> m_cameras;
	//std::map<std::string, SceneObject *> m_objects;
	//std::map<std::string, LightSource *> m_lights;
	Camera *m_activeCamera;
	//std::string m_active_camera;

	Fog m_fog;
	AmbientalLight m_ambiental_light;

	SceneManager();
};
