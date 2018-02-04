
#include "SceneObjectSpawner.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "UniqueID.h"
#include "PhysicsBody.h"
#include "BoundingSphere.h"
#include "AIController.h"
#include "Projectile.h"

#include "Math.h"


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

	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(1.f), "MapCell", true);
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
	pb->m_debugDraw = true;
	pb->Init();

	BoundingSphere *bs = new BoundingSphere();
	bs->SetID(UniqueID::GetID(bs->GetClassName()));
	bs->m_radius = GameConstants::CELL_SIZE / 2.f;
	bs->Init();

	pb->AddComponent(bs);

	Robot *robot = new Robot();
	robot->SetID(UniqueID::GetID(robot->GetClassName()));
	//robot->SetMapManager(mapManager);
	robot->SetPhysicsBody(pb);
	robot->Init();

	pb->m_linkedObject = robot;

	AIController *aic = new AIController();
	aic->SetID(UniqueID::GetID(aic->GetClassName()));
	aic->m_mapManager = mapManager;
	aic->m_physicsBody = pb;
	aic->m_robot = robot;
	aic->Init();
	robot->AddComponent(aic);

	pb->AddComponent(robot);


	Model *robotModel = GetRobotModel();
	Shader *robotShader = GetRobotShader();

	// This is for adding an armor to the robot.
	// It should work the same for adding a weapon.
	// Items stats should be either in the xml or stored as const values
	// in a static class.
	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	//VisualBody *vb = new VisualBody(Vector3(1.f, 0.f, 1.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(robotModel);
	vb->SetShader(robotShader);
	//vb->m_debugDraw = true;

	Texture *tex = ResourceManager::GetInstance()->LoadTexture(PLAYER_ROBOT_TEXTURE);

	if (nullptr != tex)
	{
		vb->AddTexture(tex);
	}

	vb->Init();

	Armor *armor = new Armor();
	armor->SetID(UniqueID::GetID(armor->GetClassName()));
	armor->Init();
	armor->AddComponent(vb);

	robot->SetArmor(armor);
	robot->AddComponent(armor);
	//robot->AddComponent(vb);
	//pb->AddComponent(vb);

	return pb;
}

Projectile * SceneObjectSpawner::SpawnProjectile(const Vector3 worldCoords, Robot *target, size_t team, float damage, float ttl, size_t bounces, bool isSeeker, float speed, float knockback)
{
	PhysicsBodyDumb *pb = new PhysicsBodyDumb();
	pb->SetID(UniqueID::GetID(pb->GetClassName()));
	pb->m_transform.SetPos(worldCoords);
	pb->m_transform.ComputeWorld();
	pb->m_transform.m_relative = false;
	pb->m_kinematic = false;
	pb->m_mass = 0.1f;
	pb->m_inertia = Math::SphereInertia(pb->m_mass, GameConstants::CELL_SIZE / 5.f);
	//pb->m_debugDraw = true;
	pb->m_initialTarget = target->m_transform.GetWorldPos();
	pb->m_topSpeed = speed;
	

	

	if (isSeeker)
	{
		pb->m_turningAcceleration = 0.5f;
		pb->m_topTurningSpeed = 0.5f;
	}

	pb->Init();

	BoundingSphere *bs = new BoundingSphere();
	bs->SetID(UniqueID::GetID(bs->GetClassName()));
	bs->m_radius = GameConstants::CELL_SIZE / 5.f;
	bs->Init();

	pb->AddComponent(bs);


	Projectile *proj = new Projectile();
	proj->SetID(UniqueID::GetID(proj->GetClassName()));
	
	proj->m_team = team;
	proj->m_enemyTeam = target->m_team;
	proj->m_stats[StatType::DAMAGE] = Stat(damage);
	proj->m_stats[StatType::LINEAR_TOP] = Stat(speed);
	proj->m_stats[StatType::BOUNCES] = Stat(bounces);
	proj->m_stats[StatType::LIFE_TIME] = Stat(ttl);
	proj->m_stats[StatType::KNOCKBACK] = Stat(knockback);
	proj->m_isSeeker = isSeeker;
	proj->m_physicsBody = pb;

	proj->Init();
	pb->m_linkedObject = proj;

	pb->AddComponent(proj);


	///TODO actual textures and model
	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.2f), "Proj", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(GetRobotModel());
	vb->SetShader(GetMapCellShader());

	Texture *tex = ResourceManager::GetInstance()->LoadTexture(PLAYER_ROBOT_TEXTURE);

	if (nullptr != tex)
	{
		vb->AddTexture(tex);
	}

	vb->Init();

	proj->AddComponent(vb);

	pb->SetTarget(target->m_transform.GetWorldPos());

	return proj;
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

