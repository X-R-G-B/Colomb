#!/bin/bash
# Run cpack

set -ex

rm -rf build

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DPACKAGED=1 -DNO_USE_CLANG_TIDY=true

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G STGZ

mv Colomb_server-Linux.sh ../Colomb_server-linux.sh

cpack --config CPackSourceConfig.cmake -G TGZ

mv Colomb_server-Linux.tar.gz ../Colomb_server-linux.tar.gz
