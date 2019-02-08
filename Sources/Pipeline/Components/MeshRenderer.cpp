#include "pch.h"
#include "MeshRenderer.h"

using namespace RED_LILIUM_NAMESPACE;

MeshRenderer::MeshRenderer()
	: Component()
{}

MeshRenderer::MeshRenderer(ptr<Entity> parent)
	: Component(parent)
{}
