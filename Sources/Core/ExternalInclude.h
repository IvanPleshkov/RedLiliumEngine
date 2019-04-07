#pragma once

#include "StdInclude.h"
#include <nonstd/span.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_CXX17

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>

#include <spdlog/spdlog.h>

#define JSON_NOEXCEPTION

#include <nlohmann/json.hpp>

#include <argh.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include <opencv2/opencv.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>
