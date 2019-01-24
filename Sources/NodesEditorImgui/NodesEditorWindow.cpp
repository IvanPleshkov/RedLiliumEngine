#include "pch.h"
#include "NodesEditorWindow.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"
#include <Nodes/Components/NodeComponent.h>
#include <NodesEditor/UserInput.h>
#include <NodesEditor/Events/MouseKeyDown.h>
#include <NodesEditor/Events/MouseKeyUp.h>
#include <NodesEditor/Events/MouseMove.h>
#include <NodesEditor/Events/MouseScroll.h>

using namespace RED_LILIUM_NAMESPACE;

NodesEditorWindow::NodesEditorWindow(ptr<EditorImguiApplication> application)
	: EditorImguiWindow()
	, m_application(application)
	, m_editorSystem(application->GetEditorSystem())
	, m_canvasPos(0, 0)
	, m_canvasSize(1, 1)
	, m_scale(1)
	, m_translate(0, 0)
{
	SetCaption("Nodes");
}

vec2 NodesEditorWindow::GetScreenPosition(vec2 globalPosition)
{
	return globalPosition - m_canvasPos - m_canvasSize / 2;
}

vec2 NodesEditorWindow::GetGlobalPosition(vec2 screenPosition)
{
	return screenPosition + m_canvasPos + m_canvasSize / 2;
}

void NodesEditorWindow::Tick()
{
	ptr<ImDrawList> imguiDrawList = ImGui::GetWindowDrawList();

	ptr<const Scene> scene = m_editorSystem->GetCurrentEditor()->GetScene();
	ptr<const Entity> root = scene->GetRoot();
	ptr<const Entity> nodesRoot = root->GetChild("Nodes");
	RED_LILIUM_ASSERT(nodesRoot != nullptr);

	auto canvasPos = ImGui::GetCursorScreenPos();;
	m_canvasPos = { canvasPos.x, canvasPos.y };
	auto canvasSize = ImGui::GetContentRegionAvail();
	m_canvasSize = { canvasSize.x, canvasSize.y };

	imguiDrawList->AddRect(canvasPos, ImVec2(m_canvasPos.x + m_canvasSize.x, m_canvasPos.y + m_canvasSize.y), IM_COL32(100, 0, 0, 255));
	ImGui::InvisibleButton("canvas", { m_canvasSize.x, m_canvasSize.y });

	MouseState currentMouseState;
	currentMouseState.position = { ImGui::GetIO().MousePos.x - m_canvasPos.x, ImGui::GetIO().MousePos.y - m_canvasPos.y };
	currentMouseState.pressedKeys = Flags<MouseKey>();
	if (ImGui::IsMouseDown(0))
	{
		currentMouseState.pressedKeys.Add(MouseKey::Left);
	}
	if (ImGui::IsMouseDown(1))
	{
		currentMouseState.pressedKeys.Add(MouseKey::Right);
	}
	if (ImGui::IsMouseDown(2))
	{
		currentMouseState.pressedKeys.Add(MouseKey::Middle);
	}

	{
		std::vector<uptr<Event>> events;

		if (ImGui::IsMousePosValid(&ImGui::GetMousePos()) && ImGui::IsItemHovered())
		{
			if (ImGui::GetIO().MouseWheel != 0.0f)
			{
				events.push_back(std::move(umake<MouseScroll>(currentMouseState, ImGui::GetIO().MouseWheel)));
			}
			if (ImGui::IsMouseClicked(0))
			{
				events.push_back(std::move(umake<MouseKeyDown>(currentMouseState, MouseKey::Left)));
			}
			if (ImGui::IsMouseClicked(1))
			{
				events.push_back(std::move(umake<MouseKeyDown>(currentMouseState, MouseKey::Right)));
			}
			if (ImGui::IsMouseClicked(2))
			{
				events.push_back(std::move(umake<MouseKeyDown>(currentMouseState, MouseKey::Middle)));
			}
			if (ImGui::IsMouseReleased(0))
			{
				events.push_back(std::move(umake<MouseKeyUp>(currentMouseState, MouseKey::Left)));
			}
			if (ImGui::IsMouseReleased(1))
			{
				events.push_back(std::move(umake<MouseKeyUp>(currentMouseState, MouseKey::Right)));
			}
			if (ImGui::IsMouseReleased(2))
			{
				events.push_back(std::move(umake<MouseKeyUp>(currentMouseState, MouseKey::Middle)));
			}
			if (m_mousePos != currentMouseState.position)
			{
				events.push_back(std::move(umake<MouseMove>(currentMouseState, m_mousePos)));
				m_mousePos = currentMouseState.position;
			}
		}

		for (auto& event : events)
		{
			m_editorSystem->GetCurrentEditor()->HandleEvent(event.get());
		}
	}

	u64 nodesCount = nodesRoot->GetChildrenCount();
	for (u64 i = 0; i < nodesCount; i++)
	{
		ptr<const Entity> nodeEntity = nodesRoot->GetChild(i);
		DrawNode(imguiDrawList, nodeEntity);
	}
}

void NodesEditorWindow::DrawNode(ptr<ImDrawList> imguiDrawList, ptr<const Entity> nodeEntity)
{
	const ImU32 nodeFillColor = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
	const ImU32 nodeStrokeColor = ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	ptr<const NodesViewComponent> viewComponent = nodeEntity->GetParent()->GetComponent<NodesViewComponent>();
	RED_LILIUM_ASSERT(viewComponent != nullptr);
	ptr<const NodeComponent> nodeComponent = nodeEntity->GetComponent<NodeComponent>();
	RED_LILIUM_ASSERT(nodeComponent != nullptr);

	vec2 positionStart = GetGlobalPosition(viewComponent->GetScreenPosition(nodeComponent->GetPosition()));
	vec2 positionEnd = GetGlobalPosition(viewComponent->GetScreenPosition(nodeComponent->GetPosition() + nodeComponent->GetSize()));
	imguiDrawList->AddRectFilled({ positionStart.x, positionStart.y }, { positionEnd.x, positionEnd.y }, nodeFillColor);
	imguiDrawList->AddRect({ positionStart.x, positionStart.y }, { positionEnd.x, positionEnd.y }, nodeStrokeColor);
}
