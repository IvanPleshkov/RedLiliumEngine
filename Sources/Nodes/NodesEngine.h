#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodesEngine : public RedLiliumObject
{
public:
	RED_LILIUM_CLASS(NodesEngine, RedLiliumObject);

	NodesEngine();
	~NodesEngine() override = default;

private:
};

} // namespace RED_LILIUM_NAMESPACE
