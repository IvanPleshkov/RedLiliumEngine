mkdir build
cd build
conan install .. -s build_type=Debug
cmake .. -DCMAKE_CONFIGURATION_TYPES=Debug -G Xcode
