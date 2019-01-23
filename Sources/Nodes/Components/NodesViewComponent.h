#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodesViewComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeComponent, Component);

	NodesViewComponent();
	NodesViewComponent(ptr<Entity> parent);
	~NodesViewComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	float GetScale() const;
	void SetScale(float value);
	vec2 GetTranslate() const;
	void SetTranslate(vec2 translate);

	vec2 GetScreenPosition(vec2 documentPosition) const;
	vec2 GetDocumentPosition(vec2 screenPosition) const;

private:
	float m_scale;
	vec2 m_translate;
};

} // namespace RED_LILIUM_NAMESPACE
