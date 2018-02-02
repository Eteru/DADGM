#include "GameManager.h"

#include <ctime>

#include "PrintUtils.h"
#include "DeltaTime.h"

#include "SceneObjectSpawner.h"

#include "CollisionDetection.h"

#include "Robot.h"
#include "UniqueID.h"

#include "DebugDrawPrimitives.h"
#include "RigidCollisionResponse.h"
#include "Level.h"

#include "ItemDescriptions.h"



GameManager::GameManager()
{
	m_parent = nullptr;
	m_transform.m_relative = false;
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	m_initTime = Now();
	m_lastFixedTime = m_initTime;
	m_lastFrameTime = m_initTime;
	DeltaTime::SetDt(0);

	SceneManager::GetInstance()->SetID(UniqueID::GetID(SceneManager::GetInstance()->GetClassName()));

	//SceneManager::GetInstance()->LoadFromFile("XMLs/sceneManager.xml");
	SceneManager::GetInstance()->LoadFromFile("XMLs/hartaDeTest.xml");


	LoadRandomLevel();

	AddComponent(SceneManager::GetInstance());


	PrintUtils::PrintI(ToStringTree());

	m_debugDraw = true;
}

void GameManager::FixedUpdate()
{
 	std::vector<GameLoopObject *> allPhysicsBodies = FindComponentsTreePrefix("PhysicsBody");

	//PrintUtils::PrintD("Found " + PrintUtils::ToString(allPhysicsBodies.size()) + " physicsBodies");


	std::vector<BVIntersections::ContactInfo> allCollisions = CollisionDetection::DetectCollisions(allPhysicsBodies);

	for (BVIntersections::ContactInfo contact : allCollisions)
	{
		RigidCollisionResponse::ApplyImpulses(contact);
		contact.m_o1->OnCollision(contact.m_o2);
	}

	static TimePointNano last = Now();
	TimePointNano crt = Now();

	if (Duration(last, crt) >= 5)
	{
// 		PhysicsBody *projpb = SceneObjectSpawner::SpawnProjectile(Vector2(10, 10), static_cast<Robot*>(FindComponentTree("Robot")), 1, 5, 0, false, 5); 
// 		AddComponent(projpb);

		LoadRandomLevel();

		last = crt;
	}

}

void GameManager::LoadRandomLevel()
{
	DestroyComponents("Level");


	Level *level = new Level();
	level->SetID(UniqueID::GetID(level->GetClassName()));
	level->Init();
	
	
	m_mapManager = new MapManager();
	m_mapManager->SetID(UniqueID::GetID(m_mapManager->GetClassName()));
	m_mapManager->InitFromDesc(ItemDescriptions::GetInstance().GetRandomMap());

	level->AddComponent(m_mapManager);

	std::pair<Vector2, Vector2> spawnPoints = m_mapManager->GetRandomOptimalSpawns();

	Robot *playerRobot = ItemDescriptions::GetInstance().GetRandomRobot(spawnPoints.first, m_mapManager);

	level->AddComponent(playerRobot->m_physicsBody);

	SceneManager::GetInstance()->GetActiveCamera()->SetFollowingObject(playerRobot->m_physicsBody, 15);


	Robot *enemyRobot = ItemDescriptions::GetInstance().GetRandomRobot(spawnPoints.second, m_mapManager);
	
	PhysicsBody *enemyPB = enemyRobot->m_physicsBody;	

	level->AddComponent(enemyPB);

	AddComponent(level);
}

void GameManager::Update()
{

}

void GameManager::Draw()
{
// 	DebugDrawPrimitives::DrawCube(Vector3(10.f, 0.f, 10.f), Vector3(0.f), Vector3(0.5f), DebugDrawPrimitives::COLOR_RED);
// 	DebugDrawPrimitives::DrawSphere(Vector3(10.f, 2.f, 11.f), Vector3(0.f), Vector3(1.f), DebugDrawPrimitives::COLOR_BLUE);
// 
// 	DebugDrawPrimitives::DrawLine(Vector3(-1.f), Vector3(1.f), DebugDrawPrimitives::COLOR_YELLOW);

	
}

void GameManager::DebugDraw()
{	
// 	MapManager *manager = dynamic_cast<MapManager *>(FindComponent("MapManager"));
// 
// 	static std::vector<Vector2> path = manager->FindPath(Vector2(1, 1), Vector2(13, 13));
// 
// 	DebugDrawPrimitives::DrawSphere(Vector3(1, GameConstants::WALL_HEIGHT, 1), 0.5, DebugDrawPrimitives::COLOR_RED);
// 	for (int i = 0; i < path.size() - 1; ++i)
// 	{
// 		PrintUtils::PrintD("WAA " + path[i].ToString());
// 		DebugDrawPrimitives::DrawLine(Vector3(path[i].x, GameConstants::WALL_HEIGHT, path[i].y), Vector3(path[i + 1].x, GameConstants::WALL_HEIGHT, path[i + 1].y), DebugDrawPrimitives::COLOR_RED);
// 	}
}

void GameManager::Destroy()
{	
}

std::string GameManager::GetClassName()
{
	return std::string("GameManager");
}

std::string GameManager::ToString()
{
	return "GM test";
}


void GameManager::UpdateTree()
{
	TimePointNano crtTime = Now();

	long long timeSpentNano = DurationNano(m_lastFixedTime, crtTime);

	float timeSpentMili = (float)timeSpentNano / 1000000.f;

	float timeSinceLastFrame = NToS(DurationNano(m_lastFrameTime, crtTime));
	DeltaTime::SetDt(timeSinceLastFrame);
	m_lastFrameTime = crtTime;

	if (timeSpentMili >= GameConstants::PHYSICS_TIME_STEP_MS)
	{
		//PrintUtils::PrintI("Calling Fixed Update after " + PrintUtils::ToString(timeSpentMili) + " ms");
		_FixedUpdate();
		m_lastFixedTime = crtTime;
	}


	//PrintUtils::PrintI("Calling Update after " + PrintUtils::ToString(timeSinceLastFrame) + " ms");


	_Update();
}

void GameManager::DrawTree()
{
	_Draw();
	_DebugDraw();
}

void GameManager::DestroyTree()
{
	_Destroy();
}
