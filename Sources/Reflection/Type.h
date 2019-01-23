#pragma once

#include <Core/Common.h>
#include "Serialization.h"

namespace RED_LILIUM_NAMESPACE
{

class Type : public RedLiliumObject
{
public:
	Type(const std::string& name, std::function<uptr<Serializable>()> constructor);
	~Type() override = default;

	uptr<Serializable> Construct() const;
	const std::string& GetName() const;
	const std::vector<std::string>& GetRequiredPackages() const;

private:
	std::function<uptr<Serializable>()> m_constructor;
	std::string m_name;
};

} // namespace RED_LILIUM_NAMESPACE
