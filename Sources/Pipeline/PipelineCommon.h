#pragma once

#include <Core/Common.h>
#include <Render/RenderCommon.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderPipeline;
class RenderPass;

class CameraComponent;
class MeshRenderer;
class MeshFilter;

using RenderComponentsPair = std::pair<ptr<const MeshFilter>, ptr<const MeshRenderer>>;

} // namespace RED_LILIUM_NAMESPACE
