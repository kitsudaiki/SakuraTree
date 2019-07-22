QT -= qt core gui

TARGET = SakuraSeed
CONFIG += console
CONFIG += c++14

LIBS += -L../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS += -L../libKitsuneJson/src -lKitsuneJson
LIBS += -L../libKitsuneJson/src/debug -lKitsuneJson
LIBS += -L../libKitsuneJson/src/release -lKitsuneJson
INCLUDEPATH += ../../libKitsuneJson/include/libKitsuneJson

LIBS += -L../libKitsuneJinja2/src -lKitsuneJinja2
LIBS += -L../libKitsuneJinja2/src/debug -lKitsuneJinja2
LIBS += -L../libKitsuneJinja2/src/release -lKitsuneJinja2
INCLUDEPATH += ../../libKitsuneJinja2/include/libKitsuneJinja2

LIBS += -L../libKitsuneSakuraParser/src -lKitsuneSakuraParser
LIBS += -L../libKitsuneSakuraParser/src/debug -lKitsuneSakuraParser
LIBS += -L../libKitsuneSakuraParser/src/release -lKitsuneSakuraParser
INCLUDEPATH += ../libKitsuneSakuraParser/include/libKitsuneSakuraParser

LIBS +=  -lboost_filesystem -lboost_system -lboost_program_options -lboost_iostreams

INCLUDEPATH += $$PWD \
            src

SOURCES += \
    src/main.cpp \
    src/processing/blossoms/install/apt/aptBlossom.cpp \
    src/sakuraRoot.cpp \
    src/processing/sakuraThread.cpp \
    src/initializing/sakuraCompiler.cpp \
    src/commonMethods.cpp \
    src/processing/blossoms/install/apt/aptUpdateBlossom.cpp \
    src/processing/blossoms/install/apt/aptUpgradeBlossom.cpp \
    src/processing/blossoms/install/apt/aptPresentBossom.cpp \
    src/processing/blossoms/install/apt/aptAbsentBlossom.cpp \
    src/processing/blossoms/install/apt/aptLatestBossom.cpp \
    src/processing/blossoms/blossom.cpp \
    src/processing/blossoms/ssh/scpBlossom.cpp \
    tests/runUnitTests.cpp \
    tests/core/commonMethodsTest.cpp \
    src/initializing/fileCollector.cpp \
    src/processing/blossoms/blossomGetter.cpp


HEADERS += \
    src/processing/blossoms/install/apt/aptBlossom.h \
    src/sakuraRoot.h \
    src/common.h \
    src/common/defines.h \
    src/common/enums.h \
    src/common/includes.h \
    src/common/typedefs.h \
    src/processing/sakuraThread.h \
    src/initializing/sakuraCompiler.h \
    src/commonMethods.h \
    src/processing/blossoms/install/apt/aptUpdateBlossom.h \
    src/processing/blossoms/install/apt/aptUpgradeBlossom.h \
    src/processing/blossoms/install/apt/aptAbsentBlossom.h \
    src/processing/blossoms/install/apt/aptPresentBlossom.h \
    src/processing/blossoms/install/apt/aptLatestBlossom.h \
    src/processing/blossoms/blossom.h \
    src/processing/blossoms/ssh/scpBlossom.h \
    tests/runUnitTests.h \
    tests/core/commonMethodsTest.h \
    src/initializing/fileCollector.h \
    src/processing/blossoms/blossomGetter.h


