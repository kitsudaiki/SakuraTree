# SakuraTree

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/tobiasanker/SakuraTree?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/tobiasanker/SakuraTree?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/tobiasanker/SakuraTree?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

WIP: This is/should become a simple-to-use and fast automation tool to deploy tools and files multiple nodes.

Primary goals are:

- easy to use
- fast execution
- fast debugging

If you are interested in this project, then keep an eye on it or feel free to help me.

## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### required tools to build

name | repository | version | task
--- | --- | --- | ---
g++ | g++ | 6.3.0 | Compiler for the C++ code.
qmake | qt5-qmake | 5.7.1 | This package provides the tool qmake, which is similar to cmake and create the make-file for compilation.
FLEX | flex | 2.6.1 | Build the lexer-code for all used parser.
GNU Bison | bison | 3.0.4 | Build the parser-code together with the lexer-code.

Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ qt5-qmake bison flex
```

#### required official libraries

repository-name | version | task
--- | --- | ---
libboost-program-options-dev | 1.62 | Provides the argument-parser for the CLI input.
libboost-filesystem-dev | 1.62 | Use for file-interactions like for example listing files in a directory or check if a path exist.
libsqlite3-dev | 3.16.2 | Privides a Interaction with SQLite-Databases. Its a requirement of the used library libKitsunemimiPersistence, but its not used by this project now.

Installation on Ubuntu/Debian:

```bash
sudo apt-get install libboost-program-options-dev libboost-filesystem-dev libsqlite3-dev
```

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.9.0 |  https://github.com/tobiasanker/libKitsunemimiCommon.git
libKitsunemimiPersistence | v0.6.1 |  https://github.com/tobiasanker/libKitsunemimiPersistence.git
libKitsunemimiJson | v0.9.0 |  https://github.com/tobiasanker/libKitsunemimiJson.git
libKitsunemimiJinja2 | v0.6.2 |  https://github.com/tobiasanker/libKitsunemimiJinja2.git
libKitsunemimiIni | v0.3.2 |  https://github.com/tobiasanker/libKitsunemimiIni.git
libKitsunemimiSakuraParser | v0.2.0 |  https://github.com/tobiasanker/libKitsunemimiSakuraParser.git


### build programm

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

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

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the final binary into the `result`-directory. 

The build-script links all Kitsunemimi-libraries static into the final binary.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.

## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the Apache License Version 2.0 - see the [LICENSE](LICENSE) file for details
