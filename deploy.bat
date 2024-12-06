@echo off
rem This file is generated from deploy.pbat, all edits will be lost
set PATH=C:\msys64\ucrt64\bin;C:\Program Files\CMake\bin;%LOCALAPPDATA%\Programs\Python\Python313;%LOCALAPPDATA%\Programs\Python\Python313\Scripts;C:\Python313;C:\Python313\Scripts;C:\Program Files\Meson;C:\Miniconda3;C:\Miniconda3\Scripts;%USERPROFILE%\Miniconda3;%USERPROFILE%\Miniconda3\Scripts;%PATH%
where mugideploy > NUL 2>&1 || pip install mugideploy
where ninja > NUL 2>&1 || pip install ninja
call build-msys2
mugideploy collect --bin Release-msys2\mugi-grep.exe --zip