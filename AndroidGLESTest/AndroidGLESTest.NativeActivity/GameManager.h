#pragma once
#include "GameLoopObject.h"

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
	void FixedUpdateTree();
	void UpdateTree();
	void DrawTree();
	void DestroyTree();
};
