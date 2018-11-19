#include "pch.h"

#include "Widget.h"

using namespace RED_LILIUM_NAMESPACE;
//
//using WidgetStyleId = u32;
//
//class GuiContext : public RedLiliumObject
//{};
//
//struct WidgetHandle
//{
//	
//};
//
//class WidgetComponent : public RedLiliumObject
//{
//public:
//	~WidgetComponent() override = default;
//
//protected:
//};
//
//
//struct WidgetComponentBuilder
//{};
//
//class FixedPositionComponent : public WidgetComponent
//{
//public:
//
//
//public:
//	~FixedPositionComponent() override = default;
//
//private:
//};
//
//struct FixedPositionComponentBuilder
//{
//
//};
//
//namespace WidgetState
//{
//	u32 Focused;
//}
//
//template<class T>
//class WidgetBuilder
//{
//public:
//	WidgetBuilder(ptr<GuiContext> context, WidgetStyleId style)
//		: m_context(context)
//	{
//	}
//
//	T& Add(std::initializer_list<WidgetHandle> children)
//	{
//		return m_this;
//	}
//
//	T& If(
//		std::function<bool(T& builder)> condition,
//		std::function<void(T& builder)> action)
//	{
//		return m_this
//	}
//
//	T& If(
//		u32 condition,
//		std::function<void(T& actions)> action)
//	{
//		return m_this
//	}
//
//	T& Hint()
//	{
//		return m_this;
//	}
//
//	template<class TComponent>
//	T& Component(std::function<void(TComponent& component)> action)
//	{
//		return m_this;
//	}
//
//	template<class TComponent>
//	T& Component()
//	{
//		return m_this;
//	}
//
//	WidgetHandle End()
//	{
//		return WidgetHandle();
//	}
//
//private:
//	T& m_this;
//	ptr<GuiContext> m_context;
//	WidgetStyleId m_style;
//};
//
//class Button : public WidgetBuilder<Button>
//{
//public:
//	Button(ptr<GuiContext> context, WidgetStyleId style = 0)
//		: WidgetBuilder(context, style)
//	{ }
//
//	Button& Text(std::string_view text)
//	{
//		return *this;
//	}
//
//	Button& OnClick(std::function<void()> action)
//	{
//		return *this;
//	}
//};
//
//class StackLayout : public WidgetBuilder<StackLayout>
//{
//public:
//	StackLayout(ptr<GuiContext> context, WidgetStyleId style)
//		: WidgetBuilder(context, style)
//	{ }
//};
//
//WidgetHandle f(ptr<GuiContext> context)
//{
//	WidgetStyleId style = 0;
//	return StackLayout(context, style)
//		.Add({
//			Button(context)
//				.OnClick([]() {})
//				.End(),
//			Button(context)
//				.OnClick([]() {})
//				.Component<Button>([](auto& t) {
//					t.Text("RRRR");
//				})
//				.Component<Button>()
//				.End(),
//			Button(context, style)
//				.If(WidgetState::Focused, [](auto& t) { t
//					.Text("Hello");
//					})
//				.OnClick([]() {})
//				.End(),
//			f(context),
//			f(context),
//			f(context),
//			f(context),
//			})
//		.End();
//}
