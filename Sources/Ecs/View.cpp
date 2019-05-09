#include "pch.h"
#include "View.h"
#include "Scene.h"
#include "Entity.h"

using namespace RED_LILIUM_NAMESPACE;

ViewBase::ViewBase()
{
}

ViewBase::~ViewBase()
{
}

void ViewBase::OnAddGroup(ptr<EntityGroupData> group)
{
	const ComponentsSet& componentsSet = group->GetComponentsSet();
	if (std::includes(componentsSet.begin(), componentsSet.end(), m_componentsSet.begin(), m_componentsSet.end()))
	{
		m_groups.insert(group);
	}
}

void ViewBase::OnRemoveGroup(ptr<EntityGroupData> group)
{
	const ComponentsSet& componentsSet = group->GetComponentsSet();
	if (std::includes(componentsSet.begin(), componentsSet.end(), m_componentsSet.begin(), m_componentsSet.end()))
	{
		m_groups.erase(group);
	}
}

const ComponentsSet& ViewBase::GetComponentsSet() const
{
	return m_componentsSet;
}
