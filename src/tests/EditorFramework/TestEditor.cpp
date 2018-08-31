#include "pch.h"
#include "TestEditor.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::TestEditor;

InsertTextAction::InsertTextAction(ptr<Editor> editor)
	: Action<TextEditor>(editor)
{
}

EventHandleResultFlags InsertTextAction::HandleEvent(ptr<Event> event)
{
    throw std::exception();
}

bool InsertTextAction::Undo() const
{
    throw std::exception();
}

bool InsertTextAction::Redo() const
{
    throw std::exception();
}

bool InsertTextAction::IsEmpty() const
{
    throw std::exception();
}

bool InsertTextAction::NeedPushToUndoStack() const
{
    throw std::exception();
}

bool InsertTextAction::NeedSetModifiedFlag() const
{
    throw std::exception();
}

bool InsertTextAction::NeedUndoAfterFinishing() const
{
	
    throw std::exception();
}

//======================== ChangeCursorPositionAction
DeleteTextAction::DeleteTextAction(ptr<Editor> editor)
	: Action<TextEditor>(editor)
{}

EventHandleResultFlags DeleteTextAction::HandleEvent(ptr<Event> event)
{
    throw std::exception();
}

bool DeleteTextAction::Undo() const
{
    throw std::exception();
}

bool DeleteTextAction::Redo() const
{
    throw std::exception();
}

bool DeleteTextAction::IsEmpty() const
{
    throw std::exception();
}

bool DeleteTextAction::NeedPushToUndoStack() const
{
    throw std::exception();
}

bool DeleteTextAction::NeedSetModifiedFlag() const
{
    throw std::exception();
}

bool DeleteTextAction::NeedUndoAfterFinishing() const
{
    throw std::exception();
}

//======================== ChangeCursorPositionAction
ChangeCursorPositionAction::ChangeCursorPositionAction(ptr<Editor> editor)
	: Action<TextEditor>(editor)
{
}

EventHandleResultFlags ChangeCursorPositionAction::HandleEvent(ptr<Event> event)
{
    throw std::exception();
}

bool ChangeCursorPositionAction::Undo() const
{
    throw std::exception();
}

bool ChangeCursorPositionAction::Redo() const
{
    throw std::exception();
}

bool ChangeCursorPositionAction::IsEmpty() const
{
    throw std::exception();
}

bool ChangeCursorPositionAction::NeedPushToUndoStack() const
{
    throw std::exception();
}

bool ChangeCursorPositionAction::NeedSetModifiedFlag() const
{
    throw std::exception();
}

bool ChangeCursorPositionAction::NeedUndoAfterFinishing() const
{
    throw std::exception();
}

//======================== ChangeSelectionAction
ChangeSelectionAction::ChangeSelectionAction(ptr<Editor> editor)
	: Action<TextEditor>(editor)
{}

EventHandleResultFlags ChangeSelectionAction::HandleEvent(ptr<Event> event)
{
    throw std::exception();
}

bool ChangeSelectionAction::Undo() const
{
    throw std::exception();
}

bool ChangeSelectionAction::Redo() const
{
    throw std::exception();
}

bool ChangeSelectionAction::IsEmpty() const
{
    throw std::exception();
}

bool ChangeSelectionAction::NeedPushToUndoStack() const
{
    throw std::exception();
}

bool ChangeSelectionAction::NeedSetModifiedFlag() const
{
    throw std::exception();
}

bool ChangeSelectionAction::NeedUndoAfterFinishing() const
{
    throw std::exception();
}

//======================== Events
uptr<ActionBase> SetCursorPositionEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<ChangeCursorPositionAction>(editor);
}

uptr<ActionBase> SetSelectionCursorEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<ChangeSelectionAction>(editor);
}

uptr<ActionBase> InputSymbolEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<InsertTextAction>(editor);
}

uptr<ActionBase> PasteTextEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<InsertTextAction>(editor);
}

uptr<ActionBase> DeleteEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<DeleteTextAction>(editor);
}

uptr<ActionBase> BackspaceEvent::TriggerAction(ptr<Editor> editor)
{
	return umake<DeleteTextAction>(editor);
}
