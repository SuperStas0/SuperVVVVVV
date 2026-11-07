@echo off
setlocal
cd /d "%~dp0"

set "CLEAN="
set "REL="
set "HELP="
set "BAD="

:nextarg
if "%~1"=="" goto :parsed
set "ARG=%~1"
shift
if not "%ARG:~0,1%"=="-" goto :badarg
if "%ARG%"=="-" goto :badarg
set "ARG=%ARG:~1%"
:nextchar
if "%ARG%"=="" goto :nextarg
set "CH=%ARG:~0,1%"
set "ARG=%ARG:~1%"
if "%CH%"=="c" set "CLEAN=1" & goto :nextchar
if "%CH%"=="r" set "REL=1" & goto :nextchar
if "%CH%"=="h" set "HELP=1" & goto :nextchar
if not defined BAD set "BAD=-%CH%"
goto :nextchar
:badarg
if not defined BAD set "BAD=%ARG%"
goto :nextarg

:parsed
if defined HELP goto :help
if defined BAD goto :unknown

echo %~dp0| findstr /C:" " >nul
if not errorlevel 1 goto :spaces

where gcc >nul 2>&1
if not errorlevel 1 goto :havegcc
if defined W64DEVKIT if exist "%W64DEVKIT%\bin\gcc.exe" set "PATH=%PATH%;%W64DEVKIT%\bin"
if exist "%ProgramFiles(x86)%\w64devkit\bin\gcc.exe" set "PATH=%PATH%;%ProgramFiles(x86)%\w64devkit\bin"
if exist "%ProgramFiles%\w64devkit\bin\gcc.exe" set "PATH=%PATH%;%ProgramFiles%\w64devkit\bin"
if exist "C:\w64devkit\bin\gcc.exe" set "PATH=%PATH%;C:\w64devkit\bin"
:havegcc
where gcc >nul 2>&1
if errorlevel 1 goto :nogcc
where g++ >nul 2>&1
if errorlevel 1 goto :nogcc
where cmake >nul 2>&1
if errorlevel 1 goto :nocmake
where ninja >nul 2>&1
if errorlevel 1 goto :nocmake

set "SDL=%CD%\SDL32\x86_64-w64-mingw32"
if not exist "%SDL%\include\SDL2\SDL.h" set "SDL=%CD%\SDL32"
if not exist "%SDL%\include\SDL2\SDL.h" goto :nosdl
if not exist "%SDL%\lib\libSDL2.a" goto :nosdl
if not exist "%SDL%\lib\libSDL2main.a" goto :nosdl
set "SDLC=%SDL:\=/%"

if defined CLEAN if exist build rmdir /s /q build
if defined CLEAN if exist build goto :cleanfailed

cmake -S desktop_version -B build -G Ninja -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DSTEAM=ON ^
    "-DSDL2_INCLUDE_DIRS=%SDLC%/include/SDL2" ^
    "-DSDL2_LIBRARIES=mingw32;%SDLC%/lib/libSDL2main.a;%SDLC%/lib/libSDL2.a;m;dinput8;dxguid;dxerr8;user32;gdi32;winmm;imm32;ole32;oleaut32;shell32;setupapi;version;uuid" ^
    "-DCMAKE_EXE_LINKER_FLAGS=-static -static-libgcc -static-libstdc++"
if errorlevel 1 goto :failed

cmake --build build
if errorlevel 1 goto :failed

if exist steam_api64.dll copy /y steam_api64.dll build\ >nul
copy /y autosplitter\LiveSplit.VVVVVV.asl build\ >nul || goto :failed

if not defined REL goto :done
if exist release rmdir /s /q release
mkdir release
if exist build\package rmdir /s /q build\package
mkdir build\package
copy /y build\VVVVVV.exe build\package\VVVVVV.exe >nul || goto :failed
if exist steam_api64.dll copy /y steam_api64.dll build\package\steam_api64.dll >nul
copy /y autosplitter\LiveSplit.VVVVVV.asl build\package\LiveSplit.VVVVVV.asl >nul || goto :failed
copy /y readme.md build\package\SuperVVVVVV-README.md >nul || goto :failed
powershell -NoProfile -Command "Compress-Archive -Path 'build\package\*' -DestinationPath 'release\SuperVVVVVV.zip' -Force"
if errorlevel 1 goto :failed

:done
echo.
echo Done: build\VVVVVV.exe and build\LiveSplit.VVVVVV.asl
if defined REL echo Release: release\SuperVVVVVV.zip
if not exist steam_api64.dll echo Note: steam_api64.dll not found in the repo root, Steam achievements need it next to VVVVVV.exe.
exit /b 0

:help
echo Usage: build.bat [-c] [-r] [-h]
echo   -c  clean: delete previous build output first
echo   -r  release: package the build into release\
echo   -h  show this help
echo Build output: %CD%\build
echo Release:      %CD%\release
exit /b 0

:unknown
echo ERROR: unknown flag %BAD%, see build.bat -h
exit /b 1

:spaces
echo ERROR: repo path contains spaces, which breaks icon embedding. Move the repo to a path without spaces.
exit /b 1

:nogcc
echo ERROR: gcc/g++ not found. Add the w64devkit bin folder to PATH or set W64DEVKIT to its folder.
exit /b 1

:nocmake
echo ERROR: cmake/ninja not found. Run pip install cmake ninja and add Python's Scripts folder to PATH.
exit /b 1

:nosdl
echo ERROR: SDL2 MinGW package incomplete in SDL32, expected include\SDL2\SDL.h, lib\libSDL2.a and lib\libSDL2main.a.
exit /b 1

:cleanfailed
echo ERROR: could not delete the build folder, close programs using it.
exit /b 1

:failed
echo.
echo ERROR: build failed.
exit /b 1
