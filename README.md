# Mugi-grep

![mugi-grep](https://mugiseyebrows.github.io/img/mugi-grep.png)

It is like `find some/path -iname foo -exec grep bar {} \; | grep -v baz` but with sessions and history.

# Build

## Windows

1) Install qt5 from [www.qt.io](https://www.qt.io/download) ([direct link](https://download.qt.io/official_releases/qt/5.12/5.12.0/qt-opensource-windows-x86-5.12.0.exe))
2) Clone sources and build using qtcreator or cmd

```bash
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
```