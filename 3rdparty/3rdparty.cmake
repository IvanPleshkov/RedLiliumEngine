# RedLilium

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#==================== GOOGLE TEST
add_subdirectory("3rdparty/googletest")

set_target_properties (gtest PROPERTIES FOLDER 3rdparty)
set_target_properties (gmock PROPERTIES FOLDER 3rdparty)
set_target_properties (gmock_main PROPERTIES FOLDER 3rdparty)
set_target_properties (gtest_main PROPERTIES FOLDER 3rdparty)

#==================== BGFX
add_subdirectory("3rdparty/bgfx")

#add_subdirectory("3rdparty/glfw")
#set_target_properties (glfw PROPERTIES FOLDER 3rdparty)
#add_subdirectory("3rdparty/glew")
