#include "pch.h"

#include "GuiApplication.h"
#include <GuiWidgets/LabeledButton.h>
#include <GuiFramework/Widgets/StackLayout.h>
#include <GuiFramework/Widgets/Label.h>
#include <GuiFramework/Widgets/ColoredRectangle.h>
#include <GuiFramework/Widgets/ClickableWidget.h>

using namespace RED_LILIUM_NAMESPACE;

namespace
{

class ExampleHelloWorld : public GuiApplication
{
public:
	ExampleHelloWorld(const char* _name, const char* _description)
		: GuiApplication(_name, _description)
		, m_mainPanel(nullptr)
	{
		m_guiManager = std::make_unique<GuiManager>();
		m_mainPanel = m_guiManager->AddPanel();
	}

	void init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height) override
	{
		GuiApplication::init(argc, argv, width, height);
		LoadFonts();
	}

	void example2(const ptr<Widget>& top)
	{
		ptr<StackLayout> stackLayout = top
			->Add<StackLayout>()
			->SetPadding({ 10, 10 })
			->SetHorizontalAlignment(HorizontalAlignment::Fill);

		ptr<ClickableWidget> r1 = stackLayout->Add<ClickableWidget>();
		ptr<ClickableWidget> r2 = stackLayout->Add<ClickableWidget>();
		ptr<LabeledButton> button = stackLayout->Add<LabeledButton>()->SetText("Cancel");
		ptr<ClickableWidget> r3 = stackLayout->Add<ClickableWidget>();
		ptr<ClickableWidget> r4 = stackLayout->Add<ClickableWidget>();
	}

	void example3(const ptr<Widget>& top)
	{
		ptr<StackLayout> stackLayout = top
			->Add<StackLayout>()
			->SetPadding({ 10, 10 })
			->SetHorizontalAlignment(HorizontalAlignment::Fill);

		ptr<ColoredRectangle> r1 = stackLayout->Add<ColoredRectangle>()->SetColor({ 1, 0, 0, 0.3f });
		ptr<ColoredRectangle> r2 = stackLayout->Add<ColoredRectangle>()->SetColor({ 0, 1, 0, 0.3f });
		ptr<LabeledButton> button = stackLayout->Add<LabeledButton>()->SetText("Cancel");
		ptr<ColoredRectangle> r3 = stackLayout->Add<ColoredRectangle>()->SetColor({ 0, 0, 1, 0.3f });
		ptr<ColoredRectangle> r4 = stackLayout->Add<ColoredRectangle>()->SetColor({ 1, 0, 1, 0.3f });

		if (button->IsClicked())
		{
			std::cout << "Clicked!" << std::endl;
		}
	}

	void draw() override
	{
		UpdateInputState();

		ptr<Widget> top = m_mainPanel->BeginFrame();

		example3(top);

		m_mainPanel->EndFrame(m_nvg, { 0, 0 }, { m_width, m_height });
		m_mainPanel->Draw();

		vec2 mousePosition = { m_mouseState.m_mx, m_mouseState.m_my };

	}

	void UpdateInputState()
	{
		KeyState keyState;

		MouseState mouseState;
		mouseState.mousePosition = { m_mouseState.m_mx, m_mouseState.m_my };
		if (m_mouseState.m_buttons[entry::MouseButton::Left])
		{
			mouseState.pressedKeys.Add(MouseKey::Left);
		}
		if (m_mouseState.m_buttons[entry::MouseButton::Right])
		{
			mouseState.pressedKeys.Add(MouseKey::Right);
		}
		if (m_mouseState.m_buttons[entry::MouseButton::Middle])
		{
			mouseState.pressedKeys.Add(MouseKey::Middle);
		}

		m_guiManager->UpdateFrame(0, mouseState, keyState);
	}

private:
	void LoadFonts()
	{
		std::string robotoRegularFile = m_dataPath + "/fonts/roboto-regular.ttf";
		m_sansFontFamily = nvgCreateFont(m_nvg, "sans", robotoRegularFile.c_str());

		std::string robotoBoldFile = m_dataPath + "/fonts/roboto-bold.ttf";
		m_sansFontBoldFamily = nvgCreateFont(m_nvg, "sans-bold", robotoBoldFile.c_str());

		ptr<Style> style = m_guiManager->GetStyle();
		style->m_regular.color = { 1, 1, 1, 1 };
		style->m_regular.fontSize = 12;
		style->m_regular.fontFamily = m_sansFontFamily;

		style->m_h1.color = { 1, 1, 1, 1 };
		style->m_h1.fontSize = 12;
		style->m_h1.fontFamily = m_sansFontFamily;

		style->m_h2.color = { 1, 1, 1, 1 };
		style->m_h2.fontSize = 12;
		style->m_h2.fontFamily = m_sansFontFamily;

		style->m_caption.color = { 1, 1, 1, 1 };
		style->m_caption.fontSize = 12;
		style->m_caption.fontFamily = m_sansFontFamily;
	}

	FontFamily m_sansFontFamily;
	FontFamily m_sansFontBoldFamily;
	ptr<Panel> m_mainPanel;
	uptr<GuiManager> m_guiManager;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(ExampleHelloWorld, "00-helloworld", "Initialization and debug text.");
