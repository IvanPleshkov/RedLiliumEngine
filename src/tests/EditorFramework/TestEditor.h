#pragma once

#include <Core/Common.h>
#include <EditorFramework/Editor.h>
#include <EditorFramework/Action.h>
#include <EditorFramework/Event.h>

namespace RED_LILIUM_NAMESPACE
{
namespace TestEditor
{

class TextEditor : public Editor
{
public:
	std::string m_text;
	u64 m_cursorPosition;
	u64 m_selectionPosition;
};

class InsertTextAction : public Action<TextEditor>
{
public:
	InsertTextAction(const ptr<Editor>& editor);
	EventHandleResultFlags HandleEvent(const ptr<Event>& event) override;
	bool Undo() const override;
	bool Redo() const override;
	bool IsEmpty() const override;
	bool NeedPushToUndoStack() const override;
	bool NeedSetModifiedFlag() const override;
	bool NeedUndoAfterFinishing() const override;

private:
	u64 m_startActionCursorPosition;
	u64 m_startActionSelectionPosition;
	std::string m_insertedText;
};

class DeleteTextAction : public Action<TextEditor>
{
public:
	DeleteTextAction(const ptr<Editor>& editor);
	EventHandleResultFlags HandleEvent(const ptr<Event>& event) override;
	bool Undo() const override;
	bool Redo() const override;
	bool IsEmpty() const override;
	bool NeedPushToUndoStack() const override;
	bool NeedSetModifiedFlag() const override;
	bool NeedUndoAfterFinishing() const override;

private:
	u64 m_startActionCursorPosition;
	u64 m_startActionSelectionStartCursorPosition;
	std::string m_deletedText;
};

class ChangeCursorPositionAction : public Action<TextEditor>
{
public:
	ChangeCursorPositionAction(const ptr<Editor>& editor);
	EventHandleResultFlags HandleEvent(const ptr<Event>& event) override;
	bool Undo() const override;
	bool Redo() const override;
	bool IsEmpty() const override;
	bool NeedPushToUndoStack() const override;
	bool NeedSetModifiedFlag() const override;
	bool NeedUndoAfterFinishing() const override;
};

class ChangeSelectionAction : public Action<TextEditor>
{
public:
	ChangeSelectionAction(const ptr<Editor>& editor);
	EventHandleResultFlags HandleEvent(const ptr<Event>& event) override;
	bool Undo() const override;
	bool Redo() const override;
	bool IsEmpty() const override;
	bool NeedPushToUndoStack() const override;
	bool NeedSetModifiedFlag() const override;
	bool NeedUndoAfterFinishing() const override;
};

class SetCursorPositionEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;

private:
	u64 m_cursorPosition;
};

class SetSelectionCursorEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;

private:
	u64 m_selectionPosition;
};

class InputSymbolEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;

private:
	std::string m_symbol;
};

class PasteTextEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;

private:
	std::string m_pastedText;
};

class DeleteEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;
};

class BackspaceEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override;
};

} // namespace TestEditor
} // namespace RED_LILIUM_NAMESPACE
