#include "pch.h"
#include "Type.h"

using namespace RED_LILIUM_NAMESPACE;

Type::Type(const std::string& name, std::function<uptr<Serializable>()> constructor)
	: RedLiliumObject()
	, m_name(name)
	, m_constructor(constructor)
{
}

uptr<Serializable> Type::Construct() const
{
	return m_constructor();
}

const std::string& Type::GetName() const
{
	return m_name;
}
