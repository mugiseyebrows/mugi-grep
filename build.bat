@echo off
rem This file is generated from build.pbat, all edits will be lost
set PATH=C:\Windows\System32;C:\Program Files\7-Zip;C:\Qt\6.10.0\mingw_64\bin;C:\mingw1310_64\bin;C:\zlib-1.3.1\bin;C:\Miniconda3;C:\Miniconda3\Scripts;%USERPROFILE%\Miniconda3;%USERPROFILE%\Miniconda3\Scripts;%LOCALAPPDATA%\Programs\Python\Python313;%LOCALAPPDATA%\Programs\Python\Python313\Scripts;C:\Python313;C:\Python313\Scripts;%PATH%
if exist C:\mingw1310_64\bin\gcc.exe goto mingw_end
if not exist winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z (
    echo downloading winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z
    curl -L -o winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-ucrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z
)
7z rn winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z mingw64 mingw1310_64
if not exist C:\mingw1310_64\bin\gcc.exe 7z x -y -oC:\ winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64ucrt-11.0.0-r5.7z
:mingw_end
if exist C:\Qt\6.10.0\mingw_64\bin\qmake.exe goto qt_end
if not exist Qt-6.10.0-mingw13.7z (
    echo downloading Qt-6.10.0-mingw13.7z
    curl -L -o Qt-6.10.0-mingw13.7z https://github.com/mugiseyebrows/build-qt/releases/download/6.10.0/Qt-6.10.0-mingw13.7z
)
7z x -y -oC:\Qt\6.10.0 Qt-6.10.0-mingw13.7z
:qt_end
if not exist Release mkdir Release
pushd Release
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
popd
where mugideploy > NUL 2>&1 || pip install mugideploy
mugideploy collect --bin Release\src\mugi-grep.exe --zip