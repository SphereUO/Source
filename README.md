# SphereUO Source
Ultima Online game server
<br>
[![GitHub license](https://img.shields.io/github/license/SphereUO/Source?color=blue)](https://github.com/SphereUO/Source/blob/main/LICENSE)
<br>
[![Build](https://github.com/SphereUO/Source/actions/workflows/build.yml/badge.svg)](https://github.com/SphereUO/Source/actions/workflows/build.yml)
<br>
[![GitHub issues](https://img.shields.io/github/issues/SphereUO/Source.svg)](https://github.com/SphereUO/Source/issues)
[![GitHub last commit](https://img.shields.io/github/last-commit/SphereUO/Source.svg)](https://github.com/SphereUO/Source/)
[![GitHub repo size](https://img.shields.io/github/repo-size/SphereUO/Source.svg)](https://github.com/SphereUO/Source/)
[![GitHub stars](https://img.shields.io/github/stars/SphereUO/Source?logo=github)](https://github.com/SphereUO/Source/stargazers)


### Social
[![Discord](https://img.shields.io/discord/1036751214655848548?logo=discord&style=social)](https://discord.gg/wMqNup5jhE)


### Scripts Pack
Required for run server.<br>
https://github.com/SphereUO/Scripts


## Running

### Required libraries (Windows):
* `libmysql.dll`: Placed on /DLLs/32/libmysql.dll for 32 bits builds or in /DLLs/64/libmysql.dll for 64 bits builds.
* `dbghelp.dll`: Newer OS versions have it by default on system folders so don't mess with it, but for some old OS you may need it, 
 so there is an old one (32 bits) in /dlls/dbghelp.dll.

### Required libraries (Linux):

#### Ubuntu 14.x to 16.x
Install MySQL client: `sudo apt-get install libmysql++`

#### Debian 9
Install MySQL client (the default repository supports only the MariaDB client, so we need to install MySQL manually):<br>
For 64 bits builds:<br>
```
wget http://security.debian.org/debian-security/pool/updates/main/m/mysql-5.5/libmysqlclient18_5.5.62-0+deb8u1_amd64.deb
sudo dpkg -i libmysqlclient18_5.5.62-0+deb8u1_amd64.deb
```
For 32 bits builds:<br>
```
wget http://security.debian.org/debian-security/pool/updates/main/m/mysql-5.5/libmysqlclient18_5.5.62-0+deb8u1_i386.deb
sudo dpkg -i libmysqlclient18_5.5.62-0+deb8u1_i386.deb
```

#### CentOS 6 / 7 - Red Hat 6 / 7 - Fedora 26+
If you're using CentOS, Red Hat or Fedora, nowadays the default package repository only supports MariaDB instead of MySQL, so we need to add the repo for MySQL:<br>
* For CentOS 6 and Red Hat 6: `sudo rpm -Uvh https://dev.mysql.com/get/mysql57-community-release-el6-9.noarch.rpm`<br>
* For CentOS 7 and Red Hat 7: `sudo rpm -Uvh https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm`<br>
* For Fedora 26: `sudo rpm -Uvh https://dev.mysql.com/get/mysql57-community-release-fc26-10.noarch.rpm`<br>
* For Fedora 27: `sudo rpm -Uvh https://dev.mysql.com/get/mysql57-community-release-fc27-10.noarch.rpm`<br>
Then install MySQL client via yum (CentOS or RH) or dnf (Fedora): `mysql-community-libs`<br>

## Building

### Generating the project files
The compilation of the code is possible only using recent compilers, since C++17 features are used: Visual Studio 2017 or 2015 Update 3, GCC 7.1 and later (even if GCC 6 can work, 7 is reccomended), MinGW distributions using GCC 7.1 and later (like nuwen's).<br>
You need to build makefiles (and project files if you wish) with CMake for both Linux (GCC) and Windows (MSVC and MinGW).<br>
Both 32 and 64 bits compilation are supported.<br>
No pre-built project files included.<br>
Does CMake give you an error? Ensure that you have Git installed, and if you are on Windows ensure also that the Git executable was added to the PATH environmental variable
 (you'll need to add it manually if you are using Git Desktop,
 <a href="https://stackoverflow.com/questions/26620312/installing-git-in-path-with-github-client-for-windows?answertab=votes#tab-top">here's a quick guide</a>).<br>

#### Toolchains and custom CMake variables
When generating project files, if you don't specify a toolchain, the CMake script will pick the 32 bits one as default.<br>
How to set a toolchain:
* Via CMake GUI: when configuring for the first time the project, choose "Specify toolchain file for cross-compiling", then on the next step you'll be allowed to select the toolchain file
* Via CMake CLI (command line interface): pass the parameter `-DCMAKE_TOOLCHAIN_FILE="..."`
<br>When using Unix Makefiles, you can specify a build type by setting (also this via GUI or CLI) `CMAKE_BUILD_TYPE="build"`, where build is Nightly, Debug or Release. If the build type
 was not set, by default the makefiles for all of the three build types are generated.<br>
<br />
You can also add other compiler flags, like optimization flags, with the custom variables C_FLAGS_EXTRA and CXX_FLAGS_EXTRA.<br>

Example of CMake CLI additional parameters:<br>
```
-DC_FLAGS_EXTRA="-mtune=native" -DCXX_FLAGS_EXTRA="-mtune=native"
```
(Use the -mtune=native flag only if you are compiling on the same machine on which you will execute Sphere!)

Example to build makefiles on Linux for a 64 bits Nightly version, inside the "build" directory (run it inside the project's root folder, not inside "src"):<br>
```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../src/cmake/toolchains/Linux-GNU-x86_64.cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Nightly" --build ./ ../src
```

### Compiling

#### Installing the required packages on Linux
Building will require more packages than the ones needed to run Sphere.
 
##### Ubuntu 14.x to 16.x and Debian 9
Install these additional packages: `sudo apt-get install git libmysql++-dev libmysqld-dev libmysqlclient-dev`<br>
If you are on a 64 bits architecture but you want to compile (or execute) a 32 bits binary, you will need to
 install the MySQL packages adding the postfix `:i386` to each package name.

##### CentOS 6 / 7 - Red Hat 6 / 7 - Fedora 26
Then install these additional packages via yum (CentOS or RH) or dnf (Fedora): `git gcc-c++ glibc-devel mysql-community-devel`<br>
<br>If you are on a 64 bits architecture but you want to compile (or execute) a 32 bits binary, you will need to install the appropriate gcc package
 and to install the MySQL packages adding the postfix `.i686` to each package name.

#### Compiling on Linux
Just run the `make` command inside the `build` folder. You can pass the -jX argument (`make -jX`, where X is a number) to speed up the compilation and split the work between X threads.
 
#### Extra: compiling with Clang (if you wish to try)
At the moment, Clang was tested only on Windows and in combination with Visual Studio 2017.<br>

##### On Windows
1. Install Clang for LLVM 6.0 (<a href="http://releases.llvm.org/download.html">here</a>) and select the option to add the bin folder to the PATH environmental variable.
2. Install via the Visual Studio installer the package "Clang/C2".
3. Install the 3rd party Visual Studio 2017 toolset for Clang from <a href="https://github.com/arves100/llvm-vs2017-integration">here</a>.
4. Run CMake using the Visual Studio 15 2017 (Win64) generator, "Windows-clang-MSVC-*.cmake" toolchain and toolset "LLVM-vs2017".


## Thanks
- SphereUO