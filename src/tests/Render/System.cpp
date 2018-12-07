#include "pch.h"

#include "Tests.h"
#include <Render/System.h>

using namespace RED_LILIUM_NAMESPACE;

TEST(RenderSystem, Init)
{
	uptr<Render::System> renderSystem = umake<Render::System>();
}
