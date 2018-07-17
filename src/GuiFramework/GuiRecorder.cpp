#include "pch.h"
#include "GuiRecorder.h"
#include "Panel.h"
#include "Widget.h"

using namespace RED_LILIUM_NAMESPACE;

//====================== GuiRecorder
GuiRecorder::GuiRecorder(GuiRecordingMode recordingMode)
	: m_recordingMode(recordingMode)
{}

GuiRecorder::~GuiRecorder()
{}

void GuiRecorder::PushNewFrame()
{
	m_data->m_logMessagesData.push_back({});
	m_data->m_framesCount++;
}

void GuiRecorder::PushFrameTime(const Time& time)
{
	if (m_recordingMode != GuiRecordingMode::NoRecording)
	{
		m_data->m_timeData.push_back(time);
	}
}

void GuiRecorder::PushMouseState(const MouseState& mouseState)
{
	if (m_recordingMode != GuiRecordingMode::NoRecording)
	{
		m_data->m_mouseState.push_back(mouseState);
	}
}

void GuiRecorder::PushKeyState(const KeyState& keyState)
{
	if (m_recordingMode != GuiRecordingMode::NoRecording)
	{
		m_data->m_keyState.push_back(keyState);
	}
}

void GuiRecorder::PushLogMessage(const std::string& message)
{
	if (m_recordingMode != GuiRecordingMode::NoRecording)
	{
		m_data->m_logMessagesData.back().push_back(message);
	}
}

ptr<GuiReplayData> GuiRecorder::GetReplayData()
{
	return m_data.get();
}

const ptr<GuiReplayData> GuiRecorder::GetReplayData() const
{
	return m_data.get();
}


//====================== GuiReplayData
GuiReplayData::~GuiReplayData()
{}

void GuiReplayData::Save(const std::string& filename) const
{
}

void GuiReplayData::Load(const std::string& filename)
{
	Clear();
}

void GuiReplayData::Clear()
{
	m_framesCount = 0;
	m_timeData.clear();
	m_mouseState.clear();
	m_keyState.clear();
	m_logMessagesData.clear();
}

u32 GuiReplayData::GetFramesCount() const
{
	return m_framesCount;
}

const Time& GuiReplayData::GetTime(u32 frameIndex) const
{
	RED_LILIUM_ASSERT(frameIndex < m_framesCount);
	return m_timeData[frameIndex];
}

const MouseState& GuiReplayData::GetMouseState(u32 frameIndex) const
{
	RED_LILIUM_ASSERT(frameIndex < m_framesCount);
	return m_mouseState[frameIndex];
}

const KeyState& GuiReplayData::GetKeyState(u32 frameIndex) const
{
	RED_LILIUM_ASSERT(frameIndex < m_framesCount);
	return m_keyState[frameIndex];
}

u32 GuiReplayData::GetLogMessagesCount(u32 frameIndex) const
{
	RED_LILIUM_ASSERT(frameIndex < m_framesCount);
	return m_logMessagesData[frameIndex].size();
}

const std::string GuiReplayData::GetLogMessage(u32 frameIndex, u32 messageIndex) const
{
	RED_LILIUM_ASSERT(frameIndex < m_framesCount);
	RED_LILIUM_ASSERT(messageIndex < m_logMessagesData[frameIndex].size());
	return m_logMessagesData[frameIndex][messageIndex];
}
