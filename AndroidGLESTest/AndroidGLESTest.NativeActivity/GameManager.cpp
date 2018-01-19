#include "GameManager.h"
#include "PrintUtils.h"
#include "DeltaTime.h"

#include <ctime>





GameManager::GameManager()
{
	m_parent = nullptr;
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

	SceneManager::GetInstance()->LoadFromFile("XMLs/sceneManager.xml");

	AddComponent(SceneManager::GetInstance());

	PrintUtils::PrintI(ToStringTree());
	
}

void GameManager::FixedUpdate()
{
	
}

void GameManager::Update()
{
	
}

void GameManager::Draw()
{
	
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

	if (timeSpentMili >= DeltaTime::PHYSICS_TIME_STEP_MS)
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
