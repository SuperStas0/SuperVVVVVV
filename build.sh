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
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DSTEAM=ON \
    "-DSDL2_INCLUDE_DIRS=$SDL/include/SDL2" \
    "-DSDL2_LIBRARIES=mingw32;$SDL/lib/libSDL2main.a;$SDL/lib/libSDL2.a;m;dinput8;dxguid;dxerr8;user32;gdi32;winmm;imm32;ole32;oleaut32;shell32;setupapi;version;uuid" \
    "-DCMAKE_EXE_LINKER_FLAGS=-static -static-libgcc -static-libstdc++"
cmake --build build
cp autosplitter/LiveSplit.VVVVVV.asl build/
if [ -f steam_api64.dll ]; then cp steam_api64.dll build/; fi
echo
echo "Built: build/VVVVVV.exe and build/LiveSplit.VVVVVV.asl"
