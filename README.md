# Mugi-grep

## Problem

Suppose you don't have search utility in your editor that satisfies all your needs. But you're clever so at first you're like

`grep something *.cpp *.h`

and then you

`find . -iname '*.cpp' -or -iname '*.h' -exec grep -Hn something {} \;`

and then you

`find . \( -iname '*.cpp' -or -iname '*.h' \) -exec grep -Hn something {} \;`

and then you

`find . \( -iname '*.cpp' -or -iname '*.h' \) -exec grep -Hn something {} \; | grep -v butNotThat`

and then you need to repeat pre-previous search, but keep current results so you tmux (and then you can't easily scroll) or ctrl-shift-t and then you lost in ten tabs

and then you wish you could use results to click-junp to specific file on specific line in your editor of choice.

## Solution

Struggle no more, _mugi-grep_ is here to help. Look at that beauty!

![mugi-grep](https://mugiseyebrows.github.io/img/mugi-grep-1.0.1.png)

# Build

## Windows

1) Install qt5 from [www.qt.io](https://www.qt.io/download) ([direct link](https://download.qt.io/official_releases/qt/5.12/5.12.0/qt-opensource-windows-x86-5.12.0.exe))
2) Include compiler and qt path to `%PATH%` env variable
2) Clone sources and build

```bash
set PATH=C:\Qt5\5.11.1\mingw53_32\bin;C:\Qt5\Tools\mingw530_32\bin;%PATH%
git clone git@github.com:mugiseyebrows/mugi-grep.git
cd mugi-grep
qmake
make release
make clean
```

Compiled binaries with all dependencies are available in [releases tab](https://github.com/mugiseyebrows/mugi-grep/releases)

## Linux (Ubuntu)

```bash
sudo apt install build-essential qtbase5-dev git
git clone git@github.com:mugiseyebrows/mugi-grep.git
cd mugi-grep
qmake -qt=5
make
make clean
sudo make install # installs to /usr/local/bin
python desktop.py # copies icons, creates desktop file
```