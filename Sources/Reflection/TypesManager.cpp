#include "pch.h"
#include "TypesManager.h"

using namespace RED_LILIUM_NAMESPACE;

TypesManager::TypesManager()
{
}

ptr<const Type> TypesManager::GetType(const std::string& name) const
{
	const auto& result = m_typesByName.find(name);
	if (result == m_typesByName.end())
	{
		return nullptr;
	}

	return result->second.get();
}

ptr<const Type> TypesManager::GetType(std::type_index typeIndex) const
{
	const auto result = m_typesById.find(typeIndex);
	if (result == m_typesById.end())
	{
		return nullptr;
	}

	return result->second;
}

ptr<const Type> TypesManager::GetType(ptr<const Serializable> obj) const
{
	const Serializable* o = obj;
	const Serializable& oRef = *o;
	auto typeIndex = std::type_index(typeid(oRef));
	return GetType(typeIndex);
}
