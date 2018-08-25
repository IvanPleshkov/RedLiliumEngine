# RedLilium

# макрос генерации флагов компиляции для precompiled headers
MACRO(ADD_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_PRECOMPILED_HEADER)

# ошибка компиляции при варнингах
if(MSVC)
  # добавляем флаг W4 или заменяем флаг Wx на W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # обновляем флаги при необходимости
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "/std:c++latest /EHsc")
else()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

file(GLOB_RECURSE SRC_FILES
	"*.cpp"
	"*.c"
	"*.hpp"
	"*.h"
	"*.inl")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("..")
include_directories("../../3rdparty/pbhogan")
include_directories("../../3rdparty/glm")
include_directories("../../3rdparty/spdlog/include")
include_directories("../../3rdparty/bgfx/bgfx/include")
include_directories("../../3rdparty/bgfx/bimg/include")
include_directories("../../3rdparty/bgfx/bx/include")
include_directories("../../3rdparty/bgfx/bgfx/examples/common")

# генерация MSVC фильтров
foreach(SRC_FILE IN LISTS SRC_FILES)
	if(IS_ABSOLUTE "${SRC_FILE}")
		file(RELATIVE_PATH SRC_FILE_REL "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC_FILE}")
	else()
		set(SRC_FILE_REL "${SRC_FILE}")
	endif()
	
	get_filename_component(SRC_PATH "${SRC_FILE_REL}" PATH)
	string(REPLACE "/" "\\" SRC_PATH_MSVC "${SRC_PATH}")
	source_group("${SRC_PATH_MSVC}" FILES "${SRC_FILE}")
endforeach()

ADD_PRECOMPILED_HEADER("pch.h" "../pch.cpp" SRC_FILES)
source_group("" FILES "../pch.cpp")
