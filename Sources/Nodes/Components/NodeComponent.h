#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeComponent, Component);

	NodeComponent();
	NodeComponent(ptr<Entity> parent);
	~NodeComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	vec2 GetPosition() const;
	void SetPosition(vec2 position);
	vec2 GetSize() const;
	void SetSize(vec2 size);

private:
	vec2 m_position;
	vec2 m_size;
};

} // namespace RED_LILIUM_NAMESPACE
