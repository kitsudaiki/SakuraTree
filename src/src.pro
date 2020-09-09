QT -= qt core gui

TARGET = SakuraTree
CONFIG += console
CONFIG += c++14

INCLUDEPATH += $$PWD \
               src


LIBS += -L../../libKitsunemimiArgs/src -lKitsunemimiArgs
LIBS += -L../../libKitsunemimiArgs/src/debug -lKitsunemimiArgs
LIBS += -L../../libKitsunemimiArgs/src/release -lKitsunemimiArgs
INCLUDEPATH += ../../libKitsunemimiArgs/include

LIBS += -L../../libKitsunemimiConfig/src -lKitsunemimiConfig
LIBS += -L../../libKitsunemimiConfig/src/debug -lKitsunemimiConfig
LIBS += -L../../libKitsunemimiConfig/src/release -lKitsunemimiConfig
INCLUDEPATH += ../../libKitsunemimiConfig/include

LIBS += -L../../libKitsunemimiSakuraLang/src -lKitsunemimiSakuraLang
LIBS += -L../../libKitsunemimiSakuraLang/src/debug -lKitsunemimiSakuraLang
LIBS += -L../../libKitsunemimiSakuraLang/src/release -lKitsunemimiSakuraLang
INCLUDEPATH += ../../libKitsunemimiSakuraLang/include

LIBS += -L../../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../../libKitsunemimiSakuraNetwork/include

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS += -L../../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../../libKitsunemimiNetwork/include

LIBS += -L../../libKitsunemimiPersistence/src -lKitsunemimiPersistence
LIBS += -L../../libKitsunemimiPersistence/src/debug -lKitsunemimiPersistence
LIBS += -L../../libKitsunemimiPersistence/src/release -lKitsunemimiPersistence
INCLUDEPATH += ../../libKitsunemimiPersistence/include

LIBS += -L../../libKitsunemimiJinja2/src -lKitsunemimiJinja2
LIBS += -L../../libKitsunemimiJinja2/src/debug -lKitsunemimiJinja2
LIBS += -L../../libKitsunemimiJinja2/src/release -lKitsunemimiJinja2
INCLUDEPATH += ../../libKitsunemimiJinja2/include

LIBS += -L../../libKitsunemimiJson/src -lKitsunemimiJson
LIBS += -L../../libKitsunemimiJson/src/debug -lKitsunemimiJson
LIBS += -L../../libKitsunemimiJson/src/release -lKitsunemimiJson
INCLUDEPATH += ../../libKitsunemimiJson/include

LIBS += -L../../libKitsunemimiIni/src -lKitsunemimiIni
LIBS += -L../../libKitsunemimiIni/src/debug -lKitsunemimiIni
LIBS += -L../../libKitsunemimiIni/src/release -lKitsunemimiIni
INCLUDEPATH += ../../libKitsunemimiIni/include

LIBS +=  -lboost_filesystem -lboost_system -lssl -lcrypto


HEADERS += \
    common/defines.h \
    common/enums.h \
    common/includes.h \
    common/typedefs.h \
    networking/host_handler.h \
    networking/messages.h \
    networking/network_callbacks.h \
    networking/sakura_network.h \
    args.h \
    common.h \
    config.h \
    sakura_root.h \
    sakura_tree_callbacks.h \
    blossoms/apt_blossoms.h \
    blossoms/ini_blossoms.h \
    blossoms/path_blossoms.h \
    blossoms/special_blossoms.h \
    blossoms/ssh_blossoms.h \
    blossoms/template_blossoms.h \
    blossoms/text_blossoms.h

SOURCES += \
    networking/host_handler.cpp \
    networking/sakura_network.cpp \
    main.cpp \
    sakura_root.cpp \
    blossoms/apt_blossoms.cpp \
    blossoms/ini_blossoms.cpp \
    blossoms/path_blossoms.cpp \
    blossoms/speacal_blossom.cpp \
    blossoms/ssh_blossoms.cpp \
    blossoms/template_blossoms.cpp \
    blossoms/text_blossoms.cpp


SAKURA_PROVISIONING_SUBTREE = predefined_subtrees/sakura_provisioning_subtree.tree

OTHER_FILES +=  \
    $$SAKURA_PROVISIONING_SUBTREE

sakura_provisioning_subtree.input = SAKURA_PROVISIONING_SUBTREE
sakura_provisioning_subtree.output = ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.commands = xxd -i ${QMAKE_FILE_IN} | sed 's/_________SakuraTree_src_predefined_subtrees_//g' > ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.variable_out = HEADERS
sakura_provisioning_subtree.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += sakura_provisioning_subtree


