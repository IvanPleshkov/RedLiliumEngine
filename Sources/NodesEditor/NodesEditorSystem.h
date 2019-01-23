#pragma once

#include <Scene/Component.h>
#include <Editor/Editor.h>
#include <Reflection/TypesManager.h>
#include "NodesEditor.h"

namespace RED_LILIUM_NAMESPACE
{

class NodesEditorSystem : public RedLiliumObject
{
public:
	RED_LILIUM_CLASS(NodesEditorSystem, RedLiliumObject);

	NodesEditorSystem();
	~NodesEditorSystem() override = default;

	ptr<NodesEditor> GetCurrentEditor();
	ptr<const NodesEditor> GetCurrentEditor() const;

	uptr<NodesEditor> NewEditor();

private:
	void InitTypes();

	uptr<NodesEditor> m_editor;
	uptr<TypesManager> m_typesManager;
};

} // namespace RED_LILIUM_NAMESPACE
