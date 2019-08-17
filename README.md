# Mugi-grep

## Problem

Suppose you don't have search utility in your editor that satisfies all your needs. But you're clever so at first you

`grep something *.cpp *.h`

and then you

`find . -iname '*.cpp' -or -iname '*.h' -exec grep -Hn something {} \;`

and then you

`find . \( -iname '*.cpp' -or -iname '*.h' \) -exec grep -Hn something {} \;`

and then you

`find . \( -iname '*.cpp' -or -iname '*.h' \) -exec grep -Hn something {} \; | grep -v butNotThat`

and then you need to repeat previous search, but also keep current results so you open new tab or pane

and then you wish you could use results to click-jump to specific file on specific line in your editor of choice

and then you wish you could `sed -i` on results without breaking everything

## Solution

Struggle no more, _mugi-grep_ is here to help. Look at that beauty!

### Search action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-search.png" width="600"/>

### Replace action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-replace2.png width="600"/>

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