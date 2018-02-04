#pragma once

#include <vector>
#include <unordered_map>

#include "Math.h"

#include "PrintUtils.h"
#include "Transform.h"
#include "DeltaTime.h"
#include "GameConstants.h"

class GameLoopObject
{
public:
	virtual ~GameLoopObject() {}

	virtual void Init();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void Draw();
	virtual void DebugDraw();
	virtual void Destroy();
	

	virtual std::string ToString() = 0;
	virtual std::string GetClassName() = 0;

	virtual void OnTouchDown(const int x, const int y);
	virtual void OnTouchUp(const int x, const int y);
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y);

	virtual void AddComponent(GameLoopObject *component) final;

	GameLoopObject *FindComponent(const std::string className);
	GameLoopObject *FindComponent(const std::string className, const size_t id);

	GameLoopObject *FindComponentTree(const std::string className);

	GameLoopObject *FindComponentPrefix(const std::string classPrefix);

	std::vector<GameLoopObject *> FindComponents(const std::string className);
	std::vector<GameLoopObject *> FindComponentsPrefix(const std::string classPrefix);
	std::vector<GameLoopObject *> FindComponentsTree(const std::string className);
	std::vector<GameLoopObject *> FindComponentsTreePrefix(const std::string classPrefix);

	void RemoveComponent(GameLoopObject *child);
	
	void DestroyComponents(const std::string className);

	virtual std::string ToStringTree(int indent = 0) final;

	void DestroyRec();

	size_t GetID() const { return m_ID; }
	void SetID(size_t val) { m_ID = val; }

	GameLoopObject * GetParent() const { return m_parent; }
	void SetParent(GameLoopObject * val) { m_parent = val; }

	Transform m_transform;
	bool m_debugDraw = false;
protected:

	virtual void _FixedUpdate() final;
	virtual void _Update() final;
	virtual void _Draw() final;
	virtual void _Destroy() final;
	virtual void _DebugDraw() final;

	size_t m_ID;

	GameLoopObject *m_parent;
	std::unordered_map<std::string, std::vector<GameLoopObject *>> m_children;
private:
	void _FindComponentsTree(const std::string className, std::vector<GameLoopObject *> &result);
	void _FindComponentsTreePrefix(const std::string classPrefix, std::vector<GameLoopObject *> &result);

	void _UpdateTransforms(const Transform &parent);

};


inline void GameLoopObject::Init() {}
inline void GameLoopObject::FixedUpdate() {}
inline void GameLoopObject::Update() {}
inline void GameLoopObject::Draw() {}
inline void GameLoopObject::DebugDraw() {}

inline void GameLoopObject::Destroy() {}
inline void GameLoopObject::OnTouchDown(const int x, const int y) {}
inline void GameLoopObject::OnTouchUp(const int x, const int y) {}
inline void GameLoopObject::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) {}

