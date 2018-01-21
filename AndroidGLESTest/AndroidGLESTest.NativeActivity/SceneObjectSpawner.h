#pragma once
#include <map>

#include "Structs.h"
#include "VisualBody.h"

#include "MapCell.h"
#include "Robot.h"


namespace SceneObjectSpawner
{

	static const std::vector<std::string> GROUND_TEXTURES = { "1" };
	static const std::vector<std::string> WALL_TEXTURES = { "2" };

	static const std::string PLAYER_ROBOT_TEXTURE = "3";

	enum MapObjectType { GROUND = 0, WALL = 1 };

	Vector2 ToMapCoords(const Vector3 worldCoords);
	Vector3 ToWorldCoords(const Vector2 mapCoords, MapObjectType type);

	MapCell * SpawnMapCell(const Vector2 mapCoords, const MapObjectType cellType);

	PhysicsBody *SpawnRobot(const Vector2 mapCoords);

	static Texture *ChooseRandomTexture(const MapObjectType type);
	static Model *GetMapCellModel();
	static Shader *GetMapCellShader();

	static Model *GetRobotModel();
	static Shader *GetRobotShader();

	// 	SceneObjectSpawner(const std::string & model_id, const std::string & shader_id);
	// 	virtual ~SceneObjectSpawner();
	// 
	// 	VisualBody *SpawnObject(const Vector3 pos, const std::vector<std::string> & texture_ids, GameLoopObject *parent);

	// 	Model *m_model;
	// 	Shader *m_shader;
};

