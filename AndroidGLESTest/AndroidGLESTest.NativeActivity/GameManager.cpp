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

#include "Button.h"
#include "Tabs.h"
#include "StringRenderer.h"
#include "ObjectContactActions.h"



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

 	const engine *eng = SceneManager::GetInstance()->GetEngine();

 	Button *btn = new Button(0.f, 0.f, 1200.f, 250.f, "Salut Hurezene;", {"8", DebugDrawPrimitives::COLOR_BLUE});
 	btn->Init();
 	AddComponent(btn);
 
	Tabs *tabs = new Tabs(eng->height - 100.f, 0.f, eng->width, eng->height, {});
	tabs->Init();


	ButtonList *bl1 = new ButtonList(eng->height - 500.f, 0.f, eng->width, eng->height, {});
	bl1->SetActive(true);

	ButtonList *bl11 = new ButtonList(eng->height - 500.f, 0.f, eng->width, eng->height, {});
	bl11->AddButton("sbtn11", nullptr);
	bl11->AddButton("sbtn12", nullptr);
	bl11->AddButton("sbtn13", nullptr);

	bl1->AddButton("btn11", bl11);
	bl1->AddButton("btn12", nullptr);
	bl1->AddButton("btn13", nullptr);

	ButtonList *bl2 = new ButtonList(eng->height - 500.f, 0.f, eng->width, eng->height, {});
	bl2->AddButton("btn21", nullptr);
	bl2->AddButton("btn22", nullptr);
	bl2->AddButton("btn23", nullptr);
	ButtonList *bl3 = new ButtonList(eng->height - 500.f, 0.f, eng->width, eng->height, {});
	bl3->AddButton("btn31", nullptr);
	bl3->AddButton("btn32", nullptr);
	bl3->AddButton("btn33", nullptr);

	tabs->AddTab("Items", bl1);
	tabs->AddTab("Movement", bl2);
	tabs->AddTab("Item Rules", bl3);

	AddComponent(tabs);

 	
 	StringRenderer::Init("7", eng->width, eng->height);
}

void GameManager::FixedUpdate()
{
 	std::vector<GameLoopObject *> allPhysicsBodies = FindComponentsTreePrefix("PhysicsBody");

	//PrintUtils::PrintD("Found " + PrintUtils::ToString(allPhysicsBodies.size()) + " physicsBodies");


	std::vector<BVIntersections::ContactInfo> allCollisions = CollisionDetection::DetectCollisions(allPhysicsBodies);

	for (BVIntersections::ContactInfo contact : allCollisions)
	{
		ObjectContactActions::OnContact(contact);
		//RigidCollisionResponse::ApplyImpulses(contact, 0.5f);
		//contact.m_o1->OnCollision(contact.m_o2);
	}

	static TimePointNano last = Now();
	TimePointNano crt = Now();

	if (Duration(last, crt) >= 5)
	{
// 		PhysicsBody *projpb = SceneObjectSpawner::SpawnProjectile(Vector2(10, 10), static_cast<Robot*>(FindComponentTree("Robot")), 1, 5, 0, false, 5); 
// 		AddComponent(projpb);

		//LoadRandomLevel();

		last = crt;
	}

}

void GameManager::LoadRandomLevel()
{
	DestroyComponents("Level");

	m_playerRobot = nullptr;
	m_enemyRobots.clear();

	Level *level = new Level();
	level->SetID(UniqueID::GetID(level->GetClassName()));
	level->Init();
	
	
	m_mapManager = new MapManager();
	m_mapManager->SetID(UniqueID::GetID(m_mapManager->GetClassName()));
	m_mapManager->InitFromDesc(ItemDescriptions::GetInstance().GetRandomMap());

	level->AddComponent(m_mapManager);

	std::pair<Vector2, Vector2> spawnPoints = m_mapManager->GetRandomOptimalSpawns();

	Robot *playerRobot = ItemDescriptions::GetInstance().GetRandomRobot(spawnPoints.first, m_mapManager);
	playerRobot->m_team = 0;

	m_playerRobot = playerRobot;

	level->AddComponent(playerRobot->m_physicsBody);

	SceneManager::GetInstance()->GetActiveCamera()->SetFollowingObject(playerRobot->m_physicsBody, 15);


	Robot *enemyRobot = ItemDescriptions::GetInstance().GetRandomRobot(spawnPoints.second, m_mapManager);
	enemyRobot->m_team = 1;

	m_enemyRobots.push_back(enemyRobot);
	
	PhysicsBody *enemyPB = enemyRobot->m_physicsBody;	

	level->AddComponent(enemyPB);

	AddComponent(level);


	m_mapManager->m_robots.push_back(playerRobot);
	m_mapManager->m_robots.push_back(enemyRobot);
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

bool GameManager::IsGameWon()
{
	for (auto enemy : m_enemyRobots)
	{
		if (!enemy->IsDead())
			return false;
	}

	return true;
}

bool GameManager::IsGameLost()
{
	return m_playerRobot->IsDead();
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
