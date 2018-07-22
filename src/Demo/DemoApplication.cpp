#include "pch.h"

#include <GuiBgfxAdapter/BgfxGuiApplication.h>
#include <GuiBgfxAdapter/BgfxWindow.h>
#include <GuiBgfxAdapter/BgfxEnvironment.h>
#include <GuiFramework/GuiFramework.h>
#include <GuiWidgets/LabeledButton.h>
#include <GuiWidgets/TextBox.h>

using namespace RED_LILIUM_NAMESPACE;

namespace
{

class DemoGuiApplication : public BgfxGuiApplication
{
public:
	DemoGuiApplication();
	~DemoGuiApplication() override {}

	void OnBeginApplication() override;
	void OnEndApplication() override;
	void Update() override { Demo1(); }

private:
	void LoadFonts(NVGcontextPtr nvg);

	void Demo1();
	void Demo2();

	ptr<Panel> m_mainPanel;
	int m_sansFontFamily;
	int m_sansFontBoldFamily;
};

} // namespace

DemoGuiApplication::DemoGuiApplication()
	: BgfxGuiApplication("Demo", "", {}, {500, 500})
	, m_mainPanel(nullptr)
{}

void DemoGuiApplication::OnBeginApplication()
{
	NVGcontextPtr nvg = GetNativeWindow()->GetNvgContext();
	LoadFonts(nvg);

	m_mainPanel = GetGuiManager()->AddPanel();
}

void DemoGuiApplication::OnEndApplication()
{
}

void DemoGuiApplication::Demo1()
{
	m_mainPanel->SetWindow(GetNativeWindow());
	m_mainPanel->SetPosition({ 0, 0 });
	m_mainPanel->SetSize( GetNativeWindow()->GetSize() );

	ptr<Widget> topWidget = m_mainPanel->GetTopWidget();

	ptr<StackLayout> stackLayout = topWidget
		->Add<StackLayout>()
		->SetPadding({ 10, 10 })
		->SetHorizontalAlignment(HorizontalAlignment::Left);
	
	ptr<ColoredRectangle> r1 = stackLayout
		->Add<ColoredRectangle>()
		->SetColor({ 1, 0, 0, 0.3f });

	ptr<ColoredRectangle> r2 = stackLayout
		->Add<ColoredRectangle>()
		->SetColor({ 0, 1, 0, 0.3f });

	ptr<LabeledButton> button = stackLayout
		->Add<LabeledButton>()
		->SetText("Cancel");

	ptr<TextBox> textBox = stackLayout
		->Add<TextBox>()
		->SetText(u8"ptr<Widget> topWidget = m_mainPanel->GetTopWidget();опаопаровкынпруцп");

	ptr<ColoredRectangle> r3 = stackLayout
		->Add<ColoredRectangle>()
		->SetColor({ 0, 0, 1, 0.3f });

	ptr<ColoredRectangle> r4 = stackLayout
		->Add<ColoredRectangle>()
		->SetColor({ 1, 0, 1, 0.3f });
	
	if (button->IsClicked())
	{
		std::cout << "Clicked!" << std::endl;
	}

	if (button->IsDoubleClicked())
	{
		std::cout << "Double Clicked!" << std::endl;
	}

	if (button->IsTripleClicked())
	{
		std::cout << "Triple Clicked!" << std::endl;
	}
}

void DemoGuiApplication::Demo2()
{

}

void DemoGuiApplication::LoadFonts(NVGcontextPtr nvg)
{
	std::string robotoRegularFile = GetDataPath() + "/fonts/roboto-regular.ttf";
	m_sansFontFamily = nvgCreateFont(nvg, "sans", robotoRegularFile.c_str());

	std::string robotoBoldFile = GetDataPath() + "/fonts/roboto-bold.ttf";
	m_sansFontBoldFamily = nvgCreateFont(nvg, "sans-bold", robotoBoldFile.c_str());

	ptr<Style> style = GetGuiManager()->GetStyle();
	style->m_regular.color = { 1, 1, 1, 1 };
	style->m_regular.fontSize = 25;
	style->m_regular.fontFamily = m_sansFontFamily;

	style->m_h1.color = { 1, 1, 1, 1 };
	style->m_h1.fontSize = 16;
	style->m_h1.fontFamily = m_sansFontFamily;

	style->m_h2.color = { 1, 1, 1, 1 };
	style->m_h2.fontSize = 16;
	style->m_h2.fontFamily = m_sansFontFamily;

	style->m_caption.color = { 1, 1, 1, 1 };
	style->m_caption.fontSize = 16;
	style->m_caption.fontFamily = m_sansFontFamily;
}

int _main_(int _argc, char** _argv)
{
	uptr<DemoGuiApplication> application = std::make_unique<DemoGuiApplication>();
	application->Run();
	return 0;
}
