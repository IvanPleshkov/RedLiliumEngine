#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>
#include <Reflection/Serialization.h>
#include "Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class Component;

enum class TransactionOptions : u8
{
	Immediate			= 1 << 0,
	RollbackFeature		= 1 << 1,
};
using TransactionFlags = Flags<TransactionOptions>;

class Scene final
{
public:
	Scene();
	~Scene();

	Entity GetRoot() const;

	template<class T>
	void RegisterComponent();

	const std::string& GetName(Entity entity) const;
	Entity GetParent(Entity entity) const;
	u32 GetChildrenCount(Entity entity) const;
	Entity GetChild(Entity entity, u32 index) const;
	Entity GetChild(Entity entity, std::string_view name, u32 index = 0) const;

	template<class T>
	ptr<const T> GetComponent(Entity entity) const;

	sptr<Transaction> CreateTransaction(TransactionFlags flags = TransactionFlags(TransactionOptions::Immediate));
	void ApplyTransactions();

	friend class Transaction;
private:
	Entity m_root;
	std::mutex m_transactionMutex;
	std::vector<sptr<Transaction>> m_transactions;

	std::unordered_map<std::type_index, u32> m_componentsIndex;
	std::vector<std::vector<uptr<Component>>> m_components;
	std::vector<std::vector<Entity>> m_children;
	std::vector<Entity> m_parents;
	std::vector<std::string> m_names;
	std::vector<bool> m_enable;
};

class Transaction final
{
public:
	Transaction(ptr<Scene> scene, TransactionFlags flags);
	~Transaction();

	void Remove(Entity entity);
	void ChangeParent(Entity entity, Entity newParent);
	Entity AddChild(Entity entity, const std::string& name, u64 position = u64_max);
	void SetName(Entity entity, const std::string& name);

	template<class T>
	void SetComponent(Entity entity, ptr<const T> component) const;

	void Apply();

private:
	ptr<Scene> m_scene;
	TransactionFlags m_flags;
};

template<class T>
inline void Scene::RegisterComponent()
{
	m_componentsIndex.insert({ std::type_index(typeid(T)), m_componentsIndex.size() });

	for (auto& components = m_components)
	{
		components.push_back(nullptr);
	}
}

template<class T>
inline ptr<const T> Scene::GetComponent(Entity entity) const
{
	RED_LILIUM_ASSERT(m_componentsIndex.find(std::type_index(typeid(T))) != m_componentsIndex.end());

	if (!entity.IsValid() || entity.m_index >= m_components.size())
	{
		return nullptr;
	}

	const auto& components = m_components[entity.m_index];
	const auto componentIndex = m_componentsIndex.find(std::type_index(typeid(T)));

	return Cast<const T>(components[componentIndex]);
}

template<class T>
inline void Transaction::SetComponent(Entity entity, ptr<const T> component) const
{
	RED_LILIUM_ASSERT(m_scene->m_componentsIndex.find(std::type_index(typeid(T))) != m_scene->m_componentsIndex.end());

	if (m_flags.Test(TransactionOptions::Immediate))
	{
		const auto& components = m_scene->m_components[entity.m_index];
		const auto componentIndex = m_scene->m_componentsIndex.find(std::type_index(typeid(T)));
		if (!components[componentIndex])
		{
			components[componentIndex] = umake<T>();
		}
		*components[componentIndex] = *component;
	}
	else
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}
}

} // namespace RED_LILIUM_NAMESPACE
