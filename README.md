# SakuraTree

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/tobiasanker/SakuraTree?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/tobiasanker/SakuraTree?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/tobiasanker/SakuraTree?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

<p align="center">
  <img src=".pictures/logo.png?raw=true" alt="Logo"/>
</p>


## Description

This is/should become a simple-to-use and fast automation tool to deploy tools and files on multiple nodes. 

The primary goals are:

* easy to use
* fast execution
* fast debugging


## Documentation

Documentation for version 0.2.0: 

https://gitlab.com/tobiasanker/Sakura-Project-Documentation/-/jobs/394246432/artifacts/browse

Documentation for latest master-version: 

https://gitlab.com/tobiasanker/Sakura-Project-Documentation/builds/artifacts/master/browse?job=build

**IMPORTANT**: The master-version of the documentation actually differs from the master of the code, because the code has a bit bigger restructure for the multi-node support.

## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### required tools to build

name | repository | version | task
--- | --- | --- | ---
g++ | g++ | >= 6.0 | Compiler for the C++ code.
make | make | >= 4.0 | process the make-file, which is created by qmake to build the programm with g++
qmake | qt5-qmake | >= 5.0 | This package provides the tool qmake, which is similar to cmake and create the make-file for compilation.
FLEX | flex | >= 2.6 | Build the lexer-code for all used parser.
GNU Bison | bison | >= 3.0 | Build the parser-code together with the lexer-code.
xxd | xxd | >= 1.10 | converts text files into source code files

Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ make qt5-qmake bison flex xxd
```

#### required official libraries

repository-name | version | task
--- | --- | ---
libssl-dev | >= 1.1.0l | For tls-encrypted data-transfer.
libboost-filesystem-dev | >= 1.60 | Use for file-interactions like for example listing files in a directory or check if a path exist.

Installation on Ubuntu/Debian:

```bash
sudo apt-get install libssl-dev libboost-filesystem-dev
```

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsunemimi-repositories

These repositories will be downloaded automatically by the build script of the tool itself (see next section). This list here is only as information to give an overview of all used Kitsunemimi libraries in this project.

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | master | https://github.com/tobiasanker/libKitsunemimiCommon.git
libKitsunemimiPersistence | master | https://github.com/tobiasanker/libKitsunemimiPersistence.git
libKitsunemimiArgs | master| https://github.com/tobiasanker/libKitsunemimiArgs.git
libKitsunemimiJson | master | https://github.com/tobiasanker/libKitsunemimiJson.git
libKitsunemimiJinja2 | master | https://github.com/tobiasanker/libKitsunemimiJinja2.git
libKitsunemimiIni | master | https://github.com/tobiasanker/libKitsunemimiIni.git
libKitsunemimiNetwork | master | https://github.com/tobiasanker/libKitsunemimiNetwork.git
libKitsunemimiProjectNetwork | master | https://github.com/tobiasanker/libKitsunemimiProjectNetwork.git
libKitsunemimiSakuraParser | master | https://github.com/tobiasanker/libKitsunemimiSakuraParser.git
libKitsunemimiSakuraNetwork | master | https://github.com/tobiasanker/libKitsunemimiSakuraNetwork.git

### build programm

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't require sudo, because you have to install the required tools manually beforehand, for example via apt. But if other projects of mine are required, it downloads them from GitHub and builds them in the correct version too. This script is also used by the CI pipeline, so it's tested with every commit.

Before running the build-script:

```bash
.
└── SakuraTree
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsunemimiCommon
│   │   └── ...
│   ├── libKitsunemimiPersistence
│   │   └── ...
│   └── ...
│
├── libKitsunemimiCommon
│   └── ...
├── libKitsunemimiPersistence
│   └── ...
├── ...
│
├── SakuraTree
│   ├── build.sh
│   └── ...
│
└── result
    └─── SakuraTree
```

It automatically creates a `build` and `result` directory in the directory where you have cloned the project. At first it builds all into the build directory and after all build steps are finished, it copies the final binary into the result directory.

The build script links all Kitsunemimi libraries statically into the final binary.

Tested on Debian and Ubuntu. If you use CentOS, Arch, etc and the build script fails on your machine, then please write me a message or file a GitHub issue and I will try to fix the script.


## Simple example

The following example is not very usefull, but should give a first impression of the file-syntax, which is processed by the tool. See full syntax explaination in section [Documentation](#documentation).


```c++
["test example file"]

- packages = [ "nano", "vim" ]
- path = "/tmp/test_file"
- test_output = ""


for(package : packages)
{
    print("print packages-names")
    - output = package
}


apt("update and install")
-> update
-> present:
- packages = packages


if(packages.size() == 2)
{
    ini_file("get value from a test-ini-file")
    - file_path = path
    - group = "DEFAULT"
    - entry = "asdf"

    -> read:
        - blossom_output >> test_output
    -> set:
        - value = "123456789"

    print("print init-file-output")
    - first_try = test_output
}
```


## Contributing

Please give me as many inputs as possible: feature suggestions, bugs, bad code style, bad documentation, bad spelling and so on. 

I know that my code is far away from being perfect. Its fast, but has only a little amount of functionality and, I'm sure, that it still has a huge amount of unhandled error-cases and bugs. This is mostly the result of working alone for this project. Beside this, I have unfortunately nearly no knowledge, how to work in an open-source project. So I would be grateful for any help. :)

Feel free to contact me under tobias.anker@kitsunemimi.moe

## License

This project is licensed under the Apache License Version 2.0 - see the [LICENSE](LICENSE) file for details
