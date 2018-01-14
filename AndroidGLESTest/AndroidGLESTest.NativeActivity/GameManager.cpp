#include "GameManager.h"
#include "PrintUtils.h"



GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
//	PrintUtils::PrintD(ToStringTree());
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

void GameManager::FixedUpdateTree()
{
	_FixedUpdate();
}

void GameManager::UpdateTree()
{
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
