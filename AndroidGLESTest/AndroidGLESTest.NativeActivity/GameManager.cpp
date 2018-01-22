#include "GameManager.h"

#include <ctime>

#include "PrintUtils.h"
#include "DeltaTime.h"

#include "SceneObjectSpawner.h"

#include "CollisionDetection.h"

#include "Robot.h"
#include "UniqueID.h"
#include "MapManager.h"
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

	

	MapManager *mapManager = new MapManager();
	mapManager->SetID(UniqueID::GetID(mapManager->GetClassName()));
	mapManager->Init();

	AddComponent(mapManager);

	PhysicsBody *robotPB = SceneObjectSpawner::SpawnRobot(Vector2(3, 3));
	robotPB->m_topSpeed = 3;
	robotPB->m_acceleration = 3;
	robotPB->m_mass = 1.f;
	robotPB->SetTarget(Vector3(10, GameConstants::WALL_HEIGHT, 10));
	AddComponent(robotPB);

	SceneManager::GetInstance()->GetActiveCamera()->SetFollowingObject(robotPB, 15);

	AddComponent(SceneManager::GetInstance());



	PrintUtils::PrintI(ToStringTree());
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
	DebugDrawPrimitives::DrawCube(Vector3(10.f, 0.f, 10.f), Vector3(0.f), Vector3(100.f), DebugDrawPrimitives::COLOR_RED);
	DebugDrawPrimitives::DrawSphere(Vector3(10.f, 0.f, 10.f), Vector3(0.f), Vector3(100.f), DebugDrawPrimitives::COLOR_BLUE);

	DebugDrawPrimitives::DrawLine(Vector3(-100.f), Vector3(100.f), DebugDrawPrimitives::COLOR_YELLOW);
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
}

void GameManager::DestroyTree()
{
	_Destroy();
}
