#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Entity;
class Component;

template<class ...Ts>
struct SceneView
{
	using Tuple = std::tuple<ptr<Ts>...>;

	struct Iterator
	{
		Iterator(ptr<SceneView> sceneView)
			: m_sceneView(sceneView)
		{}

		Iterator operator++()
		{
			if (m_sceneView->FindNext())
			{
				return Iterator(m_sceneView);
			}
			else
			{
				return Iterator(nullptr);
			}
		}
		Tuple operator*()
		{
			return m_sceneView->GetCurrent();
		}
		Tuple operator->()
		{
			return m_sceneView->GetCurrent();
		}
		bool operator == (const Iterator& i)
		{
			return m_sceneView == i.m_sceneView;
		}
		bool operator != (const Iterator& i)
		{
			return m_sceneView != i.m_sceneView;
		}

	private:
		ptr<SceneView> m_sceneView;
	};

public:
	SceneView(ptr<Entity> root)
		: m_roots()
		, m_stack()
		, m_parentIndex()
	{
		m_roots.push_back(root);
	}
	SceneView(std::initializer_list<ptr<Entity>> roots)
		: m_roots(roots)
		, m_stack()
		, m_parentIndex()
	{}
	~SceneView() = default;

	Iterator begin()
	{
		if (m_roots.empty())
		{
			return Iterator(nullptr);
		}

		RED_LILIUM_ASSERT(m_stack.empty() && m_parentIndex.empty());
		m_stack.push(m_roots.front());
		m_parentIndex.push(0);
		if (FindNext())
		{
			return Iterator(this);
		}
		else
		{
			return Iterator(nullptr);
		}
	}
	Iterator end()
	{
		return Iterator(nullptr);
	}

public: // only for iterator
	bool IsValid()
	{
		return (m_stack.top()->GetComponent<Ts>() != nullptr)...;
	}
	bool FindNext()
	{
		if (m_stack.empty())
		{
			m_stack.push(m_roots.front());
			m_parentIndex.push(0);
		}

		while (true)
		{
			if (m_stack.empty())
			{
				return false;
			}

			if (IsValid())
			{
				return true;
			}

			if (m_stack.size() == 1)
			{

			}
			else
			{
			}
		}
		return false;
	}
	Tuple GetCurrent()
	{
		return Tuple(m_stack.top()->GetComponent<Ts>()...);
	}

private:
	std::vector<ptr<Entity>> m_roots;
	std::stack<ptr<Entity>> m_stack;
	std::stack<u32> m_parentIndex;
};

} // namespace RED_LILIUM_NAMESPACE
