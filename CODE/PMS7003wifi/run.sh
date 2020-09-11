


#!/bin/bash

HISTSIZE=

if [ ! -d "build_tests" ]; then
  mkdir build_tests;
fi
cd build_tests
CMAKE_CXX_EXTENSIONS=ON
#cmake .. -G Ninja  -DCMAKE_TOOLCHAIN_FILE=D:/Programs/ESP/NEW/.espressif/releases/esp-idf-v4.0/tools/cmake/project.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DIDF_TARGET=esp32 -DTARGET=esp32 
CXX="D:/Program Files/Qt/Tools/mingw730_64/bin/c++.exe"
cmake .. -G Ninja -DTARGET=unit_tests  -DCMAKE_BUILD_TYPE=Release
echo "executing cmake --build ."
cmake --build . -- -v 

./PMS7003wifi_test.exe