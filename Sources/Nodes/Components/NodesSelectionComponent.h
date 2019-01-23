#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodesSelectionComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodesSelectionComponent, Component);

	NodesSelectionComponent();
	NodesSelectionComponent(ptr<Entity> parent);
	~NodesSelectionComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
	std::vector<ptr<Entity>> m_selectedNodes;
};

} // namespace RED_LILIUM_NAMESPACE
