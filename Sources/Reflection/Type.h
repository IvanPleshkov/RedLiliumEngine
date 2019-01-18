#pragma once

#include <Core/Common.h>
#include "Serialization.h"

namespace RED_LILIUM_NAMESPACE
{

class Type : public RedLiliumObject
{
public:
	template<class T> Type();
	~Type() override = default;

	uptr<Serializable> Construct() const;
	const std::vector<std::string>& GetRequiredPackages() const;

private:
	std::function<uptr<Serializable>()> m_constructor;
	std::string m_name;
};

template<class T>
inline Type::Type()
	: RedLiliumObject()
{
	m_constructor = []()
	{
		return umake<T>();
	};
}

} // namespace RED_LILIUM_NAMESPACE
