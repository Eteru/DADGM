#include "GameManager.h"
#include "PrintUtils.h"
#include "DeltaTime.h"

#include <ctime>





GameManager::GameManager()
{
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


	if (timeSpentMili >= DeltaTime::PHYSICS_TIME_STEP_MS)
	{
		PrintUtils::PrintI("Calling Fixed Update after " + PrintUtils::ToString(timeSpentMili) + " ms");
		_FixedUpdate();
		m_lastFixedTime = crtTime;
	}

	float timeSinceLastFrame = std::chrono::duration_cast<std::chrono::nanoseconds>(crtTime - m_lastFrameTime).count() / 1000000.f;

	PrintUtils::PrintI("Calling Update after " + PrintUtils::ToString(timeSinceLastFrame) + " ms");

	DeltaTime::SetDt(timeSpentMili);
	m_lastFrameTime = crtTime;

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
