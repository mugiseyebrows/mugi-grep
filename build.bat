@echo off
rem This file is generated from build.pbat, all edits will be lost
set PATH=C:\Miniconda3;C:\Miniconda3\Scripts;%USERPROFILE%\Miniconda3;%USERPROFILE%\Miniconda3\Scripts;C:\Qt\5.15.2\mingw81_64\bin;C:\Qt\Tools\mingw810_64\bin;%PATH%
where mugideploy || pip install mugideploy
where aqt || pip install aqtinstall
if not exist "C:\Qt\5.15.2\mingw81_64\bin\qmake.exe" aqt install-qt windows desktop 5.15.2 win64_mingw81 -O C:\Qt
if not exist "C:\Qt\Tools\mingw810_64\bin\gcc.exe" aqt install-tool windows desktop tools_mingw qt.tools.win64_mingw810 -O C:\Qt
qmake
mingw32-make release -j4
mugideploy collect --app mugi-query --git-version --bin release\mugi-query.exe --plugins odbc mysql sqlite psql --zip