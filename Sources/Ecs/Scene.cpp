#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{
	m_components.push_back(std::vector<uptr<Component>>());
	m_children.push_back(std::vector<Entity>());
	m_parents.push_back(Entity());
	m_names.push_back("root");
	m_enable.push_back(true);
	Entity root;
	root.m_cluster = 0;
	root.m_index = 0;
	root.m_generation = 0;
	m_root = root;
}

Scene::~Scene()
{
}

Entity Scene::GetRoot() const
{
	return m_root;
}

const std::string& Scene::GetName(Entity entity) const
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_names.size());

	return m_names[entity.m_index];
}

Entity Scene::GetParent(Entity entity) const
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_parents.size());

	return m_parents[entity.m_index];
}

u32 Scene::GetChildrenCount(Entity entity) const
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_children.size());

	return static_cast<u32>(m_children[entity.m_index].size());
}

Entity Scene::GetChild(Entity entity, u32 index) const
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_children.size());

	auto& children = m_children[entity.m_index];
	RED_LILIUM_ASSERT(index < children.size());
	return children[index];
}

Entity Scene::GetChild(Entity entity, std::string_view name, u32 index) const
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_children.size());

	auto& children = m_children[entity.m_index];
	for (auto& child : children)
	{
		if (GetName(child) == name)
		{
			if (index == 0)
			{
				return child;
			}
			--index;
		}
	}

	return Entity();
}

sptr<Transaction> Scene::CreateTransaction(TransactionFlags flags)
{
	return smake<Transaction>(this, flags);
}

void Scene::ApplyTransactions()
{
}


Transaction::Transaction(ptr<Scene> scene, TransactionFlags flags)
{
}

Transaction::~Transaction()
{
}

void Transaction::Remove(Entity entity)
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_scene->m_children.size());
	RED_LILIUM_ASSERT(entity.m_index != 0);

	if (m_flags.Test(TransactionOptions::Immediate))
	{
		auto& children = m_scene->m_children[entity.m_index];
		for (auto& child : children)
		{
			Remove(child);
		}
		m_scene->m_enable[entity.m_index] = false;

		RED_LILIUM_NOT_IMPLEMENTED();

		/*
		Entity parent = m_scene->m_parents[entity.m_index];
		m_scene->m_children[parent.m_index].erase(
			std::remove(
				m_scene->m_children[parent.m_index].begin(),
				m_scene->m_children[parent.m_index].end(),
				entity),
			m_scene->m_children[parent.m_index].end()
		);
		*/
	}
	else
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
}

void Transaction::ChangeParent(Entity entity, Entity newParent)
{
	if (m_flags.Test(TransactionOptions::Immediate))
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
	else
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
}

Entity Transaction::AddChild(Entity entity, const std::string& name, u64 position)
{
	if (m_flags.Test(TransactionOptions::Immediate))
	{
		m_scene->m_components.push_back(std::vector<uptr<Component>>());
		m_scene->m_children.push_back(std::vector<Entity>());
		m_scene->m_parents.push_back(Entity());
		m_scene->m_names.push_back(name);
		m_scene->m_enable.push_back(true);

		Entity entity;
		entity.m_cluster = 0;
		entity.m_index = m_scene->m_enable.size() - 1;
		entity.m_generation = 0;
		return entity;
	}
	else
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
	return Entity();
}

void Transaction::SetName(Entity entity, const std::string& name)
{
	RED_LILIUM_ASSERT(entity.IsValid());
	RED_LILIUM_ASSERT(entity.m_index < m_scene->m_names.size());

	if (m_flags.Test(TransactionOptions::Immediate))
	{
		m_scene->m_names[entity.m_index] = name;
	}
	else
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
}

void Transaction::Apply()
{
	if (m_flags.Test(TransactionOptions::Immediate))
	{
		return;
	}
}
