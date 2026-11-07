#!/bin/sh
# Run from the w64devkit shell:  sh build.sh C:/path/to/SDL2-2.32.8/x86_64-w64-mingw32
set -e
SDL="$1"
if [ -z "$SDL" ]; then
    echo "usage: sh build.sh <path to the x86_64-w64-mingw32 folder inside the SDL2 mingw package>"
    exit 1
fi
cd "$(dirname "$0")"
cmake -S desktop_version -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
    "-DSDL2_INCLUDE_DIRS=$SDL/include/SDL2" \
    "-DSDL2_LIBRARIES=mingw32;$SDL/lib/libSDL2main.a;$SDL/lib/libSDL2.dll.a" \
    "-DCMAKE_EXE_LINKER_FLAGS=-static -static-libgcc -static-libstdc++"
cmake --build build
cp autosplitter/LiveSplit.VVVVVV.asl build/
echo
echo "Built: build/VVVVVV.exe and build/LiveSplit.VVVVVV.asl  (copy $SDL/bin/SDL2.dll next to the exe)"
