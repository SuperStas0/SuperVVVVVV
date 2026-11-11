@echo off
setlocal
cd /d "%~dp0"

set "CLEAN="
set "INST="
set "REL="
set "NOBUILD="
set "HELP="
set "BAD="
set "WANTS="
set "SDLARG="
set "NOPATH="

:nextarg
if defined WANTS goto :takepath
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
if "%CH%"=="i" set "INST=1" & goto :nextchar
if "%CH%"=="r" set "REL=1" & goto :nextchar
if "%CH%"=="n" set "NOBUILD=1" & goto :nextchar
if "%CH%"=="s" set "WANTS=1" & goto :nextchar
if "%CH%"=="h" set "HELP=1" & goto :nextchar
if not defined BAD set "BAD=-%CH%"
goto :nextchar
:badarg
if not defined BAD set "BAD=%ARG%"
goto :nextarg
:takepath
set "WANTS="
set "V=%~1"
if "%V%"=="" goto :nopath
if "%V:~0,1%"=="-" goto :nopath
set "SDLARG=%V%"
shift
goto :nextarg
:nopath
set "NOPATH=1"
goto :nextarg

:parsed
if defined HELP goto :help
if defined BAD goto :unknown
if defined NOPATH goto :nopatherr
if defined NOBUILD if defined CLEAN goto :nbclean
if defined NOBUILD if not defined INST if not defined REL goto :nbneeds
if defined NOBUILD if not exist build\VVVVVV.exe goto :nbmissing
if defined NOBUILD goto :afterbuild

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

set "SDL=%SDLARG%"
if defined SDL goto :sdlset
set "SDL=%CD%\SDL32\x86_64-w64-mingw32"
if not exist "%SDL%\include\SDL2\SDL.h" set "SDL=%CD%\SDL32"
:sdlset
for %%I in ("%SDL%") do set "SDL=%%~fI"
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

:afterbuild
if not defined INST goto :noinst
call :finddir
if not defined GAME goto :nogame
copy /y build\VVVVVV.exe "%GAME%\VVVVVV.exe" >nul || goto :instfailed
if exist build\steam_api64.dll copy /y build\steam_api64.dll "%GAME%\steam_api64.dll" >nul || goto :instfailed
:noinst

if not defined REL goto :done
if exist release rmdir /s /q release
mkdir release
if exist build\package rmdir /s /q build\package
mkdir build\package
copy /y build\VVVVVV.exe build\package\VVVVVV.exe >nul || goto :failed
copy /y autosplitter\LiveSplit.VVVVVV.asl build\package\LiveSplit.VVVVVV.asl >nul || goto :failed
copy /y readme.md build\package\SuperVVVVVV-README.md >nul || goto :failed
copy /y licenses\LICENSE.md build\package\LICENSE.md >nul || goto :failed
xcopy /s /i /y /q "desktop_version\lang\super.xml" "build\package\lang\" >nul || goto :failed
powershell -NoProfile -Command "Compress-Archive -Path 'build\package\*' -DestinationPath 'release\SuperVVVVVV.zip' -Force"
if errorlevel 1 goto :failed
powershell -NoProfile -Command "$n=[char]10; $z=(Get-FileHash -Algorithm SHA256 -LiteralPath 'release\SuperVVVVVV.zip').Hash.ToLower(); $e=(Get-FileHash -Algorithm SHA256 -LiteralPath 'build\package\VVVVVV.exe').Hash.ToLower(); [IO.File]::WriteAllText('release\SHA256.txt', $z + ' *SuperVVVVVV.zip' + $n + $e + ' *VVVVVV.exe' + $n)"
if errorlevel 1 goto :failed

:done
echo.
if not defined NOBUILD echo Done: build\VVVVVV.exe and build\LiveSplit.VVVVVV.asl
if defined INST echo Installed: %GAME%
if defined REL echo Release: release\SuperVVVVVV.zip
if defined REL echo Checksums: release\SHA256.txt
if not exist steam_api64.dll echo Note: steam_api64.dll not found in the repo root, Steam achievements need it next to VVVVVV.exe.
exit /b 0

:help
call :finddir
if not defined GAME set "GAME=not found"
echo Usage: build.bat [-c] [-i] [-r] [-n] [-s path] [-h]
echo   -c       clean: delete previous build output first
echo   -i       install: copy the build to the Steam game folder, replacing VVVVVV.exe: %GAME%
echo   -r       release: package the build into release\
echo   -n       nobuild: skip the build, reuse existing output ^(needs -i and/or -r, not with -c^)
echo   -s path  SDL2: x86_64-w64-mingw32 folder of the SDL2 MinGW package, default SDL32 in the repo
echo   -h       show this help
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
echo ERROR: SDL2 MinGW package not found at %SDL%, expected include\SDL2\SDL.h, lib\libSDL2.a and lib\libSDL2main.a inside it.
exit /b 1

:nbclean
echo ERROR: -n cannot be used with -c, see build.bat -h
exit /b 1

:nbneeds
echo ERROR: -n needs -i and/or -r, see build.bat -h
exit /b 1

:nbmissing
echo ERROR: -n needs an existing build, build\VVVVVV.exe not found.
exit /b 1

:nopatherr
echo ERROR: -s needs a path, see build.bat -h
exit /b 1

:nogame
echo ERROR: VVVVVV folder not found in the Steam libraries, expected steamapps\common\VVVVVV.
exit /b 1

:instfailed
echo ERROR: could not copy to %GAME%, close the game and retry.
exit /b 1

:cleanfailed
echo ERROR: could not delete the build folder, close programs using it.
exit /b 1

:failed
echo.
echo ERROR: build failed.
exit /b 1

:finddir
set "GAME="
set "STEAM="
for /f "tokens=2,*" %%A in ('reg query "HKCU\Software\Valve\Steam" /v SteamPath 2^>nul ^| findstr /i SteamPath') do set "STEAM=%%B"
if defined STEAM set "STEAM=%STEAM:/=\%"
if not defined STEAM set "STEAM=%ProgramFiles(x86)%\Steam"
call :trylib "%STEAM%"
if defined GAME exit /b 0
if not exist "%STEAM%\steamapps\libraryfolders.vdf" exit /b 0
for /f "usebackq tokens=1,*" %%A in (`findstr /c:"path" "%STEAM%\steamapps\libraryfolders.vdf"`) do if /i "%%~A"=="path" call :trylib "%%~B"
exit /b 0

:trylib
if defined GAME exit /b 0
set "LIB=%~1"
set "LIB=%LIB:\\=\%"
if exist "%LIB%\steamapps\common\VVVVVV\" set "GAME=%LIB%\steamapps\common\VVVVVV"
exit /b 0
