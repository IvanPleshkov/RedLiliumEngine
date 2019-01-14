#include "pch.h"
#include "EditorImguiApplication.h"

using namespace RED_LILIUM_NAMESPACE;

EditorImguiWindow::EditorImguiWindow()
	: m_caption("Window")
{}

const std::string& EditorImguiWindow::GetCaption()
{
	return m_caption;
}

void EditorImguiWindow::SetCaption(const std::string& caption)
{
	m_caption = caption;
}

void EditorImguiWindow::Tick()
{}

void EditorImguiApplication::Init()
{
	m_windows.push_back(umake<EditorImguiWindow>());
	m_windows.back()->SetCaption("Window 1");
	m_windows.push_back(umake<EditorImguiWindow>());
	m_windows.back()->SetCaption("Window 2");
	m_windows.push_back(umake<EditorImguiWindow>());
	m_windows.back()->SetCaption("Window 3");
}

void EditorImguiApplication::Tick(f32 dTime)
{
	const auto pos = ImVec2(0.0f, 0.0f);
	const auto& vPos = ImGui::GetMainViewport()->Pos;
	ImGui::SetNextWindowPos(ImVec2(vPos.x + pos.x, vPos.y + pos.y));

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	auto size = ImVec2((int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui::SetNextWindowSize(size);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("editor", nullptr, flags);
	ImGui::PopStyleVar();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New", "Ctrl+N");
			ImGui::MenuItem("Open", "Ctrl+O");
			ImGui::MenuItem("Save", "Ctrl+S");
			ImGui::MenuItem("Save as...");
			ImGui::Separator();
			ImGui::MenuItem("Exit", "Alt+F4");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl+Z");
			ImGui::MenuItem("Redo", "Ctrl+Y");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	// Create a DockSpace node where any window can be docked
	ImGuiID dockspaceId = ImGui::GetID("#contexts");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), 0, &m_guiWindowClass);

	for (auto& window : m_windows)
	{
		WindowTick(dockspaceId, window.get());
	}

	ImGui::End();
}

void EditorImguiApplication::PollEvent(ptr<SDL_Event> event)
{
	if (event->type == SDL_QUIT)
	{
		Stop();
	}

	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_CLOSE &&
		event->window.windowID == SDL_GetWindowID(GetMainWindow()))
	{
		Stop();
	}
}

void EditorImguiApplication::WindowTick(ImGuiID dockspaceId, ptr<EditorImguiWindow> window)
{
	bool open = true;

	ImGui::SetNextWindowClass(&m_guiWindowClass);
	ImGui::SetNextWindowDockID(dockspaceId, ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	const bool isOpen = ImGui::Begin(window->GetCaption().data(), &open, flags);
	ImGui::PopStyleVar();
	if (isOpen)
	{
		if (auto* viewport = ImGui::GetWindowViewport())
		{
			if (viewport->PlatformUserData == 0)
			{
				viewport->PlatformUserData = reinterpret_cast<void*>(-1);
			}
		}

		ImGui::PushID(reinterpret_cast<void*>(window));
		window->Tick();
		ImGui::PopID();
	}
	ImGui::End();
}
