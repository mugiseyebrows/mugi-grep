# Mugi-grep

Interactive and visual `grep` and `sed` tool.

## Search action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-search.png" width="600"/>

## Replace action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-replace2.png" width="600"/>

# Build

## Windows MSVC (Visual Studio)

### a) Build in shell

1) Install qt5 from [www.qt.io](https://www.qt.io/download) 
2) Clone sources with `Git Bash`  `(Start menu: Git -> Git Bash)`

```bash
git clone git@github.com:mugiseyebrows/mugi-grep.git
```

3) Open `x64 Native Tools Command Prompt`
4) Run `C:\qt\5.15.1\msvc2019_64\bin\qtenv2.bat`
5) Navigate to sources `cd %USERPROFILE%\mugi-grep`
6) Build 

```bash
qmake && make release
```

### b) Build in qtcreator

1) Install qt5 from [www.qt.io](https://www.qt.io/download) 
2) Clone sources with `Git Bash`  `(Start menu: Git -> Git Bash)`

```bash
git clone git@github.com:mugiseyebrows/mugi-grep.git
```

3) Open QtCreator `(Start menu: Qt -> QtCreator)`
4) Configure kits `(Top menu: Tools -> Options -> Kits)`
5) Open `mugi-grep.pro` file `(Top menu: File -> Open File Or Project)`
6) Build `(Top menu: Build -> Build All Projects)`




## Linux GCC (Ubuntu)

```bash
# install toolchain and qt
sudo apt install build-essential qtbase5-dev git
# clone sources
git clone git@github.com:mugiseyebrows/mugi-grep.git
cd mugi-grep
qmake -qt=5
make release
make clean
sudo make install # installs to /usr/local/bin
python desktop.py # copies icons, creates desktop file
```

# Binaries

Compiled windows binaries with all dependencies are available in [releases tab](https://github.com/mugiseyebrows/mugi-grep/releases)

# Contribute

You are welcome to submit issues questions and pull requests.