#include "pch.h"
#include "NodesEditorSystem.h"
#include <Nodes/Components/NodeComponent.h>
#include <Nodes/Components/NodeInputSlotComponent.h>
#include <Nodes/Components/NodeOutputSlotComponent.h>

#include <NodesEditor/Events/MouseKeyDown.h>
#include <NodesEditor/Events/MouseKeyUp.h>
#include <NodesEditor/Events/MouseMove.h>
#include <NodesEditor/Events/MouseScroll.h>

using namespace RED_LILIUM_NAMESPACE;

NodesEditorSystem::NodesEditorSystem()
	: RedLiliumObject()
{
	m_typesManager = umake<TypesManager>();
	InitTypes();

	m_editor = NewEditor();
}

ptr<NodesEditor> NodesEditorSystem::GetCurrentEditor()
{
	return m_editor.get();
}

ptr<const NodesEditor> NodesEditorSystem::GetCurrentEditor() const
{
	return m_editor.get();
}

void NodesEditorSystem::InitTypes()
{
	m_typesManager->RegisterType<MouseKeyDown>("MouseKeyDown");
	m_typesManager->RegisterType<MouseKeyUp>("MouseKeyUp");
	m_typesManager->RegisterType<MouseMove>("MouseMove");
	m_typesManager->RegisterType<MouseScroll>("MouseScroll");

	m_typesManager->RegisterType<NodeComponent>("NodeComponent");
	m_typesManager->RegisterType<NodeInputSlotComponent>("NodeInputSlotComponent");
	m_typesManager->RegisterType<NodeOutputSlotComponent>("NodeOutputSlotComponent");
	m_typesManager->RegisterType<NodesViewComponent>("NodesViewComponent");
	m_typesManager->RegisterType<NodesSelectionComponent>("NodesSelectionComponent");
}

uptr<NodesEditor> NodesEditorSystem::NewEditor()
{
	uptr<NodesEditor> editor = umake<NodesEditor>(m_typesManager.get());
	editor->NewScene();
	return std::move(editor);
}
