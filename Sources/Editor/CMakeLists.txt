project(Editor)

include("../Common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} Reflection)