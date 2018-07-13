#include "pch.h"
#include "TestEditor.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::TestEditor;

InsertTextAction::InsertTextAction(const ptr<Editor>& editor)
	: Action<TextEditor>(editor)
{
}

EventHandleResultFlags InsertTextAction::HandleEvent(const ptr<Event>& event)
{
	throw std::exception("not implemented");
}

bool InsertTextAction::Undo() const
{
	throw std::exception("not implemented");
}

bool InsertTextAction::Redo() const
{
	throw std::exception("not implemented");
}

bool InsertTextAction::IsEmpty() const
{
	throw std::exception("not implemented");
}

bool InsertTextAction::NeedPushToUndoStack() const
{
	throw std::exception("not implemented");
}

bool InsertTextAction::NeedSetModifiedFlag() const
{
	throw std::exception("not implemented");
}

bool InsertTextAction::NeedUndoAfterFinishing() const
{
	
	throw std::exception("not implemented");
}

//======================== ChangeCursorPositionAction
DeleteTextAction::DeleteTextAction(const ptr<Editor>& editor)
	: Action<TextEditor>(editor)
{}

EventHandleResultFlags DeleteTextAction::HandleEvent(const ptr<Event>& event)
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::Undo() const
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::Redo() const
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::IsEmpty() const
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::NeedPushToUndoStack() const
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::NeedSetModifiedFlag() const
{
	throw std::exception("not implemented");
}

bool DeleteTextAction::NeedUndoAfterFinishing() const
{
	throw std::exception("not implemented");
}

//======================== ChangeCursorPositionAction
ChangeCursorPositionAction::ChangeCursorPositionAction(const ptr<Editor>& editor)
	: Action<TextEditor>(editor)
{
}

EventHandleResultFlags ChangeCursorPositionAction::HandleEvent(const ptr<Event>& event)
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::Undo() const
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::Redo() const
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::IsEmpty() const
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::NeedPushToUndoStack() const
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::NeedSetModifiedFlag() const
{
	throw std::exception("not implemented");
}

bool ChangeCursorPositionAction::NeedUndoAfterFinishing() const
{
	throw std::exception("not implemented");
}

//======================== ChangeSelectionAction
ChangeSelectionAction::ChangeSelectionAction(const ptr<Editor>& editor)
	: Action<TextEditor>(editor)
{}

EventHandleResultFlags ChangeSelectionAction::HandleEvent(const ptr<Event>& event)
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::Undo() const
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::Redo() const
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::IsEmpty() const
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::NeedPushToUndoStack() const
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::NeedSetModifiedFlag() const
{
	throw std::exception("not implemented");
}

bool ChangeSelectionAction::NeedUndoAfterFinishing() const
{
	throw std::exception("not implemented");
}

//======================== Events
uptr<ActionBase> SetCursorPositionEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<ChangeCursorPositionAction>(editor);
}

uptr<ActionBase> SetSelectionCursorEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<ChangeSelectionAction>(editor);
}

uptr<ActionBase> InputSymbolEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<InsertTextAction>(editor);
}

uptr<ActionBase> PasteTextEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<InsertTextAction>(editor);
}

uptr<ActionBase> DeleteEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<DeleteTextAction>(editor);
}

uptr<ActionBase> BackspaceEvent::TriggerAction(const ptr<Editor>& editor)
{
	return std::make_unique<DeleteTextAction>(editor);
}
