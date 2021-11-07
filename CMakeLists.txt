cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME LearnVulkan)
project(${PROJECT_NAME})

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup()

find_package(Vulkan REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories(${Vulkan_INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES
    "src/*.h"
    "src/*.mm"
	"src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

conan_target_link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY})

get_filename_component(RESOURCES_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources" ABSOLUTE)
message("Resources path: ${RESOURCES_ABSOLUTE_PATH}")
set(RESOURCES_PATHS_JSON "{ \"resourcesPath\" : \"${RESOURCES_ABSOLUTE_PATH}\" }")
message("Write path.json to: ${CMAKE_CURRENT_BINARY_DIR}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/paths.json" ${RESOURCES_PATHS_JSON})
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Debug/paths.json" ${RESOURCES_PATHS_JSON})
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Release/paths.json" ${RESOURCES_PATHS_JSON})
