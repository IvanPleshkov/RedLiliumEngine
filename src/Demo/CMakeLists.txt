cmake_minimum_required (VERSION 3.2.3)

set(PROJECT_NAME Demo)
PROJECT("Demo")

include("../common.cmake")

add_definitions(-DENTRY_CONFIG_IMPLEMENT_MAIN)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} EditorFramework)
target_link_libraries(${PROJECT_NAME} GuiFramework)
target_link_libraries(${PROJECT_NAME} GuiWidgets)

target_link_libraries(${PROJECT_NAME} bgfx)
target_link_libraries(${PROJECT_NAME} example-common)
target_link_libraries(${PROJECT_NAME} bimg)
target_link_libraries(${PROJECT_NAME} bx)

get_filename_component(DATA_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../data" ABSOLUTE)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/paths.json" "${DATA_ABSOLUTE_PATH}")
