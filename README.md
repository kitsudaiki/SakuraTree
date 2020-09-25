# SakuraTree

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/kitsudaiki/SakuraTree?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/kitsudaiki/SakuraTree?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/kitsudaiki/SakuraTree?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

<p align="center">
  <img src=".pictures/logo.png?raw=true" alt="Logo"/>
</p>


## Documentation

Documentation for latest master-version: 

https://gitlab.com/kitsudaiki/Sakura-Project-Documentation/builds/artifacts/master/browse?job=build

For version 0.4.0:

https://files.kitsunemimi.moe/docs/SakuraTree-Documentation_0_4_0.pdf


## Description

This is/should become a simple-to-use and fast automation tool to deploy tools and files on multiple nodes. 

Core Features
-------------

**Be aware, that most of the planed core-features are not implemented at the moment**


**simple language**

Chef contains the full power of Ruby. That brings many functionalities, but makes it hard to write and to understand. Ansible is more easy with its YAML files, but this makes things like loops and conditionals really cumbersome. So I wanted to create my own description language, which is designed for automation. Based on the cherry trees the structure is: seeds create trees and trees own blossoms.

**easily multi-threading**

*current state: implemented*

I often encountered things while deploying one or multiple nodes, which could be done parallel within one node. In Ansible and Chef this is not really intended. This is a big time loss. So I designed the language and the current implementation, so that you can easily create multiple threads and decide which tasks should run parallel and even loops can be spread over multiple threads.

**hierarchical client-server-architecture**

*current state: prototype (was prototypical Implemented in 0.3.0, was removed with 0.4.0 again. Will be reimplemented again later)*

Similar to Chef, this tool has a client-server-architecture. But this was not enough. Each SakuraTree process can work as client and/or server. So you can create a hierarchical structure of clients and servers for faster data transfer. For example let's say you have a very big image and want to copy it to each node of a deployment with hundreds of nodes. With the ability to spawn new clients and servers within the deployment, you can copy this to one node, which in turn copies the file to two other nodes and these nodes again copy it to the next four nodes and so on.

**live-debugging**

*current state: not implemented now*

Debugging in Ansible and Chef is really slow. When you run into an error and you want to test your fix, you have to run all from the beginning. The changes which were already done in the last run will not be done again but even so the new run can take minutes or hours, until it comes to the point which you attempted to fix. Furthermore, in case the fix was not correct, you have to run this again and wait a massive amount of time and try to process other tasks while waiting. So the plan is that each node stores its process in a local sqlite database to record which task was done and what the input and output was. So after a bugfix, the old process could continue at the point where it failed in the last run.

**rollback-function**

*current state: not implemented now*

For each action a reverse action should be defined to rollback as much as possible. This is also necessary for the live-debugging in the case, that some tasks are deleted or moved while fixing a problem.

**additional command output**

*current state: not implemented now*

When working with an automation tool it's easy to forget each step to create the same setup without the tool. So I'd like a feature to generate a simple manual out of the used scripts with all necessary command-line calls, some simple comments and step descriptions. This could also help debugging.

**graphical monitoring**

*current state: not implemented now*

This is a feature which may be added when the rest works fine and is ready for productive use. Plain text output of deploying is sometimes hard to follow, especially when having many parallel tasks on multiple nodes. Therefore it would be nice to have a monitoring to follow the process, which supports debugging capabilities.


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

Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ make qt5-qmake bison flex
```

#### required official libraries

repository-name | version | task
--- | --- | ---
libboost-filesystem-dev | >= 1.60 | Use for file-interactions like for example listing files in a directory or check if a path exist.

Installation on Ubuntu/Debian:

```bash
sudo apt-get install libboost-filesystem-dev
```

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsunemimi-repositories

These repositories will be downloaded automatically by the build script of the tool itself (see next section). This list here is only as information to give an overview of all used Kitsunemimi libraries in this project.

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.15.1 | https://github.com/kitsudaiki/libKitsunemimiCommon.git
libKitsunemimiPersistence | v0.10.0 | https://github.com/kitsudaiki/libKitsunemimiPersistence.git
libKitsunemimiArgs | master | https://github.com/kitsudaiki/libKitsunemimiArgs.git
libKitsunemimiJson | v0.10.4 | https://github.com/kitsudaiki/libKitsunemimiJson.git
libKitsunemimiJinja2 | v0.8.0 | https://github.com/kitsudaiki/libKitsunemimiJinja2.git
libKitsunemimiIni | v0.4.5 | https://github.com/kitsudaiki/libKitsunemimiIni.git
libKitsunemimiSakuraLang | master | https://github.com/kitsudaiki/libKitsunemimiSakuraLang.git

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

The following example is not very usefull, but should give a first impression of the file-syntax, which is processed by the tool. See full syntax explanation in section [Documentation](#documentation).


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

    print("print ini-file-output")
    - value = test_output
}
```


## Contributing

Please give me as many inputs as possible: feature suggestions, bugs, bad code style, bad documentation, bad spelling and so on. 

I know that my code is far away from being perfect. Its fast, but has only a little amount of functionality and, I'm sure, that it still has a huge amount of unhandled error-cases and bugs. This is mostly the result of working alone for this project. Beside this, I have unfortunately nearly no knowledge, how to work in an open-source project. So I would be grateful for any help. :)

Feel free to contact me under tobias.anker@kitsunemimi.moe

## License

This project is licensed under the Apache License Version 2.0 - see the [LICENSE](LICENSE) file for details
