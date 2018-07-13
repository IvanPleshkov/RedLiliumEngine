cmake_minimum_required (VERSION 3.2.3)

if(WIN32)
	SET(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
endif()

set(PROJECT_NAME RedLiliumEngine)
PROJECT("RedLiliumEditor")

# 3rdparty
add_subdirectory("3rdparty/googletest")
add_subdirectory("3rdparty/bgfx")

add_subdirectory("src/Core")
add_subdirectory("src/EditorFramework")
add_subdirectory("src/GuiFramework")
add_subdirectory("src/GuiWidgets")
add_subdirectory("src/Demo")
add_subdirectory("src/tests")
