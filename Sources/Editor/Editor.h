#pragma once

#include <Core/Common.h>
#include <Reflection/TypesManager.h>
#include "Action.h"
#include "Event.h"

namespace RED_LILIUM_NAMESPACE
{

class Editor : public RedLiliumObject
{
public:
	Editor(ptr<TypesManager> typesManager);

	void HandleEvent(ptr<Event> event);
	void Undo();
	void Redo();
	bool IsModified() const;
	void ResetModifiedStatus();

private:
	void MoveCurrentActionToUndoStack();
	void UpdateModifiedFlag();

	ptr<TypesManager> m_typesManager;
	uptr<ActionBase> m_currentAction;
	std::list<uptr<ActionBase>> m_undoStack;
	std::list<uptr<ActionBase>>::iterator m_undoStackPosition;
	bool m_isModified;
	std::unordered_set<ptr<ActionBase>> m_prevModificationActions;
};

} // namespace RED_LILIUM_NAMESPACE
