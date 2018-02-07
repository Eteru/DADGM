#pragma once
#include "Structs.h"

#include "GameLoopObject.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "XMLParser.h"
#include "ButtonList.h"

class Robot;

class GameManager : public GameLoopObject
{
public:
	GameManager();
	virtual ~GameManager();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;



	bool IsGameWon();
	bool IsGameLost();



	// Astea se apeleaza doar de GM, care e root-ul ierarhiei. Restul claselor nu apeleaza explicit functiile cu "_"	
	void UpdateTree();
	void DrawTree();
	void DestroyTree();

	void Play(int);
	void PlayRandom(int);
	void SetPresset(int index);
private:

	void LoadRandomLevel();

	ButtonList *m_menu;
	Robot *m_playerRobot;
	std::vector<Robot *> m_enemyRobots;

	MapManager * m_mapManager;

	TimePointNano m_initTime;
	TimePointNano m_lastFixedTime; // last fixed update
	TimePointNano m_lastFrameTime;	
};
