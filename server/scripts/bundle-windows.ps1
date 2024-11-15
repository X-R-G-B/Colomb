#!/bin/pwsh
# Run cpack

$ErrorActionPreference = "SilentlyContinue"

rm -Recurse build

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES="Release" -DPACKAGED=1 -DNO_USE_CLANG_TIDY=true

cmake --build build --config Release

cd build

cpack --config CPackConfig.cmake -G NSIS

if ($LASTEXITCODE -ne 0) {
    type _CPack_Packages/win64/NSIS/NSISOutput.log
}

Copy-Item Colomb_server-Windows.exe ../Colomb_server-windows.exe

cpack --config CPackSourceConfig.cmake -G ZIP

Copy-Item Colomb_server-Windows.zip ../Colomb_server-windows.zip

cd ..
