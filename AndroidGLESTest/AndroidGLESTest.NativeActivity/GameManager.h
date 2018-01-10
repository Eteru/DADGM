#pragma once

class GameManager
{
public:
	GameManager();
	virtual ~GameManager();

	void Init();
	void Run();

private:
	void FixedUpdate();
	void Update();
	void Draw();
};
