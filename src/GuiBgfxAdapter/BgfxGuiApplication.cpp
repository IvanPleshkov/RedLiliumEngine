#include "pch.h"
#include "BgfxGuiApplication.h"

// bgfx
#include <bx/uint32_t.h>
#include <common.h>
#include <entry/input.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include <nanovg/nanovg.h>

#include "BgfxWindow.h"
#include "BgfxEnvironment.h"
#include "BgfxKeysMapping.h"

using namespace RED_LILIUM_NAMESPACE;

class BgfxGuiApplication::Impl : public entry::AppI
{
public:
	Impl(ptr<BgfxGuiApplication> application, const char* _name, const char* _description)
		: entry::AppI(_name, _description)
		, m_application(application)
	{ }

	~Impl() override {}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;
	int shutdown() override;
	bool update() override;

	MouseState GetMouseState();
	KeyState GetKeyState();

protected:
	ptr<BgfxGuiApplication> m_application;

	NVGcontextPtr m_nvg;
	int64_t m_timeOffset;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	entry::MouseState m_bgfxMouseState;
};

//============================= GuiApplication

BgfxGuiApplication::BgfxGuiApplication(
	const std::string& name,
	const std::string& description,
	const std::vector<std::string>& args,
	vec2 windowSize)
	: m_args(args)
	, m_name(name)
	, m_description(description)
	, m_screenshotOutputFilename()
	, m_isStopped(false)
{
	InitDataPath();

	m_impl = std::make_unique<Impl>(this, m_name.c_str(), m_description.c_str());
	m_nativeWindow = std::make_unique<BgfxWindow>();
	m_nativeEnvironment = std::make_unique<BgfxEnvironment>();
}

BgfxGuiApplication::~BgfxGuiApplication()
{}

void BgfxGuiApplication::Run(GuiRecordingMode recordingMode)
{
	m_guiManager = std::make_unique<GuiManager>(m_nativeEnvironment.get(), recordingMode);

	std::vector<const char*> argv;
	argv.reserve(m_args.size());
	for (auto& arg : m_args)
	{
		argv.push_back(arg.c_str());
	}
	if (argv.size() == 0)
	{
		argv.push_back(nullptr);
	}
	entry::runApp(m_impl.get(), m_args.size(), &argv[0]);
}

void BgfxGuiApplication::Stop()
{
	m_isStopped = true;
}

void BgfxGuiApplication::TakeScreenshot(const std::string& outputFile)
{
	m_screenshotOutputFilename = outputFile;
	m_guiManager->LogMessage("[Screenshot]" + outputFile);
}

const std::string& BgfxGuiApplication::GetDataPath() const
{
	return m_dataPath;
}

ptr<GuiManager> BgfxGuiApplication::GetGuiManager()
{
	return m_guiManager.get();
}

ptr<const GuiManager> BgfxGuiApplication::GetGuiManager() const
{
	return m_guiManager.get();
}

ptr<BgfxWindow> BgfxGuiApplication::GetNativeWindow()
{
	return m_nativeWindow.get();
}

ptr<const BgfxWindow> BgfxGuiApplication::GetNativeWindow() const
{
	return m_nativeWindow.get();
}

ptr<BgfxEnvironment> BgfxGuiApplication::GetNativeEnvironment()
{
	return m_nativeEnvironment.get();
}

ptr<const BgfxEnvironment> BgfxGuiApplication::GetNativeEnvironment() const
{
	return m_nativeEnvironment.get();
}

void BgfxGuiApplication::InitDataPath()
{
	std::string line;
	std::ifstream pathsFile("paths.json");
	if (pathsFile.is_open())
	{
		while (std::getline(pathsFile, line))
		{
			std::cout << line << '\n';
		}
		pathsFile.close();
	}
	m_dataPath = line;
}

//============================= GuiApplication::Impl

void BgfxGuiApplication::Impl::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
{
	Args args(_argc, _argv);

	m_width = _width;
	m_height = _height;
	m_debug = BGFX_DEBUG_TEXT;
	m_reset = BGFX_RESET_VSYNC;

	bgfx::Init init;
	init.type = args.m_type;
	init.vendorId = args.m_pciId;
	init.resolution.width = m_width;
	init.resolution.height = m_height;
	init.resolution.reset = m_reset;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(m_debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x202020ff
		, 1.0f
		, 0
	);

	imguiCreate();

	m_nvg = nvgCreate(1, 0);
	bgfx::setViewMode(0, bgfx::ViewMode::Sequential);

	m_timeOffset = bx::getHPCounter();

	entry::WindowState windowState;
	windowState.m_width = m_width;
	windowState.m_height = m_height;
	m_application->GetNativeWindow()->UpdateWindowState(windowState, m_nvg);

	m_application->OnBeginApplication();
}

int BgfxGuiApplication::Impl::shutdown()
{
	m_application->OnEndApplication();

	nvgDelete(m_nvg);

	imguiDestroy();

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool BgfxGuiApplication::Impl::update()
{
	if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_bgfxMouseState))
	{
		const int64_t now = bx::getHPCounter();
		const double freq = double(bx::getHPFrequency());
		double time = static_cast<double>(now - m_timeOffset) / freq;

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		nvgBeginFrame(m_nvg, m_width, m_height, 1.0f);

		{
			ptr<GuiManager> guiManager = m_application->GetGuiManager();
			guiManager->BeginFrame(
				time,
				GetMouseState(),
				GetKeyState());

			m_application->Update();

			guiManager->EndFrame();
			guiManager->Draw();
		}

		nvgEndFrame(m_nvg);
		
		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		if (!m_application->m_screenshotOutputFilename.empty())
		{
			std::string fullFilename = m_application->GetDataPath() + m_application->m_screenshotOutputFilename;
			bgfx::requestScreenShot(BGFX_INVALID_HANDLE, fullFilename.c_str());
			m_application->m_screenshotOutputFilename.clear();
		}

		return !m_application->m_isStopped;
	}

	return false;
}

MouseState BgfxGuiApplication::Impl::GetMouseState()
{
	MouseState mouseState;
	mouseState.mousePosition = { m_bgfxMouseState.m_mx, m_bgfxMouseState.m_my };
	if (m_bgfxMouseState.m_buttons[entry::MouseButton::Left])
	{
		mouseState.pressedKeys.Add(MouseKey::Left);
	}
	if (m_bgfxMouseState.m_buttons[entry::MouseButton::Right])
	{
		mouseState.pressedKeys.Add(MouseKey::Right);
	}
	if (m_bgfxMouseState.m_buttons[entry::MouseButton::Middle])
	{
		mouseState.pressedKeys.Add(MouseKey::Middle);
	}
	mouseState.hoveredWindow = m_application->GetNativeWindow();

	return mouseState;
}

KeyState BgfxGuiApplication::Impl::GetKeyState()
{
	// TODO: update ctrl, alt and shift states

	KeyState keyState;
	for (Key key : GetAllKeys())
	{
		std::optional<entry::Key::Enum> bgfxKey = convertBgfxKey(key);
		if (!bgfxKey)
		{
			continue;
		}

		uint8_t bgfxKeyModifiers;
		keyState.IsPressed(key) = inputGetKeyState(bgfxKey.value(), &bgfxKeyModifiers);
	}

	return keyState;
}
