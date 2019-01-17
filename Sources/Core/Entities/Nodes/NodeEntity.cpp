#include "pch.h"
#include "NodeEntity.h"

using namespace RED_LILIUM_NAMESPACE;

NodeEntity::NodeEntity(ptr<Entity> parent)
	: Entity(parent)
	, m_editorScene(nullptr)
{
}
