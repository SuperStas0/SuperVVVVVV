@echo off
setlocal
cd /d "%~dp0"

rem Packs the result of build.bat into release\SuperVVVVVV.zip

if not exist build\VVVVVV.exe goto :nobuild
if not exist build\SDL2.dll goto :nobuild
if not exist LICENSE.md goto :nofile
if not exist THIRD_PARTY_LICENSES.txt goto :nofile
if not exist README.md goto :nofile

set "OUT=release\SuperVVVVVV"
if exist release rmdir /s /q release
mkdir "%OUT%\SuperVVVVVV-licenses"

copy /y build\VVVVVV.exe "%OUT%\SuperVVVVVV.exe" >nul
copy /y build\SDL2.dll "%OUT%\SDL2.dll" >nul
copy /y README.md "%OUT%\SuperVVVVVV-README.md" >nul
copy /y LICENSE.md "%OUT%\SuperVVVVVV-licenses\LICENSE.md" >nul
copy /y THIRD_PARTY_LICENSES.txt "%OUT%\SuperVVVVVV-licenses\THIRD_PARTY_LICENSES.txt" >nul

powershell -NoProfile -Command "Compress-Archive -Path 'release\SuperVVVVVV\*' -DestinationPath 'release\SuperVVVVVV.zip' -Force"
if errorlevel 1 goto :failed

echo.
echo Done: release\SuperVVVVVV.zip
exit /b 0

:nobuild
echo ERROR: build\VVVVVV.exe or build\SDL2.dll missing. Run build.bat first.
exit /b 1

:nofile
echo ERROR: README.md, LICENSE.md or THIRD_PARTY_LICENSES.txt missing from the repo root.
exit /b 1

:failed
echo ERROR: creating the zip failed.
exit /b 1
