@echo off
setlocal
cd /d "%~dp0"

rem Packs the result of build.bat into release\SuperVVVVVV.zip

if not exist build\VVVVVV.exe goto :nobuild
if not exist README.md goto :nofile
if not exist autosplitter\LiveSplit.VVVVVV.asl goto :nofile

set "OUT=release\SuperVVVVVV"
if exist release rmdir /s /q release
mkdir "%OUT%"

copy /y build\VVVVVV.exe "%OUT%\VVVVVV.exe" >nul
if exist steam_api64.dll copy /y steam_api64.dll "%OUT%\steam_api64.dll" >nul
copy /y autosplitter\LiveSplit.VVVVVV.asl "%OUT%\LiveSplit.VVVVVV.asl" >nul
copy /y README.md "%OUT%\SuperVVVVVV-README.md" >nul

powershell -NoProfile -Command "Compress-Archive -Path 'release\SuperVVVVVV\*' -DestinationPath 'release\SuperVVVVVV.zip' -Force"
if errorlevel 1 goto :failed

echo.
echo Done: release\SuperVVVVVV.zip
if not exist steam_api64.dll echo Note: steam_api64.dll not found in the repo root, it is not in the zip. Steam achievements need it next to VVVVVV.exe.
exit /b 0

:nobuild
echo ERROR: build\VVVVVV.exe missing. Run build.bat first.
exit /b 1

:nofile
echo ERROR: README.md or autosplitter\LiveSplit.VVVVVV.asl missing from the repo.
exit /b 1

:failed
echo ERROR: creating the zip failed.
exit /b 1
