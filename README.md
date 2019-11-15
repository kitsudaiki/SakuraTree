# SakuraTree

## Description

WIP: This is/should become a simple-to-use and fast automation tool to deploy tools and files multiple nodes.

Primary goals are:

- easy to use
- fast execution
- fast debugging

If you are interested in this project, then keep an eye on it or feel free to help me.

### Current state

It still at the beginning and there is much to do, so its not very stable and useful now. At the moment only the very basic structure exist. I wanted to bring it open-source as fast as possible, but for this I want to make sure that the core-functions works and is very flexible for the next updates. This results in some quick-and-dirty parts in the source code, which will be fixed in the next days.

### Why I created my own tool

My main-project (still private) consist of multiple components and is hard to test and debug without an automized deploy process. At work I had already to do with the tools Ansible and Chef, but I don't like any of them. They use, with Yaml in Ansible and Ruby in Chef, way too much generic languages, which makes it, at least for me, sometimes hard to understand. Beside this, the debugging is sooo horrible slow and I miss some basic features in both of them.

So I decided to create my own tool to try to make it better. Maybe there already exist some other tools like Ansible or Chef with better handling, but I don't have the motivation to test all existing tools. Beside this, this here is a good side-project for me, to work at this when ever I need some distance from my main-project to solve a problem. 

### Why I named it SakuraTree

I used Ansible and Chef as inspration for my naming. In Ansible there are roles and playbooks and in Chef are cookbooks and recipes. Because I like tree-structures and see the different tasks and threads while deploying in this structure too, I used the tree as name-base. Especially I love trees with cherry blossoms, like I saw in japan. `sakura` is the japanese name for cherry blossom and cherry tree is in japanese `sakura no ki`, but this as name is too generic. So I elected `SakuraTree` as name. This basically says the same, but is more unique and fits better as name. Additionally I'm used to create my tool names from a japanse name I like, together with a english word, which is related to the function of the tool and in this case here, the translation of sakura match perfectly too.

Based on this, the components in my tool are named `seed`, `tree`, `branch` and `blossom`. 


### Feature Overview (most of them are not implemented so far)

- **simple language** 

    **current state: basic version implemented, but some features are missing**

    Chef contains the full power of ruby. That brings many functionallity, but makes it hard to write and to understand. Ansible is more easy with its Yaml-files, but this makes things like loops and if-conditions really broken, in my opinion. So I wanted to create my own description language, which is designed for automation. See example for the current state below. Based the cherry trees the structure is: `seeds` create `trees`, `trees` own subtrees and `branches` and `branches` own `blossoms`.

- **easily multi-threading** 

    **current state: implemented, but not complete tested**

    I often had things while deploying one or multiple nodes, which could be done parallel within one node. In Ansible and Chef this is not really intended. This is big time-loss. So I designed the language and the current implementation, that you can easily create multiple threads and decide which tasks should run parallel. 

- **hierarchical client-server-architecture** 

    **current state: prototype**

    Similar to Chef, my tool have a client-server-architecture. But this was not enough for me. Each SakuraTree-process can work as client and/or server. So you can create a hierarchical structure of clients and servers for faster data-transfer. For example you have a very big image and want to copy it on each node of a deployment with hundreds of nodes. Which the ability to spawn new clients and server within the deployment, you copy this on one node, which one copy the file on two other nodes and these nodes again copy it one four nodes and so on. I know its more a nice-to-have feature, but not a must-have.

- **live-debugging** 

    **current state: not implemented now**

    Debugging in Ansible and Chef is really slow. When you run into an error and want to test you fix, you have to run all from the beginning. The changes which were already done in the last run, will not be done again, but even so the new run can take minutes or hours, until it comes to the point with fix. And when the fix was not current, you have to run this again and wait a massive amount of time and try to process other tasks while waiting. This is so stupid. So the plan is, that each node stores it process in a local sqlite-DB, to know which task was done with its input and output. So after a bugfix, the old process could continue at the point, where it failed in the last run.

