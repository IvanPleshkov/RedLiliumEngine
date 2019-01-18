cmake_minimum_required (VERSION 3.2.3)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(PROJECT_NAME RedLiliumEngine)
PROJECT("${PROJECT_NAME}")

set(BINARY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(BINARY_OUT_DEBUG_DIR ${BINARY_OUT_DIR}/Debug)
set(BINARY_OUT_OPTIMIZED_DIR ${BINARY_OUT_DIR}/Release)

if (MSVC)
	set(EXTERNAL_DEBUG_BINARIES_DIR ${CMAKE_SOURCE_DIR}/3rdparty/Bin/msvc141.debug)
	set(EXTERNAL_OPTIMIZED_BINARIES_DIR ${CMAKE_SOURCE_DIR}/3rdparty/Bin/msvc141.release)
endif()

message("External Debug Binaries: ${EXTERNAL_DEBUG_BINARIES_DIR}")
message("External Optimized Binaries: ${EXTERNAL_OPTIMIZED_BINARIES_DIR}")
file(GLOB_RECURSE EXTERNAL_DEBUG_BINARIES "${EXTERNAL_DEBUG_BINARIES_DIR}/*.*")
file(GLOB_RECURSE EXTERNAL_OPTIMIZED_BINARIES "${EXTERNAL_OPTIMIZED_BINARIES_DIR}/*.*")

foreach( EXTERNAL_DEBUG_BINARY ${EXTERNAL_DEBUG_BINARIES} )
	message("Copy ${EXTERNAL_DEBUG_BINARY} to ${BINARY_OUT_DEBUG_DIR}")
	file(COPY ${EXTERNAL_DEBUG_BINARY} DESTINATION ${BINARY_OUT_DEBUG_DIR})
endforeach( EXTERNAL_DEBUG_BINARY )

foreach( EXTERNAL_OPTIMIZED_BINARY ${EXTERNAL_OPTIMIZED_BINARIES} )
	message("Copy ${EXTERNAL_OPTIMIZED_BINARY} to ${BINARY_OUT_OPTIMIZED_DIR}")
	file(COPY ${EXTERNAL_OPTIMIZED_BINARY} DESTINATION ${BINARY_OUT_OPTIMIZED_DIR})
endforeach( EXTERNAL_OPTIMIZED_BINARY )

get_filename_component(RESOURCES_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resources" ABSOLUTE)
message("Resources path: ${RESOURCES_ABSOLUTE_PATH}")
set(RESOURCES_PATHS_JSON "{ \"resourcesPath\" : \"${RESOURCES_ABSOLUTE_PATH}\" }")
file(WRITE "${BINARY_OUT_DEBUG_DIR}/paths.json" ${RESOURCES_PATHS_JSON})
file(WRITE "${BINARY_OUT_OPTIMIZED_DIR}/paths.json" ${RESOURCES_PATHS_JSON})

set(CMAKE_SRC
		Sources/common.cmake)
add_custom_target(CmakeProject SOURCES ${CMAKE_SRC})

add_subdirectory("Sources/imgui")
add_subdirectory("Sources/Core")
add_subdirectory("Sources/Reflection")
add_subdirectory("Sources/Scene")
add_subdirectory("Sources/Editor")
add_subdirectory("Sources/Demos")
add_subdirectory("Sources/Tests")

# setup default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demos)

