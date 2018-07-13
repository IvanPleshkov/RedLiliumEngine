cmake_minimum_required (VERSION 3.2.3)

set(PROJECT_NAME GuiWidgets)
PROJECT("GuiWidgets")

include("../common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	
