project(Pipeline)

include("../Common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	

target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} Reflection)
target_link_libraries(${PROJECT_NAME} Render)
target_link_libraries(${PROJECT_NAME} Geometrics)