inline void GameLoopObject::AddComponent(GameLoopObject *component)
{
	component->SetParent(this);
	component->_UpdateTransforms(m_transform);

	
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

inline GameLoopObject * GameLoopObject::FindComponentTree(const std::string className)
{
	GameLoopObject *obj = FindComponent(className);

	if (nullptr != obj)
		return obj;

	for (auto kvPair : m_children)
	{
		for (auto child : kvPair.second)
		{
			GameLoopObject *childObj = child->FindComponentTree(className);
			if (nullptr != childObj)
			{
				return childObj;
			}
		}
	}

	return nullptr;
}

inline GameLoopObject * GameLoopObject::FindComponentPrefix(const std::string classPrefix)
{
	for (auto kvPair : m_children)
	{
		if (std::equal(classPrefix.begin(), classPrefix.end(), kvPair.first.begin()))
		{
			return kvPair.second[0];
		}
	}

	return nullptr;
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponents(const std::string className)
{
	if (!m_children.count(className))
		return std::vector<GameLoopObject *>();

	return m_children.at(className);
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponentsPrefix(const std::string classPrefix)
{
	std::vector<GameLoopObject *> result;

	for (auto kvPair : m_children)
	{
		if (std::equal(classPrefix.begin(), classPrefix.end(), kvPair.first.begin()))
		{
			result.insert(result.end(), kvPair.second.begin(), kvPair.second.end());
		}
	}

	return result;
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponentsTree(const std::string className)
{
	std::vector<GameLoopObject *> result;

	_FindComponentsTree(className, result);

	return result;
}

inline std::vector<GameLoopObject *> GameLoopObject::FindComponentsTreePrefix(const std::string classPrefix)
{
	std::vector<GameLoopObject *> result;

	_FindComponentsTreePrefix(classPrefix, result);

	return result;
}

inline void GameLoopObject::RemoveComponent(GameLoopObject *child)
{
	if (!m_children.count(child->GetClassName()))
		return;

	std::vector<GameLoopObject *> *pVec = &m_children.at(child->GetClassName());

	for (auto it = pVec->begin(); it != pVec->end(); ++it)
	{
		if ((*it)->GetID() == child->GetID())
		{
			pVec->erase(it);
			return;
		}
	}

}

inline void GameLoopObject::DestroyComponents(const std::string className)
{
	if (!m_children.count(className))
		return;

	for (auto child : m_children.at(className))
	{
		child->_Destroy();
		delete child;
		child = nullptr;
	}

	m_children.erase(className);
}

inline void GameLoopObject::_FindComponentsTree(const std::string className, std::vector<GameLoopObject *> &result)
{
	std::vector<GameLoopObject *> comps = FindComponents(className);
	if (!comps.empty())
	{
		result.insert(result.end(), comps.begin(), comps.end());
	}

	for (auto kvPair : m_children)
	{
		for (auto child : kvPair.second)
		{
			child->_FindComponentsTree(className, result);
		}
	}


}

inline void GameLoopObject::_FindComponentsTreePrefix(const std::string classPrefix, std::vector<GameLoopObject *> &result)
{
	std::vector<GameLoopObject *> comps = FindComponentsPrefix(classPrefix);

	if (!comps.empty())
	{
		result.insert(result.end(), comps.begin(), comps.end());
	}

	for (auto kvPair : m_children)
	{
		for (auto child : kvPair.second)
		{
			child->_FindComponentsTreePrefix(classPrefix, result);
		}
	}
}

inline void GameLoopObject::_UpdateTransforms(const Transform &parent)
{
	m_transform.SetParent(parent);

	for (auto kvPair : m_children)
	{
		for (auto child : kvPair.second)
		{
			child->_UpdateTransforms(m_transform);
		}
	}
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

inline void GameLoopObject::DestroyRec()
{
	if (m_parent)
		m_parent->RemoveComponent(this);

	_Destroy();
}

inline void GameLoopObject::_FixedUpdate()
{
	// Save current transform	
	m_transform.SaveCurrentParams();

	if (m_transform.m_relative)
	{
		if (nullptr != m_parent)
		{
			m_transform.SetParentPos(m_parent->m_transform.GetWorldPos());
			m_transform.SetParentRot(m_parent->m_transform.GetWorldRot());
			m_transform.SetParentScale(m_parent->m_transform.GetWorldScale());
		}
	}


	// In FixedUpdate, the next transform is computed (every GLO is responsible for this)
	FixedUpdate();

	if (m_transform.m_relative)
	{
		if (nullptr != m_parent)
		{
			m_transform.ComputeWorld();
		}
	}

	m_transform.ResetLerps();

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
	m_transform.StepLerps(DeltaTime::GetDt() / GameConstants::PHYSICS_TIME_STEP);
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

inline void GameLoopObject::_DebugDraw()
{
	if (m_debugDraw)
	{
		m_transform.DebugDraw();
		DebugDraw();
	}

	for (auto kvPair : m_children)
	{
		for (auto component : kvPair.second)
		{
			component->_DebugDraw();
		}
	}
}
