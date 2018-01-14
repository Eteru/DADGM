
#include "SceneManager.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "AnimatedObject.h"

#include <android/asset_manager.h>

#include <unordered_map>

#include "Structs.h"
#include "XMLUtils.h"
#include "PrintUtils.h"

SceneManager *SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	for (auto it = m_objects.begin(); it != m_objects.end();) {
		delete it->second;
		it->second = nullptr;
		it = m_objects.erase(it);
	}

	for (auto it = m_cameras.begin(); it != m_cameras.end();) {
		delete it->second;
		it->second = nullptr;
		it = m_cameras.erase(it);
	}

	for (auto it = m_lights.begin(); it != m_lights.end();) {
		delete it->second;
		it->second = nullptr;
		it = m_lights.erase(it);
	}
}

SceneManager * SceneManager::GetInstance()
{
	if (nullptr == m_instance) {
		m_instance = new SceneManager;
	}

	return m_instance;
}

void SceneManager::SetEngine(engine * eng)
{
	m_engine = eng;
}

bool SceneManager::LoadFromFile(std::string filepath)
{
	AAssetManager* mgr = m_engine->app->activity->assetManager;
	AAsset* file = AAssetManager_open(mgr, filepath.c_str(), AASSET_MODE_BUFFER);

	if (NULL == file)
		return false;

	long fsize = AAsset_getLength(file);
	char *string = new char[fsize + 1];

	AAsset_read(file, string, fsize);
	AAsset_close(file);

	string[fsize] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(string);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	//background
	ParseBackgroundColor(pRoot);


	//cameras
	if (!ParseCameras(pRoot))
	{
		return false;
	}

	// fog
	ParseFog(pRoot);

	// lights

	if (!ParseLights(pRoot))
	{
		return false;
	}

	//objects

	if (!ParseObjects(pRoot))
	{
		return false;
	}

	//ambiental light
	//lights
	//debug settings

	delete[] string;

	// Init objects
	for (auto model : m_objects)
	{
		model.second->Init();
	}	

	glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 0.f);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void SceneManager::Update()
{
	for (auto & obj : m_objects) {
		obj.second->Update();
	}

	// Collision detection
	for (auto obj = m_objects.begin(); obj != m_objects.end(); ++obj) {
		for (auto obj2 = std::next(obj); obj2 != m_objects.end(); ++obj2) {
			if ("SkyBox" == obj2->second->GetName()) {
				continue;
			}

			if (true == obj->second->Collides(obj2->second)) {
				std::cout << obj->second->GetName() << " collides with " << obj2->second->GetName() << std::endl;
			}
		}

		if (true == obj->second->Contains(m_cameras[m_active_camera]->GetPosition())) {
			std::cout << "Camera collides with " << obj->second->GetName() << std::endl;
		}
	}
}

void SceneManager::Draw()
{
	for (auto & obj : m_objects) {
		obj.second->Draw();
	}
}

