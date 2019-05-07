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
}

void ViewBase::OnRemoveGroup(ptr<EntityGroupData> group)
{
}

const ComponentsSet& ViewBase::GetComponentsSet() const
{
	return m_componentsSet;
}
