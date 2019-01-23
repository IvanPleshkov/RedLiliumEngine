#pragma once

#include <Scene/Component.h>
#include <Editor/Editor.h>
#include <Scene/Scene.h>

#include <Nodes/Components/NodeComponent.h>
#include <Nodes/Components/NodeInputSlotComponent.h>
#include <Nodes/Components/NodeOutputSlotComponent.h>
#include <Nodes/Components/NodesViewComponent.h>
#include <Nodes/Components/NodesSelectionComponent.h>

namespace RED_LILIUM_NAMESPACE
{

class NodesEditor : public Editor
{
public:
	RED_LILIUM_CLASS(NodesEditor, Editor);

	NodesEditor(ptr<TypesManager> typesManager);
	~NodesEditor() override = default;

	ptr<const Scene> GetScene() const;
	ptr<Scene> GetEditableScene();
	void NewScene();
		
private:
	uptr<Scene> m_scene;
};

} // namespace RED_LILIUM_NAMESPACE
