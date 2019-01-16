#pragma once

#include "Scene/Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class NodeEntity : public Entity
{
public:
	NodeEntity(ptr<Entity> parent);
	~NodeEntity() override = default;

};

}  // namespace RED_LILIUM_NAMESPACE
