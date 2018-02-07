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
#include "HealthBar.h"


GameManager::GameManager()
{
	m_menu = nullptr;
	m_parent = nullptr;
	m_playerRobot = nullptr;
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

	m_crtPlayerPreset = 0;

	AddComponent(SceneManager::GetInstance());

	PrintUtils::PrintI(ToStringTree());

	m_debugDraw = false;

 	const engine *eng = SceneManager::GetInstance()->GetEngine();

	m_menu = new ButtonList(eng->height - 100.f, 50.f, eng->width, eng->height, {});
	m_menu->SetActive(true);

	ButtonList *robotPresets = new ButtonList(eng->height - 100.f, 50.f, eng->width, eng->height, {});
	for (int i = 0; i < ItemDescriptions::GetInstance().GetNumberOfRobots(); ++i)
	{
		robotPresets->AddButton("Preset " + PrintUtils::ToString(i + 1), nullptr);
		robotPresets->SetCustomFunction(0, std::bind(&GameManager::SetPlayerPreset, this, std::placeholders::_1));
	}
	//robotPresets->AddButton("Preset 2", nullptr);
	//robotPresets->SetCustomFunction(1, std::bind(&GameManager::SetPlayerPreset, this, std::placeholders::_1));


	m_menu->AddButton("Choose Robot", robotPresets);
	m_menu->AddButton("Play", nullptr);
	m_menu->SetCustomFunction(1, std::bind(&GameManager::Play, this, std::placeholders::_1));
	m_menu->AddButton("Play Random", nullptr);
	m_menu->SetCustomFunction(2, std::bind(&GameManager::PlayRandom, this, std::placeholders::_1));
	m_menu->Init();

	AddComponent(m_menu);

	m_player_hb = new HealthBar(eng->height - 150.f, 20.f, 600.f, 50.f, "Player", { "", DebugDrawPrimitives::COLOR_BLUE });
	m_player_hb->Init();
	AddComponent(m_player_hb);

	m_enemy_hb = new HealthBar(eng->height - 150.f, eng->width - 620.f, 600.f, 50.f, "Enemy", { "", DebugDrawPrimitives::COLOR_RED });
	m_enemy_hb->Init();
	AddComponent(m_enemy_hb);

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

	if (IsGameWon() || IsGameLost())
	{
		m_player_hb->InitRobot(nullptr);
		m_enemy_hb->InitRobot(nullptr);
		m_menu->SetActive(true);
	}
}

void GameManager::SetPlayerPreset(size_t id)
{
	m_crtPlayerPreset = id;
}

void GameManager::LoadPlayer(const Vector2 spawnPoint)
{
	m_playerRobot = ItemDescriptions::GetInstance().GetRobot(m_crtPlayerPreset, spawnPoint, m_mapManager);
	m_playerRobot->m_team = 0;
}

void GameManager::LoadRandomLevel()
{
	DestroyComponents("Level");

	if (nullptr != m_menu)
	{
		m_menu->SetActive(false);
	}

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

	LoadPlayer(spawnPoints.first);
	m_player_hb->InitRobot(m_playerRobot);

	level->AddComponent(m_playerRobot->m_physicsBody);

	SceneManager::GetInstance()->GetActiveCamera()->SetFollowingObject(m_playerRobot->m_physicsBody, 15);


	Robot *enemyRobot = ItemDescriptions::GetInstance().GetRandomRobot(spawnPoints.second, m_mapManager);
	enemyRobot->m_team = 1;

	m_enemyRobots.push_back(enemyRobot);
	m_enemy_hb->InitRobot(m_enemyRobots[0]);
	
	PhysicsBody *enemyPB = enemyRobot->m_physicsBody;	

	level->AddComponent(enemyPB);

	AddComponent(level);


	m_mapManager->m_robots.push_back(m_playerRobot);
	m_mapManager->m_robots.push_back(enemyRobot);
}

void GameManager::Update()
{
}

void GameManager::Draw()
{
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
	if (m_enemyRobots.size() == 0)
	{
		return false;
	}

	for (auto enemy : m_enemyRobots)
	{
		if (!enemy->IsDead())
			return false;
	}

	return true;
}

bool GameManager::IsGameLost()
{
	if (nullptr != m_playerRobot)
		return m_playerRobot->IsDead();

	return false;
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

void GameManager::Play(int)
{
	LoadRandomLevel();
}

void GameManager::PlayRandom(int)
{
	LoadRandomLevel();
}