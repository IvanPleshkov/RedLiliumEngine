cmake_minimum_required (VERSION 3.2.3)

set(PROJECT_NAME GuiFramework)
PROJECT("GuiFramework")

include("../common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	

target_link_libraries(${PROJECT_NAME} bgfx)
target_link_libraries(${PROJECT_NAME} example-common)
target_link_libraries(${PROJECT_NAME} bimg)
target_link_libraries(${PROJECT_NAME} bx)
