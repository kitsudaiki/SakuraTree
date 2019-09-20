QT -= qt core gui

TARGET = SakuraTree
CONFIG += console
CONFIG += c++14

LIBS += -L../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../libKitsuneCommon/include/libKitsuneCommon

LIBS += -L../libKitsuneJson/src -lKitsuneJson
LIBS += -L../libKitsuneJson/src/debug -lKitsuneJson
LIBS += -L../libKitsuneJson/src/release -lKitsuneJson
INCLUDEPATH += ../libKitsuneJson/include/libKitsuneJson

LIBS += -L../libKitsuneJinja2/src -lKitsuneJinja2
LIBS += -L../libKitsuneJinja2/src/debug -lKitsuneJinja2
LIBS += -L../libKitsuneJinja2/src/release -lKitsuneJinja2
INCLUDEPATH += ../libKitsuneJinja2/include/libKitsuneJinja2

LIBS += -L../libKitsuneSakuraParser/src -lKitsuneSakuraParser
LIBS += -L../libKitsuneSakuraParser/src/debug -lKitsuneSakuraParser
LIBS += -L../libKitsuneSakuraParser/src/release -lKitsuneSakuraParser
INCLUDEPATH += ../libKitsuneSakuraParser/include/libKitsuneSakuraParser

LIBS +=  -lboost_filesystem -lboost_system -lboost_program_options

INCLUDEPATH += $$PWD \
            src

SOURCES += \
    src/main.cpp \
    src/processing/blossoms/blossom.cpp \
    src/processing/blossoms/ssh/scpBlossom.cpp \
    src/initializing/file_collector.cpp \
    src/initializing/sakura_compiler.cpp \
    src/processing/blossoms/install/apt/apt_absent_blossom.cpp \
    src/processing/blossoms/install/apt/apt_blossom.cpp \
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
    src/items/sakura_items.cpp


HEADERS += \
    src/common.h \
    src/common/defines.h \
    src/common/enums.h \
    src/common/includes.h \
    src/common/typedefs.h \
    src/processing/blossoms/blossom.h \
    src/initializing/file_collector.h \
    src/initializing/sakura_compiler.h \
    src/processing/blossoms/ssh/scp_blossom.h \
    src/processing/blossoms/install/apt/apt_blossom.h \
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
    src/items/sakura_items.h


