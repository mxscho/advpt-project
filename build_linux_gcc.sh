#!/usr/bin/env bash

export CXX=$(which g++)

BUILD_DIRECTORY="bin"
REVERSE_BUILD_DIRECTORY=".."
rm -rf ${BUILD_DIRECTORY}
mkdir -p ${BUILD_DIRECTORY}

cd ${BUILD_DIRECTORY}
cmake ${REVERSE_BUILD_DIRECTORY}
cd ${REVERSE_BUILD_DIRECTORY}
cmake --build ${BUILD_DIRECTORY} --target sc2simulator -- -j