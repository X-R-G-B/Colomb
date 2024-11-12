#!/bin/bash
# Run cpack

set -ex

rm -rf build

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DPACKAGED=1 -DNO_USE_CLANG_TIDY=true

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G STGZ

mv Colomb-Linux.sh ../Colomb-linux.sh

cpack --config CPackSourceConfig.cmake -G TGZ

mv Colomb-Linux.tar.gz ../Colomb-linux.tar.gz
