
#include "SceneObjectSpawner.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "UniqueID.h"
#include "PhysicsBody.h"
#include "BoundingSphere.h"

// SceneObjectSpawner::SceneObjectSpawner(const std::string & model_id, const std::string & shader_id)
// {
// 	ResourceManager *rm = ResourceManager::GetInstance();
// 
// 	m_model = rm->LoadModel(model_id);
// 	m_shader = rm->LoadShader(shader_id);
// }

// SceneObjectSpawner::~SceneObjectSpawner()
// {
// 	// resources are eliminated by the managers
// }

// VisualBody * SceneObjectSpawner::SpawnObject(const Vector3 pos, const std::vector<std::string> & texture_ids, GameLoopObject *parent)
// {
// 	if (nullptr == m_model ||
// 		nullptr == m_shader)
// 	{
// 		return nullptr;
// 	}
// 
// 	VisualBody *so = new VisualBody(pos, Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 0.5f), "SpawnedObject", true);
// 
// 	so->SetModel(m_model);
// 	so->SetShader(m_shader);
// 
// 	ResourceManager *rm = ResourceManager::GetInstance();
// 	for (const std::string & id : texture_ids)
// 	{
// 		Texture *tex = rm->LoadTexture(id);
// 
// 		if (nullptr != tex)
// 		{
// 			so->AddTexture(tex);
// 		}
// 	}
// 
// 	so->SetID(UniqueID::GetID(so->GetClassName()));
// 
// 	parent->AddComponent(so);
// 
// 	return so;
// }

MapCell * SceneObjectSpawner::SpawnMapCell(const Vector2 mapCoords, const MapObjectType cellType)
{
	Model *cubeModel = GetMapCellModel();
	Shader *cubeShader = GetMapCellShader();


	MapCell *cell = new MapCell();
	cell->SetID(UniqueID::GetID(cell->GetClassName()));
	cell->m_transform.SetPos(GameConstants::ToWorldCoords(mapCoords, cellType == GROUND ? GameConstants::GROUND_HEIGHT : GameConstants::WALL_HEIGHT));	
	cell->Init();

	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.5f), "MapCell", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(cubeModel);
	vb->SetShader(cubeShader);

	Texture *tex = ChooseRandomTexture(cellType);

	if (nullptr != tex)
	{
		vb->AddTexture(tex);
	}

	vb->Init();
	cell->AddComponent(vb);

	if (cellType == MapObjectType::WALL)
	{
		PhysicsBody *pb = new PhysicsBody();
		pb->SetID(UniqueID::GetID(pb->GetClassName()));
		pb->m_mass = 10000000.f;
		pb->m_kinematic = true;
		pb->m_transform.ComputeWorld();
		pb->m_inertia = Math::BoxInertiaY(pb->m_mass, 1.f, 1.f);
		pb->m_linkedObject = cell;
		pb->Init();

		BoundingBox *bb = new BoundingBox();
		bb->SetID(UniqueID::GetID(bb->GetClassName()));
		bb->m_halfExtents = 0.5f * GameConstants::CELL_SIZE * Vector3(1.f);
		bb->Init();

// 		BoundingSphere *bs = new BoundingSphere();
// 		bs->SetID(UniqueID::GetID(bs->GetClassName()));
// 		bs->m_radius = 0.5f;
// 		bs->Init();

		pb->AddComponent(bb);
		//pb->AddComponent(bs);
		cell->AddComponent(pb);
	}

	return cell;
}

PhysicsBody * SceneObjectSpawner::SpawnRobot(const Vector2 mapCoords, MapManager *mapManager)
{
	PhysicsBody *pb = new PhysicsBody();
	pb->SetID(UniqueID::GetID(pb->GetClassName()));
	pb->m_transform.SetPos(GameConstants::ToWorldCoords(mapCoords, GameConstants::WALL_HEIGHT));	
	pb->m_transform.ComputeWorld();
	pb->m_transform.m_relative = false;
	pb->m_kinematic = false;
	pb->m_mass = 1.f;
	pb->m_inertia = Math::SphereInertia(pb->m_mass, GameConstants::CELL_SIZE / 2.f);
	pb->Init();

	BoundingSphere *bs = new BoundingSphere();
	bs->SetID(UniqueID::GetID(bs->GetClassName()));
	bs->m_radius = GameConstants::CELL_SIZE / 2.f;
	bs->Init();

	pb->AddComponent(bs);

	Robot *robot = new Robot();
	robot->SetID(UniqueID::GetID(robot->GetClassName()));
	robot->SetMapManager(mapManager);
	robot->SetPhysicsBody(pb);
	robot->Init();

	pb->m_linkedObject = robot;
	pb->AddComponent(robot);


	Model *robotModel = GetRobotModel();
	Shader *robotShader = GetRobotShader();

	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	//VisualBody *vb = new VisualBody(Vector3(1.f, 0.f, 1.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(robotModel);
	vb->SetShader(robotShader);

	Texture *tex = ResourceManager::GetInstance()->LoadTexture(PLAYER_ROBOT_TEXTURE);

	if (nullptr != tex)
	{
		vb->AddTexture(tex);
	}

	vb->Init();
	pb->AddComponent(vb);

	return pb;
}

Texture * SceneObjectSpawner::ChooseRandomTexture(const MapObjectType type)
{
	///TODO random
	switch (type)
	{
	case GROUND:
		return ResourceManager::GetInstance()->LoadTexture(GROUND_TEXTURES[0]);
	case WALL:
		return ResourceManager::GetInstance()->LoadTexture(WALL_TEXTURES[0]);
	default:
		return nullptr;
	}
}

Model * SceneObjectSpawner::GetMapCellModel()
{
	return ResourceManager::GetInstance()->LoadModel("2");
}

Shader * SceneObjectSpawner::GetMapCellShader()
{
	return  ResourceManager::GetInstance()->LoadShader("3");
}

Model * SceneObjectSpawner::GetRobotModel()
{
	return ResourceManager::GetInstance()->LoadModel("2");
}

Shader * SceneObjectSpawner::GetRobotShader()
{
	return  ResourceManager::GetInstance()->LoadShader("3");
}
