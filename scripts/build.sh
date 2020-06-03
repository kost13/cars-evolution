#!/bin/bash
# this file should be called from source directory
rm -rf build
mkdir build
cd build

if (( $# == 1 )); then
cmake ../ -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_PREFIX_PATH:STRING=$1
else 
cmake ../ -DCMAKE_BUILD_TYPE:STRING=Release	
fi

cmake --build . --target all

