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

} // namespace RED_LILIUM_NAMESPACE
