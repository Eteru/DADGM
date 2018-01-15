#pragma once

#include <vector>
#include <unordered_map>

#include "PrintUtils.h"

class GameLoopObject
{
public:
	//virtual ~GameLoopObject();

	virtual void Init() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
	virtual std::string ToString() = 0;

	virtual std::string GetClassName() = 0;


	virtual void OnTouchDown(const int x, const int y);
	virtual void OnTouchUp(const int x, const int y);
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y);

	virtual void AddComponent(GameLoopObject *component) final;

	GameLoopObject *FindComponent(const std::string className);
	GameLoopObject *FindComponent(const std::string className, const size_t id);

	std::vector<GameLoopObject *> FindComponents(const std::string className);
									

	virtual std::string ToStringTree(int indent = 0) final;



	size_t GetID() const { return m_ID; }
	void SetID(size_t val) { m_ID = val; }

	GameLoopObject * GetParent() const { return m_parent; }
	void SetParent(GameLoopObject * val) { m_parent = val; }

protected:

	virtual void _FixedUpdate() final;
	virtual void _Update() final;
	virtual void _Draw() final;
	virtual void _Destroy() final;

	size_t m_ID;

	GameLoopObject *m_parent;
	std::unordered_map<std::string, std::vector<GameLoopObject *>> m_children;

};

inline void GameLoopObject::OnTouchDown(const int x, const int y) {}
inline void GameLoopObject::OnTouchUp(const int x, const int y) {}
inline void GameLoopObject::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) {}

inline void GameLoopObject::AddComponent(GameLoopObject *component)
{
	component->Init();
	component->SetParent(this);
	m_children[component->GetClassName()].push_back(component);
}

inline GameLoopObject * GameLoopObject::FindComponent(const std::string className)
{
	if (!m_children.count(className))
		return nullptr;

	return m_children.at(className)[0];
}

inline GameLoopObject * GameLoopObject::FindComponent(const std::string className, const size_t id)
{
	if (!m_children.count(className))
		return nullptr;

	std::vector<GameLoopObject *> bucket = m_children.at(className);

	for (auto obj : bucket)
	{
		if (obj->GetID() == id)
			return obj;
	}

	return nullptr;
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponents(const std::string className)
{
	if (!m_children.count(className))
		return std::vector<GameLoopObject *>();

	return m_children.at(className);
}

inline std::string GameLoopObject::ToStringTree(int indent /*= 0*/)
{
	std::string result = "";

	for (int i = 0; i < indent; ++i)
	{
		//result += " ";
		result += "\t";
	}

	result += GetClassName() + "[" + PrintUtils::ToString(m_ID) + "]: " + ToString();

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			result += "\n" + component->ToStringTree(indent + 1);
		}
	}

	return result;
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
	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_Destroy();
		}
	}

	Destroy();
}
