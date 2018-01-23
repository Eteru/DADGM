#include "GameManager.h"

#include <ctime>

#include "PrintUtils.h"
#include "DeltaTime.h"

#include "SceneObjectSpawner.h"

#include "CollisionDetection.h"

#include "Robot.h"
#include "UniqueID.h"

#include "DebugDrawPrimitives.h"



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
	m_initTime = std::chrono::high_resolution_clock::now();
	m_lastFixedTime = m_initTime;
	m_lastFrameTime = m_initTime;
	DeltaTime::SetDt(0);

	SceneManager::GetInstance()->SetID(UniqueID::GetID(SceneManager::GetInstance()->GetClassName()));

	//SceneManager::GetInstance()->LoadFromFile("XMLs/sceneManager.xml");
	SceneManager::GetInstance()->LoadFromFile("XMLs/hartaDeTest.xml");

	

	m_mapManager = new MapManager();
	m_mapManager->SetID(UniqueID::GetID(m_mapManager->GetClassName()));
	m_mapManager->Init();

	AddComponent(m_mapManager);

	PhysicsBody *robotPB = SceneObjectSpawner::SpawnRobot(Vector2(1, 1), m_mapManager);
	robotPB->m_topSpeed = 5;
	robotPB->m_acceleration = 2;
	robotPB->m_mass = 1.f;
	
	AddComponent(robotPB);

	SceneManager::GetInstance()->GetActiveCamera()->SetFollowingObject(robotPB, 15);

	AddComponent(SceneManager::GetInstance());



	Robot *robotelTest = dynamic_cast<Robot *>(robotPB->m_linkedObject);

	robotelTest->MoveTowards(Vector2(13, 13));

	PrintUtils::PrintI(ToStringTree());

	m_debugDraw = true;
}

void GameManager::FixedUpdate()
{
 	std::vector<GameLoopObject *> allPhysicsBodies = FindComponentsTree("PhysicsBody");

	//PrintUtils::PrintD("Found " + PrintUtils::ToString(allPhysicsBodies.size()) + " physicsBodies");


	std::vector<BVIntersections::ContactInfo> allCollisions = CollisionDetection::DetectCollisions(allPhysicsBodies);

	for (BVIntersections::ContactInfo contact : allCollisions)
	{
		// Collision response goes here
	}

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
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> crtTime = std::chrono::high_resolution_clock::now();

	long long timeSpentNano = std::chrono::duration_cast<std::chrono::nanoseconds>(crtTime - m_lastFixedTime).count();

	float timeSpentMili = (float)timeSpentNano / 1000000.f;

	float timeSinceLastFrame = std::chrono::duration_cast<std::chrono::nanoseconds>(crtTime - m_lastFrameTime).count() / 1000000.f;
	DeltaTime::SetDt(timeSinceLastFrame / 1000.f);
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
