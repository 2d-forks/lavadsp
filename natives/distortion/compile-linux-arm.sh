#!/bin/bash

WD="$PWD"

cd ${BASH_SOURCE%/*}

g++ -mthumb -O3 -shared -fPIC -I"${JAVA_HOME}/include/linux" -I"${JAVA_HOME}/include" -I"distortion-effect" -std=c++17 -o "$WD/libdistortion.so" *.cpp distortion-effect/*.cpp
