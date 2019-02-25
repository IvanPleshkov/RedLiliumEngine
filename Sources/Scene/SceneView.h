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
		: m_trees()
	{
		m_trees.push_back(IteratorTree(root));
	}
	SceneView(std::initializer_list<ptr<Entity>> roots)
		: m_trees()
	{
		for (auto& root : roots)
		{
			m_trees.push(IteratorTree(root));
		}
	}
	~SceneView() = default;

	Iterator begin()
	{
		if (m_trees.empty())
		{
			return Iterator(nullptr);
		}
		if (IsValid())
		{
			return Iterator(this);
		}
		else
		{
			if (FindNext())
			{
				return Iterator(this);
			}
			else
			{
				return Iterator(nullptr);
			}
		}
	}
	Iterator end()
	{
		return Iterator(nullptr);
	}

public: // only for iterator
	bool IsValid()
	{
		return (... && (GetCurrent()->GetComponent<Ts>() != nullptr));
	}
	bool FindNext()
	{
		do
		{
			if (!m_trees.top().Next())
			{
				m_trees.pop();
			}
		} while (!m_trees.empty() && !IsValid());
		return !m_trees.empty();
	}
	Tuple GetCurrent()
	{
		return Tuple(m_trees.top().GetCurrent()->GetComponent<Ts>()...);
	}

private:
	struct IteratorTree
	{
		IteratorTree(ptr<Entity> root)
			: m_stack()
			, m_index()
		{
			m_stack.push(root);
		}

		ptr<Entity> GetCurrent()
		{
			return m_stack.top();
		}

		bool Next()
		{
			RED_LILIUM_ASSERT(!m_stack.empty());
			
			return !m_stack.empty();
		}

		std::stack<ptr<Entity>> m_stack;
		std::stack<u32> m_index;
	};

	std::stack<IteratorTree> m_trees;
};

} // namespace RED_LILIUM_NAMESPACE
