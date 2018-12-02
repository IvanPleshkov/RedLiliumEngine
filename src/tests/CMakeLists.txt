project("Tests")

include("../common.cmake")
include_directories("../../3rdparty/googletest/googletest/include")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} Render)
target_link_libraries(${PROJECT_NAME} EditorFramework)
target_link_libraries(${PROJECT_NAME} Gui2)
target_link_libraries(${PROJECT_NAME} TaskManager)

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries( ${PROJECT_NAME} "${Vulkan_LIBRARIES}" )

get_filename_component(DATA_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../data" ABSOLUTE)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/paths.json" "${DATA_ABSOLUTE_PATH}")
