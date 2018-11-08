cmake_minimum_required (VERSION 3.2.3)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(WIN32)
	SET(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
endif()

set(PROJECT_NAME RedLiliumEngine)
PROJECT("${PROJECT_NAME}")

include("3rdparty/3rdparty.cmake")

set(CMAKE_SRC
		src/common.cmake
		3rdparty/3rdparty.cmake)
add_custom_target(CmakeProject SOURCES ${CMAKE_SRC})

add_subdirectory("src/Core")
add_subdirectory("src/TaskManager")
#add_subdirectory("src/ResourceManager")
add_subdirectory("src/Render")
add_subdirectory("src/WindowsManager")
add_subdirectory("src/EditorFramework")
add_subdirectory("src/Gui2")
add_subdirectory("src/GuiFramework")
add_subdirectory("src/GuiWidgets")
add_subdirectory("src/GuiBgfxAdapter")
add_subdirectory("src/Demos")
add_subdirectory("src/tests")
