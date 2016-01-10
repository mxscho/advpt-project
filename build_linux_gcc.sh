#!/usr/bin/env bash

export CXX=$(which g++)

BUILD_DIRECTORY="bin"
rm -rf $BUILD_DIRECTORY
mkdir -p $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

cmake ..
make -j
