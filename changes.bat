@echo off
setlocal
cd /d "%~dp0"

set "TAG=2.4.4"
set "MODE=file"
set "FULL="
set "BRANCH=compare-vanilla"
set "OUT=changesFromVanilla.txt"
set "SRC=desktop_version/src"
set "VANILLA_URL=https://github.com/TerryCavanagh/VVVVVV.git"

:args
if "%~1"=="" goto :args_done
if /i "%~1"=="-h" goto :help
if /i "%~1"=="-b" set "MODE=branch" & shift & goto :args
if /i "%~1"=="-d" set "MODE=delete" & shift & goto :args
if /i "%~1"=="-f" set "FULL=1" & shift & goto :args
if /i "%~1"=="-t" goto :arg_tag
echo ERROR: unknown option %~1, see -h
exit /b 1

:arg_tag
if "%~2"=="" (
    echo ERROR: -t needs a tag, see -h
    exit /b 1
)
set "TAG=%~2"
shift
shift
goto :args

:args_done

git --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: git was not found
    exit /b 1
)
git rev-parse --is-inside-work-tree >nul 2>&1
if errorlevel 1 (
    echo ERROR: this folder is not a git repository
    exit /b 1
)

if "%MODE%"=="delete" goto :delete

set "SCOPE=%SRC%/"
set "PATHSPEC=-- %SRC%/"
if defined FULL (
    set "SCOPE=all files"
    set "PATHSPEC="
)

git remote get-url vanilla >nul 2>&1
if errorlevel 1 (
    echo Adding remote vanilla: %VANILLA_URL%
    git remote add vanilla %VANILLA_URL%
    if errorlevel 1 exit /b 1
)

git rev-parse -q --verify "refs/tags/%TAG%" >nul 2>&1
if errorlevel 1 (
    echo Fetching tag %TAG% from vanilla...
    git fetch vanilla tag %TAG% --no-tags --quiet
    if errorlevel 1 (
        echo ERROR: could not fetch tag %TAG%
        exit /b 1
    )
)

if "%MODE%"=="branch" goto :branch

echo Vanilla %TAG% to HEAD, %SCOPE% > "%OUT%"
echo. >> "%OUT%"
git diff --stat=120 %TAG% HEAD %PATHSPEC% >> "%OUT%"
echo. >> "%OUT%"
git diff --no-color %TAG% HEAD %PATHSPEC% >> "%OUT%"
if errorlevel 1 exit /b 1
echo Wrote %OUT% (%SCOPE%). Only committed changes are included.
exit /b 0

:branch
rem The commit "Compare against vanilla" has exactly the files of HEAD. Its parent
rem is a vanilla snapshot, so the diff of that commit is vanilla -> your files.
rem With -f the parent is the whole vanilla tag. Without it, the parent is your
rem HEAD with only %SRC%/ taken from vanilla, so other files do not show up.
set "BASE_TREE="
if defined FULL (
    for /f %%T in ('git rev-parse %TAG%:') do set "BASE_TREE=%%T"
    goto :branch_base
)
set "GIT_INDEX_FILE=%TEMP%\vvvvvv-compare-index"
if exist "%GIT_INDEX_FILE%" del "%GIT_INDEX_FILE%"
git read-tree HEAD || goto :branch_failed
git rm -r -q --cached %SRC% || goto :branch_failed
git read-tree --prefix=%SRC%/ %TAG%:%SRC% || goto :branch_failed
for /f %%T in ('git write-tree') do set "BASE_TREE=%%T"
set "GIT_INDEX_FILE="

:branch_base
if not defined BASE_TREE goto :branch_failed

set "BASE_COMMIT="
for /f %%C in ('git commit-tree %BASE_TREE% -m "Vanilla %TAG%, %SCOPE%"') do set "BASE_COMMIT=%%C"
if not defined BASE_COMMIT goto :branch_failed

set "HEAD_TREE="
for /f %%T in ('git rev-parse HEAD:') do set "HEAD_TREE=%%T"
set "COMPARE_COMMIT="
for /f %%C in ('git commit-tree %HEAD_TREE% -p %BASE_COMMIT% -m "Compare against vanilla %TAG%, %SCOPE%"') do set "COMPARE_COMMIT=%%C"
if not defined COMPARE_COMMIT goto :branch_failed

git branch -f %BRANCH% %COMPARE_COMMIT%
if errorlevel 1 goto :branch_failed

echo Created branch %BRANCH% (%SCOPE%). It has the same files as HEAD, so switching to it changes nothing on disk.
echo In GitHub Desktop: switch to %BRANCH%, open History, select the top commit.
echo Switch back afterwards and run: changes.bat -d
exit /b 0

:branch_failed
set "GIT_INDEX_FILE="
echo ERROR: could not create the branch
exit /b 1

:delete
git branch -D %BRANCH%
exit /b %errorlevel%

:help
echo Usage: changes.bat [-f] [-b] [-d] [-t tag] [-h]
echo   no option  write %OUT%: the changes in %SRC%/ from vanilla to the last commit
echo   -b         create the branch %BRANCH% to browse the changes in GitHub Desktop
echo   -d         delete the branch %BRANCH%
echo   -f         full: all files, not only %SRC%/ ^(for the file and for -b^)
echo   -t tag     vanilla tag to compare with, default %TAG%
echo   -h         show this help
echo Commit your changes first, only HEAD is compared.
exit /b 0
