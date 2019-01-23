#pragma once

#include <Core/Common.h>
#include <Reflection/Serialization.h>
#include "Action.h"

namespace RED_LILIUM_NAMESPACE
{

class ActionBase;
class Editor;

class Event : public Serializable
{
public:
	RED_LILIUM_CLASS(Event, Serializable);

	~Event() override = default;

	void Serialize(ptr<Serializator> serializator) const override { }
	void Deserialize(ptr<const Serializator> serializator) override { }

	virtual uptr<ActionBase> TriggerAction(ptr<Editor> editor)
	{
		return nullptr;
	}
};


// Класс для описания события создания Action
template<class IAction>
class ActionEvent : public Event
{
public:
	~ActionEvent() override = default;

	uptr<ActionBase> TriggerAction(ptr<Editor> editor) override
	{
		return umake<IAction>(editor);
	}
};

} // namespace RED_LILIUM_NAMESPACE
