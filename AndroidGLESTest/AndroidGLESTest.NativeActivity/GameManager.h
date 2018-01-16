#pragma once
#include <chrono>

#include "GameLoopObject.h"
#include "SceneManager.h"


class GameManager : public GameLoopObject
{
public:
	GameManager();
	virtual ~GameManager();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;



	// Astea se apeleaza doar de GM, care e root-ul ierarhiei. Restul claselor nu apeleaza explicit functiile cu "_"	
	void UpdateTree();
	void DrawTree();
	void DestroyTree();

private:

	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> m_initTime;
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> m_lastFixedTime; // last fixed update
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> m_lastFrameTime;
};
