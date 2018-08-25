#pragma once

#include <Core/Common.h>
#include <GuiFramework/Widgets/TextField.h>

namespace RED_LILIUM_NAMESPACE
{

class TextBox : public Widget
{
public:
	TextBox();
	~TextBox() override {}

	ptr<TextBox> SetText(const std::string& text);

protected:
	void CreateFirstChilds() override;
	void Tick() override;
	void UpdateDesiredSize() override;
	void Draw() override;

private:
	std::string m_text;
	ptr<TextField> m_textField = nullptr;
};

} // namespace RED_LILIUM_NAMESPACE
