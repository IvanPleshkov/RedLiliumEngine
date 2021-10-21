cmake_minimum_required(VERSION 3.2.3)

set(CMAKE_CXX_STANDARD 17)

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
