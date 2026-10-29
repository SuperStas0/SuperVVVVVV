@echo off
setlocal
cd /d "%~dp0"

rem Usage: build.bat [path to SDL2 x86_64-w64-mingw32 folder]
rem Default SDL2 folder: SDL32\x86_64-w64-mingw32 next to this file.

rem windres (icon embedding) breaks on paths with spaces.
echo %~dp0| findstr /C:" " >nul
if not errorlevel 1 goto :spaces

where gcc >nul 2>&1
if errorlevel 1 goto :nogcc
where g++ >nul 2>&1
if errorlevel 1 goto :nogcc
where cmake >nul 2>&1
if errorlevel 1 goto :nocmake
where ninja >nul 2>&1
if errorlevel 1 goto :nocmake

set "SDL=%~1"
if "%SDL%"=="" set "SDL=%~dp0SDL32\x86_64-w64-mingw32"
for %%I in ("%SDL%") do set "SDL=%%~fI"
if not exist "%SDL%\include\SDL2\SDL.h" goto :nosdl
if not exist "%SDL%\lib\libSDL2.dll.a" goto :nosdl
if not exist "%SDL%\bin\SDL2.dll" goto :nosdl
set "SDLC=%SDL:\=/%"

cmake -S desktop_version -B build -G Ninja -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ^
    "-DSDL2_INCLUDE_DIRS=%SDLC%/include/SDL2" ^
    "-DSDL2_LIBRARIES=mingw32;%SDLC%/lib/libSDL2main.a;%SDLC%/lib/libSDL2.dll.a" ^
    "-DCMAKE_EXE_LINKER_FLAGS=-static -static-libgcc -static-libstdc++"
if errorlevel 1 goto :failed

cmake --build build
if errorlevel 1 goto :failed

copy /y "%SDL%\bin\SDL2.dll" build\ >nul
echo.
echo Done: build\VVVVVV.exe and build\SDL2.dll
echo Run release.bat to create the archive.
exit /b 0

:spaces
echo ERROR: this folder's path contains spaces. Move the repo to a path without spaces, for example C:\vsrc
exit /b 1

:nogcc
echo ERROR: gcc/g++ not found in PATH. Add the w64devkit bin folder to PATH.
exit /b 1

:nocmake
echo ERROR: cmake/ninja not found in PATH. Install with: pip install cmake ninja
echo and make sure Python's Scripts folder is in PATH.
exit /b 1

:nosdl
echo ERROR: SDL2 MinGW package not found at: %SDL%
echo Expected include\SDL2\SDL.h, lib\libSDL2.dll.a and bin\SDL2.dll inside it.
echo Pass the x86_64-w64-mingw32 folder as the first argument.
exit /b 1

:failed
echo.
echo Build failed.
exit /b 1
