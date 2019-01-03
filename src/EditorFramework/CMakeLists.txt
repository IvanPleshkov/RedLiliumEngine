project("EditorFramework")

include("../common.cmake")

add_library(${PROJECT_NAME} ${SRC_FILES})	
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
