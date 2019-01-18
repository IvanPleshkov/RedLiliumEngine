#include "pch.h"
#include "TypesManager.h"

using namespace RED_LILIUM_NAMESPACE;

TypesManager::TypesManager()
{
}

ptr<const Type> TypesManager::GetType(const std::string& name)
{
	auto result = m_types.find(name);
	if (result == m_types.end())
	{
		return nullptr;
	}

	return result->second.get();
}
