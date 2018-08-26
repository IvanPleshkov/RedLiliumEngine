#include "pch.h"

#include <Core/Signal.h>
#include "Tests.h"

using namespace RED_LILIUM_NAMESPACE;

static int g_staticTestValue = 0;

class TestSender
{
public:
	Signal<int> updateValueSignal;

	int GetValue() const 
	{
		return m_value;
	}

	void SetValue(int value)
	{
		m_value = value;
		updateValueSignal(value);
	}

private:
	int m_value = 0;
};

class TestReceiver
{
public:
	void OnValueUpdated(int value)
	{
		m_value = value;
	}

	virtual void OnValueUpdatedVirtual(int value)
	{
		m_value = value;
	}

	static void OnValueUpdatedStatic(int value)
	{
		g_staticTestValue = value;
	}

	int GetValue() const
	{
		return m_value;
	}

protected:
	int m_value = 0;
};

class TestReceiverChild : public TestReceiver
{
public:
	void OnValueUpdatedVirtual(int value) override
	{
		m_value = value;
		m_secondValue = value;
	}

	int GetSecondValue() const
	{
		return m_secondValue;
	}

private:
	int m_secondValue = 0;
};

TEST(Signal, Method)
{
	TestSender sender;
	TestReceiver receiver;
	sender.updateValueSignal.Connect(&receiver, &TestReceiver::OnValueUpdated);
	sender.SetValue(10);
	ASSERT_TRUE(sender.GetValue() == receiver.GetValue());
}

TEST(Signal, Method_2)
{
	TestSender sender;
	TestReceiver receiver1;
	TestReceiver receiver2;
	sender.updateValueSignal.Connect(&receiver1, &TestReceiver::OnValueUpdated);
	sender.updateValueSignal.Connect(&receiver2, &TestReceiver::OnValueUpdated);
	sender.SetValue(10);
	ASSERT_TRUE(sender.GetValue() == receiver1.GetValue());
	ASSERT_TRUE(sender.GetValue() == receiver2.GetValue());
}

TEST(Signal, Disconnect)
{
	TestSender sender;
	TestReceiver receiver1;
	TestReceiver receiver2;
	sender.updateValueSignal.Connect(&receiver1, &TestReceiver::OnValueUpdated);
	sender.updateValueSignal.Connect(&receiver2, &TestReceiver::OnValueUpdated);
	sender.SetValue(10);

	sender.updateValueSignal.Disconnect(&receiver2, &TestReceiver::OnValueUpdated);
	sender.SetValue(20);

	ASSERT_TRUE(receiver1.GetValue() == 20);
	ASSERT_TRUE(receiver2.GetValue() == 10);
}

TEST(Signal, StaticMethod)
{
	g_staticTestValue = 0;
	TestSender sender;
	sender.updateValueSignal.Connect(&TestReceiver::OnValueUpdatedStatic);
	sender.SetValue(10);
	ASSERT_TRUE(g_staticTestValue == sender.GetValue());
}

TEST(Signal, VirtualMethod)
{
	TestSender sender;
	TestReceiverChild receiver;
	sender.updateValueSignal.Connect(&receiver, &TestReceiver::OnValueUpdatedVirtual);
	sender.SetValue(10);
	ASSERT_TRUE(sender.GetValue() == receiver.GetValue());
	ASSERT_TRUE(sender.GetValue() == receiver.GetSecondValue());
}

TEST(Signal, VirtualMethod_2)
{
	TestSender sender;
	TestReceiverChild receiver;
	sender.updateValueSignal.Connect(&receiver, &TestReceiverChild::OnValueUpdatedVirtual);
	sender.SetValue(10);
	ASSERT_TRUE(sender.GetValue() == receiver.GetValue());
	ASSERT_TRUE(sender.GetValue() == receiver.GetSecondValue());
}