void SceneManager::CleanUp()
{
	if (nullptr != m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

bool SceneManager::ParseBackgroundColor(rapidxml::xml_node<> *pRoot)
{
	rapidxml::xml_node<> *pBkg = pRoot->first_node("backgroundColor");

	m_background_color = XMLUtils::GetVectorValueRGBSafe(pBkg, Vector3(0.f));


	PrintUtils::PrintD("Background color " + m_background_color.ToString());

	return true;
}

bool SceneManager::ParseCameras(rapidxml::xml_node<> *pRoot)
{
	rapidxml::xml_node<> *pCameras = pRoot->first_node("cameras");
	if (nullptr == pCameras)
	{
		std::cerr << "Cameras are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pCamera = pCameras->first_node("camera"); pCamera; pCamera = pCamera->next_sibling())
	{
		if (!ParseCamera(pCamera))
		{
			return false;
		}
	}

	m_active_camera = XMLUtils::GetStringValueSafe(pRoot, "activeCamera", "1");

	return true;
}

bool SceneManager::ParseCamera(rapidxml::xml_node<> *pCamera)
{
	std::string id = XMLUtils::GetStringValueSafe(pCamera, "id", "");

	if (id == "")
	{
		LOGE("Camera ID Missing...\n");
		return false;
	}

	Vector3 pos = XMLUtils::GetVectorValueXYZSafe(pCamera, "position", Vector3(0.f));
	Vector3 target = XMLUtils::GetVectorValueXYZSafe(pCamera, "target", Vector3(0.f, 0.f, -1.f));
	Vector3 up = XMLUtils::GetVectorValueXYZSafe(pCamera, "up", Vector3(0.f, 1.f, 0.f));

	float translateSpeed = XMLUtils::GetIntValueSafe(pCamera, "translationSpeed", 10);
	float rotationSpeed = XMLUtils::GetFloatValueSafe(pCamera, "rotationSpeed", 0.3f);
	float fov = XMLUtils::GetIntValueSafe(pCamera, "fov", 45);
	float cnear = XMLUtils::GetFloatValueSafe(pCamera, "near", 0.2f);
	float cfar = XMLUtils::GetFloatValueSafe(pCamera, "far", 10000.f);

	m_cameras[id] = new Camera(pos, target, up, translateSpeed, rotationSpeed, cnear, cfar, fov);

	return true;
}

bool SceneManager::ParseFog(rapidxml::xml_node<> *pRoot)
{
	rapidxml::xml_node<> *pFog = pRoot->first_node("fog");
	if (nullptr == pFog)
	{
		return true;
	}

	Vector3 fogColor = XMLUtils::GetVectorValueRGBSafe(pFog, Vector3(1.f));
	float r = XMLUtils::GetFloatValueSafe(pFog, "start", 100.f);
	float R = XMLUtils::GetFloatValueSafe(pFog, "end", 300.f);

	m_fog.SetValues(r, R, fogColor);

	return true;
}

bool SceneManager::ParseLights(rapidxml::xml_node<> *pRoot)
{
	rapidxml::xml_node<> *pAmbientalLight = pRoot->first_node("ambientalLight");

	float amb_light_ratio = XMLUtils::GetFloatValueSafe(pAmbientalLight, "ratio", 0.1f);
	Vector3 amb_light = XMLUtils::GetVectorValueRGBSafe(pAmbientalLight, "color", Vector3(0.f));

	m_ambiental_light.SetValue(amb_light_ratio, amb_light);

	rapidxml::xml_node<> *pLights = pRoot->first_node("lights");

	for (rapidxml::xml_node<> *pLight = pLights->first_node("light"); pLight; pLight = pLight->next_sibling())
	{
		if (!ParseLight(pLight))
		{
			return false;
		}		
	}

	return true;
}

bool SceneManager::ParseLight(rapidxml::xml_node<> *pLight)
{
	std::string light_id = XMLUtils::GetStringValueSafe(pLight, "id", "");

	if (light_id == "")
	{
		std::cerr << "Light id missing" << std::endl;
		return false;
	}

	if (m_lights.find(light_id) != m_lights.end())
	{
		std::cerr << "Light id already used. Skipping." << std::endl;
		return false;
	}

	std::string asoc_obj = XMLUtils::GetStringValueSafe(pLight, "associatedObject", "");
	std::string lightType = XMLUtils::GetStringValueSafe(pLight, "type", "");

	Vector3 diffuseColor = XMLUtils::GetVectorValueRGBSafe(pLight, "diffuseColor", Vector3(0.f));
	Vector3 specularColor = XMLUtils::GetVectorValueRGBSafe(pLight, "specularColor", Vector3(0.f));
	Vector3 direction = XMLUtils::GetVectorValueXYZSafe(pLight, "direction", Vector3(0.f));
	Vector3 position = XMLUtils::GetVectorValueXYZSafe(pLight, "position", Vector3(0.f));

	float specCoef = XMLUtils::GetFloatValueSafe(pLight, "specCoef", 0.f);
	float diffCoef = XMLUtils::GetFloatValueSafe(pLight, "diffCoef", 0.f);
	float shininess = XMLUtils::GetFloatValueSafe(pLight, "shininess", 0.f);
	float spotAngle = XMLUtils::GetFloatValueSafe(pLight, "spotAngle", 0.f);

	LightSource *ls = new LightSource(shininess, diffCoef, specCoef, diffuseColor, specularColor, asoc_obj);
	ls->SetType(lightType);
	ls->SetDirection(direction);
	ls->SetPosition(position);
	ls->SetSpotAngle(spotAngle);

	m_lights[light_id] = ls;

	return true;
}

bool SceneManager::ParseObjects(rapidxml::xml_node<> *pRoot)
{
	rapidxml::xml_node<> *pObjects = pRoot->first_node("objects");
	if (nullptr == pObjects)
	{
		std::cerr << "Objects are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pObject = pObjects->first_node("object"); pObject; pObject = pObject->next_sibling())
	{
		if (!ParseObject(pObject))
		{
			return false;
		}
	}

	return true;
}

bool SceneManager::ParseObject(rapidxml::xml_node<> *pObject)
{
	std::string id = XMLUtils::GetStringValue(pObject, "id");

	rapidxml::xml_node<> *pShader = pObject->first_node("shader");
	if (nullptr == pShader)
	{
		std::cerr << "Shader id missing" << std::endl;
		return false;
	}

	std::string shaderID = pShader->value();

	rapidxml::xml_node<> *pType = pObject->first_node("type");
	if (nullptr == pType)
	{
		std::cerr << "Object type missing" << std::endl;
		return false;
	}

	std::unordered_map<std::string, ObjectType> typeMap({ {"normal", OT_NORMAL}, {"skybox", OT_SKYBOX}, {"terrain", OT_TERRAIN}, {"animated", OT_ANIMATED} });
	std::string type = pType->value();
	ObjectType ot = typeMap[type];


	//LOGD("ot value: %d\n", ot);
	bool depthTest = XMLUtils::GetBoolValueSafe(pObject, "depthTest", true);
	bool blend = XMLUtils::GetBoolValueSafe(pObject, "blend", true);
	std::string name = XMLUtils::GetStringValueSafe(pObject, "name", "");

	rapidxml::xml_node<> *pTextures = pObject->first_node("textures");
	std::vector<std::string> texture_ids;
	if (nullptr != pTextures)
	{
		for (rapidxml::xml_node<> *pTexture = pTextures->first_node("texture"); pTexture; pTexture = pTexture->next_sibling()) {
			texture_ids.push_back(XMLUtils::GetStringValue(pTexture, "id"));
		}
	}

	rapidxml::xml_node<> *pObjLights = pObject->first_node("lights");
	std::vector<std::string> light_ids;
	if (nullptr != pObjLights)
	{
		for (rapidxml::xml_node<> *pObjLight = pObjLights->first_node("light"); pObjLight; pObjLight = pObjLight->next_sibling())
		{
			light_ids.push_back(pObjLight->value());
		}
	}

	Vector3 pos = XMLUtils::GetVectorValueXYZSafe(pObject, "position", Vector3(0.f));
	Vector3 rot = XMLUtils::GetVectorValueXYZSafe(pObject, "rotation", Vector3(0.f));
	Vector3 scale = XMLUtils::GetVectorValueXYZSafe(pObject, "scale", Vector3(0.f));
	Vector3 color = XMLUtils::GetVectorValueXYZSafe(pObject, "color", Vector3(0.f));

	float selfRotateSpeed = XMLUtils::GetFloatValueSafe(pObject, "selfRotateSpeed", 0.1f);

	bool wired = XMLUtils::GetBoolValueSafe(pObject, "wired", false);

	SceneObject *object = nullptr;

	switch (ot)
	{
	case OT_ANIMATED:
	{
		float displacement = XMLUtils::GetFloatValueSafe(pObject, "displacement_max", 0.5f);
		AnimatedObject *ao = new AnimatedObject(pos, rot, scale, depthTest, name, displacement);
		object = ao;
		break;
	}
	case OT_TERRAIN:
	{
		uint32_t blockSize = XMLUtils::GetIntValueSafe(pObject, "blockSize", 4);
		uint32_t cellSize = XMLUtils::GetIntValueSafe(pObject, "cellSize", 1);
		float offsetY = XMLUtils::GetFloatValueSafe(pObject, "offsetY", 0.f);
		Vector3 heights = XMLUtils::GetVectorValueRGBSafe(pObject, "heights", Vector3(0.f));

		Terrain *t = new Terrain(pos, rot, scale, heights, depthTest, name);
		t->SetBlockSize(blockSize);
		t->SetCellSize(cellSize);
		t->SetOffsetY(offsetY);

		object = t;
		break;
	}
	case OT_SKYBOX:
	{
		float offsetY = XMLUtils::GetFloatValueSafe(pObject, "offsetY", 0.f);
		float sz = XMLUtils::GetFloatValueSafe(pObject, "size", 0.f);

		SkyBox *sb = new SkyBox(pos, rot, scale, name, offsetY, sz);

		object = sb;
		break;
	}
	case OT_NORMAL:
	{
		object = new SceneObject(pos, rot, scale, depthTest, name);
		break;
	}
	default:
		break;
	}

	if (OT_TERRAIN != ot) {
		rapidxml::xml_node<> *pModel = pObject->first_node("model");
		if (nullptr == pModel)
		{
			std::cerr << "Model id missing" << std::endl;
			return false;
		}

		std::string modelID = pModel->value();
		object->SetModel(ResourceManager::GetInstance()->LoadModel(modelID));
	}

	object->SetBlend(blend);
	object->SetShader(ResourceManager::GetInstance()->LoadShader(shaderID));
	for (std::string texID : texture_ids)
	{
		object->AddTexture(ResourceManager::GetInstance()->LoadTexture(texID));
	}

	for (std::string lightID : light_ids)
	{
		object->AddLightID(lightID);
	}

	// Trajectory
	rapidxml::xml_node<> *pTrajectory = pObject->first_node("trajectory");
	if (nullptr != pTrajectory)
	{
		uint32_t iter_count = XMLUtils::GetIntValueSafe(pTrajectory, "iteration-count", 1);
		float traj_speed = XMLUtils::GetFloatValueSafe(pTrajectory, "speed", 1.f);
		std::string type = XMLUtils::GetStringValueSafe(pTrajectory, "type", "");
		std::string direction = XMLUtils::GetStringValueSafe(pTrajectory, "direction", "");


		Trajectory *tj = new Trajectory(iter_count, traj_speed, Trajectory::GetDirectionTypeFromString(direction), Trajectory::GetTrajectoryTypeFromString(type));

		tj->AddPoint(pos);

		// add points
		rapidxml::xml_node<> *pTrajectoryPoints = pTrajectory->first_node("points");

		if (nullptr != pTrajectoryPoints)
		{
			for (rapidxml::xml_node<> *pTjPoint = pTrajectoryPoints->first_node("point"); pTjPoint; pTjPoint = pTjPoint->next_sibling())
			{
				tj->AddPoint(XMLUtils::GetVectorValueXYZSafe(pTjPoint, Vector3(0.f)));
			}
		}
		object->SetTrajectory(tj);
	}

	object->SetWired(wired);
	m_objects[id] = object;

	return true;
}
