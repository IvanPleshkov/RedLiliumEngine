#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

enum class GuiRecordingMode
{
	NoRecording				= 0,
	RecordReplay			= 1 << 0,
	RecordFunctionalTest	= 1 << 1
};

class GuiRecorder : public RedLiliumObject
{
public:
	GuiRecorder(GuiRecordingMode recordingMode);
	~GuiRecorder() override;

	void PushNewFrame();

	void PushFrameTime(const Time& time);
	void PushMouseState(const MouseState& mouseState);
	void PushKeyState(const KeyState& keyState);

	void PushLogMessage(const std::string& message);

	ptr<GuiReplayData> GetReplayData();
	ptr<const GuiReplayData> GetReplayData() const;

private:
	GuiRecordingMode m_recordingMode;
	uptr<GuiReplayData> m_data;
};


class GuiReplayData : public RedLiliumObject
{
public:
	~GuiReplayData() override;

	void Save(const std::string& filename) const;
	void Load(const std::string& filename);
	void Clear();

	u32 GetFramesCount() const;

	const Time& GetTime(u32 frameIndex) const;
	const MouseState& GetMouseState(u32 frameIndex) const;
	const KeyState& GetKeyState(u32 frameIndex) const;

	u32 GetLogMessagesCount(u32 frameIndex) const;
	const std::string GetLogMessage(u32 frameIndex, u32 messageIndex) const;

private:
	friend class GuiRecorder;

	static const u32 currentReplayVersion = 0;

	u32 m_framesCount;
	std::vector<Time> m_timeData;
	std::vector<MouseState> m_mouseState;
	std::vector<KeyState> m_keyState;
	std::vector<std::vector<std::string>> m_logMessagesData;
};

} // namespace RED_LILIUM_NAMESPACE
