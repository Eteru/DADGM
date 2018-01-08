
#include "SceneManager.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "AnimatedObject.h"

#include <android/asset_manager.h>

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

bool SceneManager::Init(std::string filepath)
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
	rapidxml::xml_node<> *pBkg = pRoot->first_node("backgroundColor");

	if (nullptr == pBkg) {
		m_background_color = Vector3(0.f, 0.f, 0.f);
	}
	else {
		m_background_color.x = strtof(pBkg->first_node("r")->value(), 0);
		m_background_color.y = strtof(pBkg->first_node("g")->value(), 0);
		m_background_color.z = strtof(pBkg->first_node("b")->value(), 0);
	}

	//cameras
	rapidxml::xml_node<> *pCameras = pRoot->first_node("cameras");
	if (nullptr == pCameras) {
		std::cerr << "Cameras are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pCamera = pCameras->first_node("camera"); pCamera; pCamera = pCamera->next_sibling()) {
		rapidxml::xml_attribute<> *pAttribute = pCamera->first_attribute("id");

		if (nullptr == pAttribute) {
			std::cerr << "Camera id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		float translateSpeed, rotationSpeed, fov, cnear, cfar;
		Vector3 pos, target, up;

		rapidxml::xml_node<> *pPosition = pCamera->first_node("position");

		if (nullptr == pPosition) {
			pos = Vector3(0.f, 0.f, 0.f);
		}
		else {
			pos.x = strtof(pPosition->first_node("x")->value(), 0);
			pos.y = strtof(pPosition->first_node("y")->value(), 0);
			pos.z = strtof(pPosition->first_node("z")->value(), 0);
		}

		rapidxml::xml_node<> *pTarget = pCamera->first_node("target");

		if (nullptr == pTarget) {
			target = Vector3(0.f, 0.f, -1.f);
		}
		else {
			target.x = strtof(pTarget->first_node("x")->value(), 0);
			target.y = strtof(pTarget->first_node("y")->value(), 0);
			target.z = strtof(pTarget->first_node("z")->value(), 0);
		}

		rapidxml::xml_node<> *pUp = pCamera->first_node("up");

		if (nullptr == pUp) {
			up = Vector3(0.f, 1.f, 0.f);
		}
		else {
			up.x = strtof(pUp->first_node("x")->value(), 0);
			up.y = strtof(pUp->first_node("y")->value(), 0);
			up.z = strtof(pUp->first_node("z")->value(), 0);
		}

		rapidxml::xml_node<> *pTranslationSpeed = pCamera->first_node("translationSpeed");

		if (nullptr == pTranslationSpeed) {
			translateSpeed = 10;
		}
		else {
			translateSpeed = atoi(pTranslationSpeed->value());
		}

		rapidxml::xml_node<> *pRotationSpeed = pCamera->first_node("rotationSpeed");

		if (nullptr == pRotationSpeed) {
			rotationSpeed = 0.3f;
		}
		else {
			rotationSpeed = strtof(pRotationSpeed->value(), 0);
		}

		rapidxml::xml_node<> *pFov = pCamera->first_node("fov");

		if (nullptr == pFov) {
			fov = 45;
		}
		else {
			fov = atoi(pFov->value());
		}

		rapidxml::xml_node<> *pNear = pCamera->first_node("near");

		if (nullptr == pNear) {
			cnear = 0.2f;
		}
		else {
			cnear = strtof(pNear->value(), 0);
		}

		rapidxml::xml_node<> *pFar = pCamera->first_node("far");

		if (nullptr == pFar) {
			cfar = 10000.f;
		}
		else {
			cfar = strtof(pFar->value(), 0);
		}

		m_cameras[id] = new Camera(pos, target, up, translateSpeed, rotationSpeed, cnear, cfar, fov);
	}

	rapidxml::xml_node<> *pActiveCamera = pRoot->first_node("activeCamera");
	if (nullptr == pActiveCamera) {
		m_active_camera = "1";
	}
	else {
		m_active_camera = pActiveCamera->value();
	}

	// fog
	rapidxml::xml_node<> *pFog = pRoot->first_node("fog");
	if (nullptr != pFog) {
		rapidxml::xml_node<> *pFogColor = pFog->first_node("color");

		float fr, fg, fb;
		if (nullptr != pFogColor) {
			rapidxml::xml_node<> *pFogColorR = pFogColor->first_node("r");
			rapidxml::xml_node<> *pFogColorG = pFogColor->first_node("g");
			rapidxml::xml_node<> *pFogColorB = pFogColor->first_node("b");

			fr = (nullptr == pFogColorR) ? 1.0 : strtof(pFogColorR->value(), 0);
			fg = (nullptr == pFogColorG) ? 1.0 : strtof(pFogColorG->value(), 0);
			fb = (nullptr == pFogColorB) ? 1.0 : strtof(pFogColorB->value(), 0);
		}

		rapidxml::xml_node<> *pFogr = pFog->first_node("start");
		rapidxml::xml_node<> *pFogR = pFog->first_node("end");

		float r = (nullptr == pFogr) ? 100.f : strtof(pFogr->value(), 0);
		float R = (nullptr == pFogR) ? 300.f : strtof(pFogR->value(), 0);

		m_fog.SetValues(r, R, Vector3(fr, fg, fb));
	}

	// lights
	rapidxml::xml_node<> *pAmbientalLight = pRoot->first_node("ambientalLight");

	float amb_light_ratio = 0.1f;
	Vector3 amb_light = Vector3(0, 0, 0);
	if (nullptr != pAmbientalLight) {
		rapidxml::xml_node<> *pAmbientalLightColor = pAmbientalLight->first_node("color");
		if (nullptr != pAmbientalLightColor) {
			rapidxml::xml_node<> *pAmbientalLightColorR = pAmbientalLightColor->first_node("r");
			rapidxml::xml_node<> *pAmbientalLightColorG = pAmbientalLightColor->first_node("g");
			rapidxml::xml_node<> *pAmbientalLightColorB = pAmbientalLightColor->first_node("b");

			amb_light.x = (nullptr == pAmbientalLightColorR) ? 0.f : strtof(pAmbientalLightColorR->value(), 0);
			amb_light.y = (nullptr == pAmbientalLightColorG) ? 0.f : strtof(pAmbientalLightColorG->value(), 0);
			amb_light.z = (nullptr == pAmbientalLightColorB) ? 0.f : strtof(pAmbientalLightColorB->value(), 0);
		}

		rapidxml::xml_node<> *pAmbientalLightRatio = pAmbientalLight->first_node("ratio");
		amb_light_ratio = (nullptr == pAmbientalLightRatio) ? 0.1f : strtof(pAmbientalLightRatio->value(), 0);
	}

	m_ambiental_light.SetValue(amb_light_ratio, amb_light);

	rapidxml::xml_node<> *pLights = pRoot->first_node("lights");
	for (rapidxml::xml_node<> *pLight = pLights->first_node("light"); pLight; pLight = pLight->next_sibling()) {
		rapidxml::xml_attribute<> *pLightAttribute = pLight->first_attribute("id");

		if (nullptr == pLightAttribute) {
			std::cerr << "Light id missing" << std::endl;
			continue;
		}
		std::string light_id = pLightAttribute->value();
		if (m_lights.find(light_id) != m_lights.end()) {
			std::cerr << "Light id already used. Skipping." << std::endl;
			continue;
		}

		rapidxml::xml_node<> *pAssociatedObject = pLight->first_node("associatedObject");
		rapidxml::xml_node<> *pDiffuseColor = pLight->first_node("diffuseColor");
		rapidxml::xml_node<> *pSpecularColor = pLight->first_node("specularColor");
		rapidxml::xml_node<> *pLightType = pLight->first_node("type");
		rapidxml::xml_node<> *pSpecCoef = pLight->first_node("specCoef");
		rapidxml::xml_node<> *pDiffCoef = pLight->first_node("diffCoef");
		rapidxml::xml_node<> *pShininess = pLight->first_node("shininess");
		rapidxml::xml_node<> *pSpotAngle = pLight->first_node("spotAngle");
		rapidxml::xml_node<> *pLightDir = pLight->first_node("direction");
		rapidxml::xml_node<> *pLightPos = pLight->first_node("position");
		///TODO: finish dir

		std::string asoc_obj = (nullptr == pAssociatedObject) ? "" : pAssociatedObject->value();

		Vector3 light_diff_color = Vector3(0, 0, 0);
		if (nullptr != pDiffuseColor) {
			rapidxml::xml_node<> *pDiffuseColorR = pDiffuseColor->first_node("r");
			rapidxml::xml_node<> *pDiffuseColorG = pDiffuseColor->first_node("g");
			rapidxml::xml_node<> *pDiffuseColorB = pDiffuseColor->first_node("b");

			light_diff_color.x = (nullptr == pDiffuseColorR) ? 0.f : strtof(pDiffuseColorR->value(), 0);
			light_diff_color.y = (nullptr == pDiffuseColorG) ? 0.f : strtof(pDiffuseColorG->value(), 0);
			light_diff_color.z = (nullptr == pDiffuseColorB) ? 0.f : strtof(pDiffuseColorB->value(), 0);
		}

		Vector3 light_spec_color = Vector3(0, 0, 0);
		if (nullptr != pSpecularColor) {
			rapidxml::xml_node<> *pSpecularColorR = pSpecularColor->first_node("r");
			rapidxml::xml_node<> *pSpecularColorG = pSpecularColor->first_node("g");
			rapidxml::xml_node<> *pSpecularColorB = pSpecularColor->first_node("b");

			light_spec_color.x = (nullptr == pSpecularColorR) ? 0.f : strtof(pSpecularColorR->value(), 0);
			light_spec_color.y = (nullptr == pSpecularColorG) ? 0.f : strtof(pSpecularColorG->value(), 0);
			light_spec_color.z = (nullptr == pSpecularColorB) ? 0.f : strtof(pSpecularColorB->value(), 0);
		}

		Vector3 light_dir = Vector3(0, 0, 0);
		if (nullptr != pLightDir) {
			rapidxml::xml_node<> *pLightDirX = pLightDir->first_node("x");
			rapidxml::xml_node<> *pLightDirY = pLightDir->first_node("y");
			rapidxml::xml_node<> *pLightDirZ = pLightDir->first_node("z");

			light_dir.x = (nullptr == pLightDirX) ? 0.f : strtof(pLightDirX->value(), 0);
			light_dir.y = (nullptr == pLightDirY) ? 0.f : strtof(pLightDirY->value(), 0);
			light_dir.z = (nullptr == pLightDirZ) ? 0.f : strtof(pLightDirZ->value(), 0);
		}

		Vector3 light_pos = Vector3(0, 0, 0);
		if (nullptr != pLightPos) {
			rapidxml::xml_node<> *pLightPosX = pLightPos->first_node("x");
			rapidxml::xml_node<> *pLightPosY = pLightPos->first_node("y");
			rapidxml::xml_node<> *pLightPosZ = pLightPos->first_node("z");

			light_pos.x = (nullptr == pLightPosX) ? 0.f : strtof(pLightPosX->value(), 0);
			light_pos.y = (nullptr == pLightPosY) ? 0.f : strtof(pLightPosY->value(), 0);
			light_pos.z = (nullptr == pLightPosZ) ? 0.f : strtof(pLightPosZ->value(), 0);
		}

		std::string light_type = (nullptr == pLightType) ? "" : pLightType->value();
		float spec_coef = (nullptr == pSpecCoef) ? 0.f : strtof(pSpecCoef->value(), 0);
		float diff_coef = (nullptr == pDiffCoef) ? 0.f : strtof(pDiffCoef->value(), 0);
		float shininess = (nullptr == pShininess) ? 0.f : strtof(pShininess->value(), 0);
		float spot_angle = (nullptr == pSpotAngle) ? 0.f : strtof(pSpotAngle->value(), 0);

		LightSource *ls = new LightSource(shininess, diff_coef, spec_coef, light_diff_color, light_spec_color, asoc_obj);
		ls->SetType(light_type);
		ls->SetDirection(light_dir);
		ls->SetPosition(light_pos);
		ls->SetSpotAngle(spot_angle);

		m_lights[light_id] = ls;
	}

	//objects
	rapidxml::xml_node<> *pObjects = pRoot->first_node("objects");
	if (nullptr == pObjects) {
		std::cerr << "Objects are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pObject = pObjects->first_node("object"); pObject; pObject = pObject->next_sibling()) {
		rapidxml::xml_attribute<> *pAttribute = pObject->first_attribute("id");

		if (nullptr == pAttribute) {
			std::cerr << "Object id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		rapidxml::xml_node<> *pShader = pObject->first_node("shader");
		if (nullptr == pShader) {
			std::cerr << "Shader id missing" << std::endl;
			return false;
		}

		std::string shaderID = pShader->value();

		rapidxml::xml_node<> *pType = pObject->first_node("type");
		if (nullptr == pType) {
			std::cerr << "Object type missing" << std::endl;
			return false;
		}

		std::string type = pType->value();
		ObjectType ot = OT_NORMAL;

		if ("normal" == type) {
			ot = OT_NORMAL;
		}
		else if ("skybox" == type) {
			ot = OT_SKYBOX;
		}
		else if ("terrain" == type) {
			ot = OT_TERRAIN;
		}
		else if ("animated" == type) {
			ot = OT_ANIMATED;
		}

		bool depthTest = true;
		rapidxml::xml_node<> *pDepthTest = pObject->first_node("depthTest");
		if (nullptr != pDepthTest) {
			depthTest = pDepthTest->value() == "true" ? true : false;
		}

		bool blend = true;
		rapidxml::xml_node<> *pBlend = pObject->first_node("blend");
		if (nullptr != pBlend) {
			blend = pBlend->value() == "true" ? true : false;
		}

		std::string name = "";
		rapidxml::xml_node<> *pName = pObject->first_node("name");
		if (nullptr != pName) {
			name = pName->value();
		}

		rapidxml::xml_node<> *pTextures = pObject->first_node("textures");
		std::vector<std::string> texture_ids;
		if (nullptr != pTextures) {
			for (rapidxml::xml_node<> *pTexture = pTextures->first_node("texture"); pTexture; pTexture = pTexture->next_sibling()) {
				rapidxml::xml_attribute<> *pAttribute = pTexture->first_attribute("id");

				if (nullptr == pAttribute) {
					std::cerr << "Texture id missing" << std::endl;
					return false;
				}

				texture_ids.push_back(pAttribute->value());
			}
		}

		rapidxml::xml_node<> *pObjLights = pObject->first_node("lights");
		std::vector<std::string> light_ids;
		if (nullptr != pObjLights) {
			for (rapidxml::xml_node<> *pObjLight = pObjLights->first_node("light"); pObjLight; pObjLight = pObjLight->next_sibling()) {
				light_ids.push_back(pObjLight->value());
			}
		}

		Vector3 pos;
		rapidxml::xml_node<> *pPosition = pObject->first_node("position");

		if (nullptr == pPosition) {
			pos = Vector3(0.f, 0.f, 0.f);
		}
		else {
			pos.x = strtof(pPosition->first_node("x")->value(), 0);
			pos.y = strtof(pPosition->first_node("y")->value(), 0);
			pos.z = strtof(pPosition->first_node("z")->value(), 0);
		}

		Vector3 rot;
		rapidxml::xml_node<> *pRotation = pObject->first_node("rotation");

		if (nullptr == pRotation) {
			pos = Vector3(0.f, 0.f, 0.f);
		}
		else {
			rot.x = strtof(pRotation->first_node("x")->value(), 0);
			rot.y = strtof(pRotation->first_node("y")->value(), 0);
			rot.z = strtof(pRotation->first_node("z")->value(), 0);
		}

		Vector3 scale;
		rapidxml::xml_node<> *pScale = pObject->first_node("scale");

		if (nullptr == pScale) {
			pos = Vector3(0.f, 0.f, 0.f);
		}
		else {
			scale.x = strtof(pScale->first_node("x")->value(), 0);
			scale.y = strtof(pScale->first_node("y")->value(), 0);
			scale.z = strtof(pScale->first_node("z")->value(), 0);
		}

		Vector3 color;
		rapidxml::xml_node<> *pColor = pObject->first_node("color");

		if (nullptr == pColor) {
			color = Vector3(0.f, 0.f, 0.f);
		}
		else {
			color.x = strtof(pColor->first_node("r")->value(), 0);
			color.y = strtof(pColor->first_node("g")->value(), 0);
			color.z = strtof(pColor->first_node("b")->value(), 0);
		}

		float selfRotateSpeed;
		rapidxml::xml_node<> *pSelfRotateSpeed = pObject->first_node("selfRotateSpeed");

		if (nullptr == pSelfRotateSpeed) {
			selfRotateSpeed = 0.1f;
		}
		else {
			selfRotateSpeed = strtof(pSelfRotateSpeed->value(), 0);
		}

		rapidxml::xml_node<> *pWired = pObject->first_node("wired");
		bool wired = pWired ? true : false;

		SceneObject *object = nullptr;

		switch (ot)
		{
		case OT_ANIMATED:
		{
			float displacement = 0.5f;
			rapidxml::xml_node<> *pDisplacementMax = pObject->first_node("displacement_max");
			if (nullptr != pDisplacementMax) {
				displacement = strtof(pDisplacementMax->value(), 0);
			}

			AnimatedObject *ao = new AnimatedObject(pos, rot, scale, depthTest, name, displacement);

			object = ao;

			break;
		}
		case OT_TERRAIN:
		{
			uint32_t blockSize = 4; // default
			rapidxml::xml_node<> *pBlockSize = pObject->first_node("blockSize");
			if (nullptr != pBlockSize) {
				blockSize = atoi(pBlockSize->value());
			}

			uint32_t cellSize = 1; // default
			rapidxml::xml_node<> *pCellSize = pObject->first_node("cellSize");
			if (nullptr != pCellSize) {
				cellSize = atoi(pCellSize->value());
			}

			float offsetY = 0.f; // default
			rapidxml::xml_node<> *pOffsetY = pObject->first_node("offsetY");
			if (nullptr != pOffsetY) {
				offsetY = strtof(pOffsetY->value(), 0);
			}

			Vector3 heights;
			rapidxml::xml_node<> *pHeights = pObject->first_node("heights");

			if (nullptr == pHeights) {
				heights = Vector3(0.f, 0.f, 0.f);
			}
			else {
				heights.x = strtof(pHeights->first_node("r")->value(), 0);
				heights.y = strtof(pHeights->first_node("g")->value(), 0);
				heights.z = strtof(pHeights->first_node("b")->value(), 0);
			}

			Terrain *t = new Terrain(pos, rot, scale, heights, depthTest, name);
			t->SetBlockSize(blockSize);
			t->SetCellSize(cellSize);
			t->SetOffsetY(offsetY);

			object = t;
			break;
		}
		case OT_SKYBOX:
		{
			float offsetY = 0.f; // default
			rapidxml::xml_node<> *pOffsetY = pObject->first_node("offsetY");
			if (nullptr != pOffsetY) {
				offsetY = strtof(pOffsetY->value(), 0);
			}

			float sz = 0.f; // default
			rapidxml::xml_node<> *pSz = pObject->first_node("size");
			if (nullptr != pSz) {
				sz = strtof(pSz->value(), 0);
			}

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
			if (nullptr == pModel) {
				std::cerr << "Model id missing" << std::endl;
				return false;
			}

			std::string modelID = pModel->value();
			object->SetModel(ResourceManager::GetInstance()->LoadModel(modelID));
		}

		object->SetBlend(blend);
		object->SetShader(ResourceManager::GetInstance()->LoadShader(shaderID));
		for (std::string texID : texture_ids) {
			object->AddTexture(ResourceManager::GetInstance()->LoadTexture(texID));
		}

		for (std::string lightID : light_ids) {
			object->AddLightID(lightID);
		}

		// Trajectory
		rapidxml::xml_node<> *pTrajectory = pObject->first_node("trajectory");
		if (nullptr != pTrajectory) {
			// default values
			uint32_t iter_count = 1;
			float traj_speed = 1.f;
			std::string type = "";
			std::string direction = "";

			// get actual values
			rapidxml::xml_attribute<> *pTjType = pTrajectory->first_attribute("type");
			rapidxml::xml_attribute<> *pTjIterCount = pTrajectory->first_attribute("iteration-count");
			rapidxml::xml_attribute<> *pTjDirection = pTrajectory->first_attribute("direction");
			rapidxml::xml_attribute<> *pTjSpeed = pTrajectory->first_attribute("speed");

			if (nullptr != pTjType) {
				type = pTjType->value();
			}
			if (nullptr != pTjIterCount) {
				std::string iter_value = pTjIterCount->value();
				if ("infinite" == iter_value) {
					iter_count = -1;
				}
				else {
					iter_count = atoi(iter_value.c_str());
				}
			}
			if (nullptr != pTjDirection) {
				direction = pTjDirection->value();
			}
			if (nullptr != pTjSpeed) {
				traj_speed = strtof(pTjSpeed->value(), 0);
			}

			Trajectory *tj = new Trajectory(iter_count, traj_speed, Trajectory::GetDirectionTypeFromString(direction),
				Trajectory::GetTrajectoryTypeFromString(type));
			tj->AddPoint(pos);

			// add points
			rapidxml::xml_node<> *pTrajectoryPoints = pTrajectory->first_node("points");

			if (nullptr != pTrajectoryPoints) {
				for (rapidxml::xml_node<> *pTjPoint = pTrajectoryPoints->first_node("point"); pTjPoint; pTjPoint = pTjPoint->next_sibling()) {
					Vector3 point;
					point.x = strtof(pTjPoint->first_node("x")->value(), 0);
					point.y = strtof(pTjPoint->first_node("y")->value(), 0);
					point.z = strtof(pTjPoint->first_node("z")->value(), 0);

					tj->AddPoint(point);
				}
			}

			object->SetTrajectory(tj);
		}

		object->SetWired(wired);
		m_objects[id] = object;
	}
	//ambiental light
	//lights
	//debug settings

	delete string;

	// Init objects
	for (auto model : m_objects) {
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
