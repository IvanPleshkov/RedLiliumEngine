#include "pch.h"
#include "NodesEditor.h"

using namespace RED_LILIUM_NAMESPACE;

NodesEditor::NodesEditor(ptr<TypesManager> typesManager)
	: Editor(typesManager)
{
	m_scene = umake<Scene>();
}

ptr<const Scene> NodesEditor::GetScene() const
{
	return m_scene.get();
}

ptr<Scene> NodesEditor::GetEditableScene()
{
	return m_scene.get();
}

void NodesEditor::NewScene()
{
	m_scene = umake<Scene>();
	ptr<Entity> root = m_scene->GetRoot();
	ptr<Entity> nodesRoot = root->AddChild("Nodes");
	nodesRoot->AddComponent<NodesSelectionComponent>();
	nodesRoot->AddComponent<NodesViewComponent>();

	ptr<Entity> nodeEntity = nodesRoot->AddChild("Node");
	ptr<NodeComponent> nodeComponent = nodeEntity->AddComponent<NodeComponent>();
	nodeComponent->SetPosition({ 0.0f, 0.0f });
	nodeComponent->SetSize({ 100.0f, 100.0f });
}
