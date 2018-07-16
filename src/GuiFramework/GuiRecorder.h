#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

enum class GuiRecordingDataType
{
	MouseState			= 1 << 0,
	KeyState			= 1 << 1,
	WidgetsTree			= 1 << 2
};
using GuiRecordingDataTypes = Flags<GuiRecordingDataType>;

class GuiRecorder : public RedLiliumObject
{
public:
	GuiRecorder(GuiRecordingDataTypes recordingData);
	~GuiRecorder() override;

	void PushNewFrame();

	void PushFrameTime(const Time& time);
	void PushMouseState(const MouseState& mouseState);
	void PushKeyState(const KeyState& keyState);

	void PushConstructWidget(const ptr<const Widget> widget);
	void PushDestructWidget(const ptr<const Widget> widget);

	void PushLogMessage(const std::string& message);

private:
	GuiRecordingDataTypes m_recordingDataTypes;
	uptr<GuiReplayData> m_data;
};

class GuiReplayData : public RedLiliumObject
{
public:
	~GuiReplayData() override;
};

} // namespace RED_LILIUM_NAMESPACE
