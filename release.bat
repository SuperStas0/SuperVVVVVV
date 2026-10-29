@echo off
setlocal
cd /d "%~dp0"

rem Packs the result of build.bat into release\SuperVVVVVV.zip

if not exist build\VVVVVV.exe goto :nobuild
if not exist build\SDL2.dll goto :nobuild
if not exist README.md goto :nofile

set "OUT=release\SuperVVVVVV"
if exist release rmdir /s /q release
mkdir "%OUT%"

copy /y build\VVVVVV.exe "%OUT%\SuperVVVVVV.exe" >nul
copy /y build\SDL2.dll "%OUT%\SDL2.dll" >nul
copy /y README.md "%OUT%\SuperVVVVVV-README.md" >nul

powershell -NoProfile -Command "Compress-Archive -Path 'release\SuperVVVVVV\*' -DestinationPath 'release\SuperVVVVVV.zip' -Force"
if errorlevel 1 goto :failed

echo.
echo Done: release\SuperVVVVVV.zip
exit /b 0

:nobuild
echo ERROR: build\VVVVVV.exe or build\SDL2.dll missing. Run build.bat first.
exit /b 1

:nofile
echo ERROR: README.md missing from the repo root.
exit /b 1

:failed
echo ERROR: creating the zip failed.
exit /b 1
