mkdir build
cd build
conan install .. -s build_type=Debug --build=missing
cmake .. -DCMAKE_CONFIGURATION_TYPES=Debug -G Xcode
