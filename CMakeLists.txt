cmake_minimum_required (VERSION 3.2.3)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(PROJECT_NAME RedLiliumEngine)
PROJECT("${PROJECT_NAME}")

set(CMAKE_SRC
		Sources/common.cmake)
add_custom_target(CmakeProject SOURCES ${CMAKE_SRC})

add_subdirectory("Sources/Core")
add_subdirectory("Sources/Demos")
add_subdirectory("Sources/Tests")

# setup default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demos)
