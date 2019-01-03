from conans import ConanFile, CMake

class Pkg(ConanFile):
    settings = "os", "compiler", "arch", "build_type"
    requires = "glm/0.9.9.1@g-truc/stable", "spdlog/1.2.1@bincrafters/stable", "jsonformoderncpp/3.5.0@vthiery/stable", "Core/0.1@user/RedLiliumEngine"
    generators = "cmake"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.h", src="src", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["EditorFramework"]
