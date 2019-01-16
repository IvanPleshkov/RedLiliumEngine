#pragma once

#include "Scene/Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class NodeSlotEntity : public Entity
{
public:
	NodeSlotEntity(ptr<Entity> parent);
	~NodeSlotEntity() override = default;

private:
};

}  // namespace RED_LILIUM_NAMESPACE
