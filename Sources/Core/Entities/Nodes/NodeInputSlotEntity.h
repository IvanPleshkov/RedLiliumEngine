#pragma once

#include "Scene/Entity.h"
namespace RED_LILIUM_NAMESPACE
{

class NodeInputSlotEntity : public Entity
{
public:
	NodeInputSlotEntity(ptr<Entity> parent);
	~NodeInputSlotEntity() override = default;

private:
};

}  // namespace RED_LILIUM_NAMESPACE
