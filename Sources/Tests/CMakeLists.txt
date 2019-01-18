project("Tests")

include("../Common.cmake")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} Reflection)
target_link_libraries(${PROJECT_NAME} Scene)
target_link_libraries(${PROJECT_NAME} Editor)
