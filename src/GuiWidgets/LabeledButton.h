#pragma once

#include <core/common.h>
#include <GuiFramework/Widgets/ClickableWidget.h>
#include <GuiFramework/Widgets/Label.h>

namespace RED_LILIUM_NAMESPACE
{

class LabeledButton : public ClickableWidget
{
public:
	LabeledButton() {}
	~LabeledButton() override {}

	ptr<LabeledButton> SetText(const std::string& text);

protected:
	void CreateFirstChilds() override;
	void Tick() override;
	void UpdateDesiredSize() override;
	void Draw() override;

private:
	std::string m_text = "OK";
	ptr<Label> m_label = nullptr;
};

} // namespace RED_LILIUM_NAMESPACE
