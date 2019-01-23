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
	ptr<const Type> GetType(const std::string& name) const;
	ptr<const Type> GetType(std::type_index typeIndex) const;
	ptr<const Type> GetType(ptr<const Serializable> obj) const;
	template<class T> ptr<const Type> GetType() const;

private:
	std::map<std::string, uptr<Type>> m_typesByName;
	std::unordered_map<std::type_index, ptr<Type>> m_typesById;
};

template<class T>
inline void TypesManager::RegisterType(const std::string& name)
{
	std::function<uptr<Serializable>()> constructor = []()
	{
		return umake<T>();
	};

	auto id = std::type_index(typeid(T));
	uptr<Type> t = umake<Type>(name, constructor);
	m_typesById.insert({ id, t.get() });
	m_typesByName.insert({ name, std::move(t) });
}

template<class T>
inline ptr<const Type> TypesManager::GetType() const
{
	auto typeIndex = std::type_index(typeid(T));
	return GetType(typeIndex);
}

} // namespace RED_LILIUM_NAMESPACE
