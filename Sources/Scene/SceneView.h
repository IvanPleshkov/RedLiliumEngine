#pragma once

/*

#include <Core/Common.h>
#include <Scene/Entity.h>

namespace RED_LILIUM_NAMESPACE
{

class Entity;
class Component;

template<class ...Components>
struct SceneView
{
	using Tuple = std::tuple<ptr<const Components>...>;
	using Entities = std::deque<ptr<const Entity>>;

	SceneView(ptr<const Entity> root)
		: m_entities()
	{
		FindEntities(root);
	}

	SceneView(std::initializer_list<ptr<const Entity>> roots)
		: m_entities()
	{
		for (auto& root : roots)
		{
			FindEntities(root);
		}
	}

	SceneView(span<ptr<const Entity>> roots)
		: m_entities()
	{
		for (auto& root : roots)
		{
			FindEntities(root);
		}
	}

	SceneView(std::vector<ptr<const Entity>> roots)
		: m_entities()
	{
		for (auto& root : roots)
		{
			FindEntities(root);
		}
	}

	~SceneView() = default;

	Entities::iterator begin()
	{
		return m_entities.begin();
	}

	Entities::iterator end()
	{
		return m_entities.end();
	}

private:
	static bool IsValid(ptr<const Entity> entity)
	{
		return (... && (entity->GetComponent<const Components>() != nullptr));
	}

	static Tuple GetTuple(ptr<const Entity> entity)
	{
		return Tuple(entity->GetComponent<const Components>()...);
	}

	void FindEntities(ptr<const Entity> root)
	{
		if (IsValid(root))
		{
			m_entities.push_back(root);
		}

		auto cnt = root->GetChildrenCount();
		for (u32 i = 0; i < cnt; ++i)
		{
			FindEntities(root->GetChild(i));
		}
	}

	Entities m_entities;
};

} // namespace RED_LILIUM_NAMESPACE

*/
