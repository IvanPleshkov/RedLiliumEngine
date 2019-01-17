#pragma once

#include <Core/Common.h>
#include "Serialization.h"
#include "Type.h"

namespace RED_LILIUM_NAMESPACE
{

class ReflectedVector : public Serializable
{
public:
	RED_LILIUM_CLASS(ReflectedVector, Serializable);

private:
	Type m_type;
	std::vector<uptr<Serializable>> m_data;
};

} // namespace RED_LILIUM_NAMESPACE
