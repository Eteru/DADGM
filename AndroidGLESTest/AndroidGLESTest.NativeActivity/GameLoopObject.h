#pragma once

#include <vector>
#include <unordered_map>

class GameLoopObject
{
public:
	//virtual ~GameLoopObject();

	virtual void Init() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;

	virtual std::string GetClassName() = 0;

	virtual void AddComponent(GameLoopObject *component) final;
	virtual GameLoopObject *FindComponent(const std::string className) final;
	virtual std::vector<GameLoopObject *> FindComponents(const std::string className) final;
	
	

	size_t GetID() const { return m_ID; }
	void SetID(size_t val) { m_ID = val; }

	GameLoopObject * GetParent() const { return m_parent; }
	void SetParent(GameLoopObject * val) { m_parent = val; }
private:

	virtual void _Init() final;
	virtual void _FixedUpdate() final;
	virtual void _Update() final;
	virtual void _Draw() final;
	virtual void _Destroy() final;

	size_t m_ID;

	GameLoopObject *m_parent;
	std::unordered_map<std::string, std::vector<GameLoopObject *>> m_children;

};

inline void GameLoopObject::AddComponent(GameLoopObject *component)
{
	component->SetParent(this);
	m_children[component->GetClassName()].push_back(component);
}

inline GameLoopObject *GameLoopObject::FindComponent(const std::string className)
{
	if (!m_children.count(className))
		return nullptr;

	return m_children.at(className)[0];
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponents(const std::string className)
{
	if (!m_children.count(className))
		return std::vector<GameLoopObject *> ();

	return m_children.at(className);
}

inline void GameLoopObject::_Init()
{
	Init();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_Init();
		}
	}
}

inline void GameLoopObject::_FixedUpdate()
{
	FixedUpdate();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_FixedUpdate();
		}
	}
}

inline void GameLoopObject::_Update()
{
	Update();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_Update();
		}
	}
}

inline void GameLoopObject::_Draw()
{
	Draw();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_Draw();
		}
	}
}

inline void GameLoopObject::_Destroy()
{
	Destroy();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_Destroy();
		}
	}
}
