#!/bin/bash
# Run cpack

set -ex

rm -rf build

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DPACKAGED=1 -DNO_USE_CLANG_TIDY=true

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G DragNDrop || (cat "./_CPack_Packages/Darwin/DragNDrop/PreinstallOutput.log" && exit 1)

cp Colomb_server-Darwin.dmg ../Colomb_server-macos.dmg

cpack --config CPackSourceConfig.cmake -G ZIP

cp Colomb_server-Darwin.zip ../Colomb_server-macos.zip
