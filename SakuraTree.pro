QT -= qt core gui

TARGET = SakuraTree
CONFIG += console
CONFIG += c++14

LIBS += -L../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../libKitsunemimiSakuraNetwork/include

LIBS += -L../libKitsunemimiProjectCommon/src -lKitsunemimiProjectCommon
LIBS += -L../libKitsunemimiProjectCommon/src/debug -lKitsunemimiProjectCommon
LIBS += -L../libKitsunemimiProjectCommon/src/release -lKitsunemimiProjectCommon
INCLUDEPATH += ../libKitsunemimiProjectCommon/include

LIBS += -L../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../libKitsunemimiCommon/include

LIBS += -L../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../libKitsunemimiNetwork/include

LIBS += -L../libKitsunemimiPersistence/src -lKitsunemimiPersistence
LIBS += -L../libKitsunemimiPersistence/src/debug -lKitsunemimiPersistence
LIBS += -L../libKitsunemimiPersistence/src/release -lKitsunemimiPersistence
INCLUDEPATH += ../libKitsunemimiPersistence/include

LIBS += -L../libKitsunemimiJinja2/src -lKitsunemimiJinja2
LIBS += -L../libKitsunemimiJinja2/src/debug -lKitsunemimiJinja2
LIBS += -L../libKitsunemimiJinja2/src/release -lKitsunemimiJinja2
INCLUDEPATH += ../libKitsunemimiJinja2/include

LIBS += -L../libKitsunemimiJson/src -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/debug -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/release -lKitsunemimiJson
INCLUDEPATH += ../libKitsunemimiJson/include

LIBS += -L../libKitsunemimiSakuraParser/src -lKitsunemimiSakuraParser
LIBS += -L../libKitsunemimiSakuraParser/src/debug -lKitsunemimiSakuraParser
LIBS += -L../libKitsunemimiSakuraParser/src/release -lKitsunemimiSakuraParser
INCLUDEPATH += ../libKitsunemimiSakuraParser/include

LIBS +=  -lboost_filesystem -lboost_system -lboost_program_options  -lssl -lcrypt

INCLUDEPATH += $$PWD \
               src

SOURCES += \
    src/main.cpp \
    src/processing/blossoms/blossom.cpp \
    src/initializing/file_collector.cpp \
    src/initializing/sakura_compiler.cpp \
    src/processing/blossoms/install/apt/apt_absent_blossom.cpp \
    src/processing/blossoms/install/apt/apt_latest_bossom.cpp \
    src/processing/blossoms/install/apt/apt_present_bossom.cpp \
    src/processing/blossoms/install/apt/apt_update_blossom.cpp \
    src/processing/blossoms/install/apt/apt_upgrade_blossom.cpp \
    src/processing/blossoms/blossom_getter.cpp \
    src/processing/sakura_thread.cpp \
    src/items/item_methods.cpp \
    src/sakura_root.cpp \
    tests/core/common_methods_test.cpp \
    tests/run_unit_tests.cpp \
    src/items/sakura_items.cpp \
    src/processing/blossoms/install/apt/apt_methods.cpp \
    src/processing/process_methods.cpp \
    src/branch_builder/provision_branch_builder.cpp \
    src/processing/blossoms/ssh/ssh_scp_blossom.cpp \
    src/processing/blossoms/ssh/ssh_cmd_blossom.cpp \
    src/processing/blossoms/sakura/sakura_copy_subtree_blossom.cpp \
    src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.cpp \
    src/processing/blossoms/sakura/print_blossom.cpp


HEADERS += \
    src/common.h \
    src/common/defines.h \
    src/common/enums.h \
    src/common/includes.h \
    src/common/typedefs.h \
    src/processing/blossoms/blossom.h \
    src/initializing/file_collector.h \
    src/initializing/sakura_compiler.h \
    src/processing/blossoms/install/apt/apt_absent_blossom.h \
    src/processing/blossoms/install/apt/apt_latest_blossom.h \
    src/processing/blossoms/install/apt/apt_update_blossom.h \
    src/processing/blossoms/install/apt/apt_present_blossom.h \
    src/processing/blossoms/install/apt/apt_upgrade_blossom.h \
    src/processing/blossoms/blossom_getter.h \
    src/processing/sakura_thread.h \
    src/items/item_methods.h \
    src/sakura_root.h \
    tests/core/common_methods_test.h \
    tests/run_unit_tests.h \
    src/items/sakura_items.h \
    src/processing/blossoms/install/apt/apt_methods.h \
    src/processing/process_methods.h \
    src/branch_builder/provision_branch_builder.h \
    src/processing/blossoms/ssh/ssh_scp_blossom.h \
    src/processing/blossoms/ssh/ssh_cmd_blossom.h \
    src/processing/blossoms/sakura/sakura_copy_subtree_blossom.h \
    src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.h \
    src/processing/sakura_tree_callbacks.h \
    src/processing/blossoms/sakura/print_blossom.h


