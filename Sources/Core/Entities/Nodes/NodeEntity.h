#pragma once

#include <Core/Scene/Entity.h>
#include <Core/Scene/Scene.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeEntity : public Entity
{
public:
	RED_LILIUM_CLASS(NodeEntity, Entity);

	NodeEntity(ptr<Entity> parent);
	~NodeEntity() override = default;


private:
	uptr<Scene> m_editorScene;
	bool m_isPinned;
	Color32 m_color;
};

}  // namespace RED_LILIUM_NAMESPACE