- **barriers between threads and nodes** 

    **current state: not implemented now**

    When working with many parrallel tasks, some interaction between all threads is necessary. For example you have to deploy multiple tool and one central database, which is used by the tools. The tools need the credentials for the database access at a specific point in the rollout. In Chef you can split you deploy process in multiple stages. In my opinion this is stupid. I like barriers for this. For the example with the database, there should be a barrier defined in all subtrees, which used and create the database. So the threads who need the database access for the next steps should wait until the thread, who created the database, which finish with the creation. At this point it should send the credentials for the database within the barrier to all threads who need them.

- **rollback-function** 

    **current state: not implemented now**

    For each action a reverse action should be defined to rollback as much as possible. This is also necessary for the live-debugging in the case, that some task are deleted or moved while fixing a problem.

- **additional command output** 

    **current state: not implemented now**

    When working with automation tools its easy to forget to step to create the same setup without the tool. So I like the feature to generate a simple manual out of the seed-files with all necessary command-line calls and some simple comments and step descriptions. This could also help debugging. 

- **graphical monitoring** 

    **current state: not implemented now**

    This is a feature, when the rest works fine and is ready for productive. Plain text output which deploying is sometimes hard to follow, especially when having many parallel tasks on multiple nodes. So it would be nice to have a monitoring the follow the process, which support debugging. This feature is the main reason, why the networking stuff and the parser were put into separate libraries.


## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### Official repositories

paket | version
--- | ---
g++ | 6.3.0
qt5-qmake | 5.7.1
libssl1.1 | 1.1.0k
libssl-dev | 1.1.0k
libboost-program-options-dev | 1.62
libboost-filesystem-dev | 1.62
libsqlite3-dev | 3.16.2


This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | master |  https://github.com/tobiasanker/libKitsunemimiCommon.git
libKitsunemimiPersistence | master |  https://github.com/tobiasanker/libKitsunemimiPersistence.git
libKitsunemimiNetwork | master |  https://github.com/tobiasanker/libKitsunemimiNetwork.git
libKitsunemimiJson | master |  https://github.com/tobiasanker/libKitsunemimiJson.git
libKitsunemimiJinja2 | master |  https://github.com/tobiasanker/libKitsunemimiJinja2.git
libKitsunemimiIni | master |  https://github.com/tobiasanker/libKitsunemimiIni.git
libKitsunemimiProjectCommon | master |  https://github.com/tobiasanker/libKitsunemimiProjectCommon.git
libKitsunemimiSakuraParser | master |  https://github.com/tobiasanker/libKitsunemimiSakuraParser.git
libKitsunemimiSakuraNetwork | master |  https://github.com/tobiasanker/libKitsunemimiSakuraNetwork.git


### build library

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


## Simple example

This is only a simple first example, which I used for my current tests. It spawns an additional client on the local host and say the client to install and uninstall nano a few times. Basically I am happy with the fundamental structure of the language, but some details will be changed in the near future. 

**Further syntax description comes, when I have improved the functionallity and stability.**

Example-Tree:

```cpp
[test_tree]
- packages = nano

{
    seed(sakura)
    - address = "127.0.0.1"
    - port = 1337
    - ssh_user = neptune
    - ssh_port = 22
    - ssh_key = "~/.ssh/sakura_test" 
    {
        branch(install_branch)
        - packages = "{{packages}}"
    }
}

```

Example-Branch, which is called by the Tree.

```cpp
[install_branch]
- packages = {{}}

apt(apt_blossomX) 
-> update 
-> present:
   - names = "{{packages}}"

apt(apt_blossom1)
-> absent:
   - names = "{{packages}}"

apt(apt_blossom2)
-> absent:
   - names = "{{packages}}"

apt(apt_blossom3)
-> present:
   - names = "{{packages}}"

apt(apt_blossom4)
-> latest:
   - names = "{{packages}}"

apt(apt_blossom5)
-> present:
   - names = "{{packages}}"
```




## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the Apache License Version 2.0 - see the [LICENSE](LICENSE) file for details
