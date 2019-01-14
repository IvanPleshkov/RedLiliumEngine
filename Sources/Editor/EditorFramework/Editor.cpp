#include "pch.h"
#include "Editor.h"

using namespace RED_LILIUM_NAMESPACE;

Editor::Editor()
	: m_isModified(false)
	, m_currentAction(nullptr)
{
}

void Editor::HandleEvent(ptr<Event> event)
{
	bool eventHandled = false;

	while (!eventHandled)
	{
		if (m_currentAction == nullptr)
		{
			m_currentAction = std::move(event->TriggerAction(this));
			if (m_currentAction == nullptr)
			{
				eventHandled = true;
				break;
			}
		}

		EventHandleResultFlags handleResult = m_currentAction->HandleEvent(event);
		if (handleResult.Test(EventHandleResult::ActionFinished))
		{
			UpdateModifiedFlag();

			if (m_currentAction->NeedPushToUndoStack() && !m_currentAction->IsEmpty())
			{
				MoveCurrentActionToUndoStack();
			}

			m_currentAction = nullptr;
		}

		eventHandled = handleResult.Test(EventHandleResult::EventHandled);
	}
}

void Editor::Undo()
{
}

void Editor::Redo()
{
}

bool Editor::IsModified() const
{
	return m_isModified;
}

void Editor::ResetModifiedStatus()
{
	m_isModified = false;

}

void Editor::MoveCurrentActionToUndoStack()
{
	m_undoStack.erase(m_undoStackPosition, m_undoStack.end());
	m_undoStack.push_back(std::move(m_currentAction));

	m_undoStackPosition = std::prev(m_undoStack.end());
	m_currentAction = nullptr;
}

void Editor::UpdateModifiedFlag()
{
	if (m_currentAction->IsEmpty() || !m_currentAction->NeedSetModifiedFlag())
	{
		return;
	}

	m_isModified = true;
}
