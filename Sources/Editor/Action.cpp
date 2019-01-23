#include "pch.h"
#include "Action.h"
#include "Event.h"
#include "Editor.h"

using namespace RED_LILIUM_NAMESPACE;

EventHandleResultFlags ActionBase::HandleEvent(ptr<Event> event)
{
	EventHandleResultFlags result;
	result.Add(EventHandleResult::ActionFinished);
	return result;
}

bool ActionBase::Undo() const
{
	return true;
}

bool ActionBase::Redo() const
{
	return true;
}

bool ActionBase::IsEmpty() const
{
	return false;
}

bool ActionBase::NeedPushToUndoStack() const
{
	return true;
}

bool ActionBase::NeedSetModifiedFlag() const
{
	return true;
}

bool ActionBase::NeedUndoAfterFinishing() const
{
	return false;
}

const std::string& ActionBase::GetName() const
{
	return m_name;
}
