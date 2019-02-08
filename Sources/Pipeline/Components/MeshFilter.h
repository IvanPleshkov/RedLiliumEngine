#pragma once

#include <Core/Common.h>
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class MeshFilter : public Component
{
public:
	RED_LILIUM_CLASS(MeshFilter, Component);

	MeshFilter();
	MeshFilter(ptr<Entity> parent);
	~MeshFilter() override = default;

private:
};

} // namespace RED_LILIUM_NAMESPACE
