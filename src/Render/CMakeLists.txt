project("Render")

include("../common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	

target_link_libraries( ${PROJECT_NAME} "${Vulkan_LIBRARIES}" )
