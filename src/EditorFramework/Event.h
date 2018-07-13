﻿#pragma once

#include "core/common.h"
#include "Action.h"

namespace RED_LILIUM_NAMESPACE
{

class ActionBase;
class Editor;

class Event : public RedLiliumObject
{
public:
	virtual uptr<ActionBase> TriggerAction(const ptr<Editor>& editor)
	{
		return nullptr;
	}
};


// Класс для описания события создания Action
template<class IAction>
class ActionEvent : public Event
{
public:
	uptr<ActionBase> TriggerAction(const ptr<Editor>& editor) override
	{
		return std::make_unique<IAction>(editor);
	}
};

} // namespace RED_LILIUM_NAMESPACE
