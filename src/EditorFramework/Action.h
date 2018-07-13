#pragma once

#include "core/common.h"

namespace RED_LILIUM_NAMESPACE
{

class Event;
class Editor;

enum class EventHandleResult : u32
{
	ActionFinished	= 1 << 0,
	EventHandled	= 1 << 1
};
using EventHandleResultFlags = Flags<EventHandleResult>;


class ActionBase : public RedLiliumObject
{
public:
	virtual EventHandleResultFlags HandleEvent(const ptr<Event>& event);
	virtual bool Undo() const;
	virtual bool Redo() const;
	virtual bool IsEmpty() const;
	virtual bool NeedPushToUndoStack() const;
	virtual bool NeedSetModifiedFlag() const;
	virtual bool NeedUndoAfterFinishing() const;
	const std::string& GetName() const;

protected:
	std::string m_name;
};


template<class IEditor>
class Action : public ActionBase
{
public:
	Action(const ptr<Editor>& editor)
		: m_editor(Cast<IEditor>(editor))
	{}

protected:
	ptr<IEditor> m_editor;
};

}  // namespace RED_LILIUM_NAMESPACE
