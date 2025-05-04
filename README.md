# Mugi-grep

Interactive and visual `grep` and `sed` tool.

## Search action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-search.png" width="600"/>

## Replace action
<img src="https://mugiseyebrows.github.io/img/mugi-grep-replace2.png" width="600"/>

# Build

## Windows Visual Studio

1) Install qt6
2) Clone sources

```bash
git clone git@github.com:mugiseyebrows/mugi-grep.git
```

3) Open `x64 Native Tools Command Prompt`
5) Navigate to sources `cd "%USERPROFILE%\mugi-grep"`
6) Create `build` directory and build

```bash
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Windows MinGW

1) Install qt6
2) Clone sources

```bash
git clone git@github.com:mugiseyebrows/mugi-grep.git
```

3) Add gcc, cmake and ninja to `%PATH%`
4) Create `build` directory and build

```bash
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Linux (Ubuntu) GCC 

```bash
# install toolchain and qt
sudo apt update
sudo apt install build-essential qt6-base-dev ninja-build cmake git
# clone sources
git clone git@github.com:mugiseyebrows/mugi-grep.git
cd mugi-grep
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
# installs to /usr/local/bin
sudo cmake --install .
# creates desktop file, copies icons
python3 desktop.py 
```

# Binaries

Compiled windows binaries with all dependencies are available in [releases tab](https://github.com/mugiseyebrows/mugi-grep/releases)

# Contribute

You are welcome to submit issues questions and pull requests.