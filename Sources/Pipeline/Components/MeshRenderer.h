#pragma once

#include <Core/Common.h>
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class MeshRenderer : public Component
{
public:
	RED_LILIUM_CLASS(MeshRenderer, Component);

	MeshRenderer();
	MeshRenderer(ptr<Entity> parent);
	~MeshRenderer() override = default;

private:
};

} // namespace RED_LILIUM_NAMESPACE
