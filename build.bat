@echo off
rem This file is generated from build.pbat, all edits will be lost
set PATH=C:\Program Files\CMake\bin;%LOCALAPPDATA%\Programs\Python\Python313;%LOCALAPPDATA%\Programs\Python\Python313\Scripts;C:\Python313;C:\Python313\Scripts;C:\mingw1310_64\bin;C:\Qt\6.8.1\mingw_64\bin;%PATH%
where ninja > NUL 2>&1 || pip install ninja
set BUILD_TYPE=Release
if /I "%1"=="debug" set BUILD_TYPE=Debug
echo BUILD_TYPE %BUILD_TYPE%
pushd %~dp0
    if not exist "%BUILD_TYPE%" mkdir "%BUILD_TYPE%"
    pushd %BUILD_TYPE%
        cmake -G Ninja -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ..
        cmake --build .
    popd
popd