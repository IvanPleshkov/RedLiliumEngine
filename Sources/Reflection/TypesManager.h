#pragma once

#include <Core/Common.h>
#include "Type.h"

namespace RED_LILIUM_NAMESPACE
{

class TypesManager : public RedLiliumObject
{
public:
	TypesManager();
	~TypesManager() override = default;

	template<class T> void RegisterType(const std::string& name);
	ptr<const Type> GetType(const std::string& name);

private:
	std::map<std::string, uptr<Type>> m_types;
};

template<class T>
inline void TypesManager::RegisterType(const std::string& name)
{
	m_types.insert({ name, umake<Type>() });
}

} // namespace RED_LILIUM_NAMESPACE
