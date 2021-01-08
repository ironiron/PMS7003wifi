


#!/bin/bash

HISTSIZE=

if [ ! -d "build_tests" ]; then
  mkdir build_tests;
fi
cd build_tests

CMAKE_CXX_EXTENSIONS=ON
echo "executing cmake .. -G Ninja -DTARGET=unit_tests"
cmake .. -G Ninja -DTARGET=unit_tests
echo "executing cmake --build . -- -v "
cmake --build . -- -v 

if [ $? -eq 0 ]; then
    ./PMS7003wifi_test.exe
else
    echo TESTS BUILDING COMMAND FAILED
fi
