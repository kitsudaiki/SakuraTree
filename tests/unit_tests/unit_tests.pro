include(../../defaults.pri)

QT -= qt core gui

TARGET = SakuraTree_UnitTests
CONFIG += console
CONFIG += c++14

LIBS += -L../../../libKitsunemimiArgs/src -lKitsunemimiArgs
LIBS += -L../../../libKitsunemimiArgs/src/debug -lKitsunemimiArgs
LIBS += -L../../../libKitsunemimiArgs/src/release -lKitsunemimiArgs
INCLUDEPATH += ../../../libKitsunemimiArgs/include

LIBS += -L../../../libKitsunemimiConfig/src -lKitsunemimiConfig
LIBS += -L../../../libKitsunemimiConfig/src/debug -lKitsunemimiConfig
LIBS += -L../../../libKitsunemimiConfig/src/release -lKitsunemimiConfig
INCLUDEPATH += ../../../libKitsunemimiConfig/include

LIBS += -L../../../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../../../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../../../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../../../libKitsunemimiSakuraNetwork/include

LIBS += -L../../../libKitsunemimiProjectNetwork/src -lKitsunemimiProjectNetwork
LIBS += -L../../../libKitsunemimiProjectNetwork/src/debug -lKitsunemimiProjectNetwork
LIBS += -L../../../libKitsunemimiProjectNetwork/src/release -lKitsunemimiProjectNetwork
INCLUDEPATH += ../../../libKitsunemimiProjectNetwork/include

LIBS += -L../../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../../libKitsunemimiCommon/include

LIBS += -L../../../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../../../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../../../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../../../libKitsunemimiNetwork/include

LIBS += -L../../../libKitsunemimiPersistence/src -lKitsunemimiPersistence
LIBS += -L../../../libKitsunemimiPersistence/src/debug -lKitsunemimiPersistence
LIBS += -L../../../libKitsunemimiPersistence/src/release -lKitsunemimiPersistence
INCLUDEPATH += ../../../libKitsunemimiPersistence/include

LIBS += -L../../../libKitsunemimiJinja2/src -lKitsunemimiJinja2
LIBS += -L../../../libKitsunemimiJinja2/src/debug -lKitsunemimiJinja2
LIBS += -L../../../libKitsunemimiJinja2/src/release -lKitsunemimiJinja2
INCLUDEPATH += ../../../libKitsunemimiJinja2/include

LIBS += -L../../../libKitsunemimiJson/src -lKitsunemimiJson
LIBS += -L../../../libKitsunemimiJson/src/debug -lKitsunemimiJson
LIBS += -L../../../libKitsunemimiJson/src/release -lKitsunemimiJson
INCLUDEPATH += ../../../libKitsunemimiJson/include

LIBS += -L../../../libKitsunemimiIni/src -lKitsunemimiIni
LIBS += -L../../../libKitsunemimiIni/src/debug -lKitsunemimiIni
LIBS += -L../../../libKitsunemimiIni/src/release -lKitsunemimiIni
INCLUDEPATH += ../../../libKitsunemimiIni/include

LIBS += -L../../../libKitsunemimiSakuraLang/src -lKitsunemimiSakuraLang
LIBS += -L../../../libKitsunemimiSakuraLang/src/debug -lKitsunemimiSakuraLang
LIBS += -L../../../libKitsunemimiSakuraLang/src/release -lKitsunemimiSakuraLang
INCLUDEPATH += ../../../libKitsunemimiSakuraLang/include

LIBS +=  -lboost_filesystem -lboost_system -lssl -lcrypto

INCLUDEPATH += $$PWD

HEADERS += \
    ../../src/common/defines.h \
    ../../src/common/enums.h \
    ../../src/common/includes.h \
    ../../src/common/typedefs.h \
    ../../src/items/sakura_items.h \
    ../../src/items/value_items.h \
    ../../src/processing/blossoms/files/ini_files/ini_delete_entry_blossom.h \
    ../../src/processing/blossoms/files/ini_files/ini_read_entry_blossom.h \
    ../../src/processing/blossoms/files/ini_files/ini_set_entry_blossom.h \
    ../../src/processing/blossoms/files/text_files/text_append_blossom.h \
    ../../src/processing/blossoms/files/text_files/text_read_blossom.h \
    ../../src/processing/blossoms/files/text_files/text_replace_blossom.h \
    ../../src/processing/blossoms/files/text_files/text_write_blossom.h \
    ../../src/processing/blossoms/install/apt/apt_absent_blossom.h \
    ../../src/processing/blossoms/install/apt/apt_latest_blossom.h \
    ../../src/processing/blossoms/install/apt/apt_methods.h \
    ../../src/processing/blossoms/install/apt/apt_present_blossom.h \
    ../../src/processing/blossoms/install/apt/apt_update_blossom.h \
    ../../src/processing/blossoms/install/apt/apt_upgrade_blossom.h \
    ../../src/processing/blossoms/special/assert_blossom.h \
    ../../src/processing/blossoms/special/cmd_blossom.h \
    ../../src/processing/blossoms/special/exit_blossom.h \
    ../../src/processing/blossoms/special/print_blossom.h \
    ../../src/processing/blossoms/ssh/ssh_cmd_blossom.h \
    ../../src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.h \
    ../../src/processing/blossoms/ssh/ssh_scp_blossom.h \
    ../../src/processing/blossoms/blossom.h \
    ../../src/processing/blossoms/blossom_getter.h \
    ../../src/processing/common/item_methods.h \
    ../../src/processing/sakura_thread.h \
    ../../src/processing/sakura_tree_callbacks.h \
    ../../src/processing/subtree_queue.h \
    ../../src/processing/thread_pool.h \
    ../../src/common.h \
    ../../src/sakura_root.h \
    ../../src/processing/common/value_item_functions.h \
    ../../src/processing/blossoms/files/common_files/path_chmod_blossom.h \
    ../../src/processing/blossoms/files/common_files/path_chown_blossom.h \
    ../../src/processing/blossoms/files/common_files/path_copy_blossom.h \
    ../../src/processing/blossoms/files/common_files/path_delete_blossom.h \
    ../../src/processing/blossoms/files/common_files/path_rename_blossom.h \
    ../../src/processing/blossoms/special/item_update_blossom.h \
    ../../src/args.h \
    ../../src/config.h \
    ../../src/processing/blossoms/files/template_files/template_create_file_blossom.h \
    ../../src/processing/blossoms/files/template_files/template_create_string_blossom.h \
    ../../src/processing/blossoms/files/template_files/template_methods.h \
    ../../src/processing/validator.h \
    processing/blossoms/files/common_files/path_rename_blossom_test.h \
    processing/blossoms/files/common_files/path_delete_blossom_test.h \
    processing/blossoms/files/common_files/path_copy_blossom_test.h \
    processing/blossoms/files/common_files/path_chmod_blossom_test.h \
    processing/blossoms/files/common_files/path_chown_blossom_test.h \
    processing/blossoms/files/template_files/template_create_blossom_test.h \
    processing/blossoms/files/text_files/text_append_blossom_test.h \
    processing/blossoms/files/text_files/text_read_blossom_test.h \
    processing/blossoms/files/text_files/text_replace_blossom_test.h \
    processing/blossoms/files/text_files/text_write_blossom_test.h \
    processing/blossoms/special/cmd_blossom_test.h \
    processing/common/functions_test.h \
    processing/common/item_methods_test.h

SOURCES += \
    ../../src/processing/blossoms/files/ini_files/ini_delete_entry_blossom.cpp \
    ../../src/processing/blossoms/files/ini_files/ini_read_entry_blossom.cpp \
    ../../src/processing/blossoms/files/ini_files/ini_set_entry_blossom.cpp \
    ../../src/processing/blossoms/files/text_files/text_append_blossom.cpp \
    ../../src/processing/blossoms/files/text_files/text_read_blossom.cpp \
    ../../src/processing/blossoms/files/text_files/text_replace_blossom.cpp \
    ../../src/processing/blossoms/files/text_files/text_write_blossom.cpp \
    ../../src/processing/blossoms/install/apt/apt_absent_blossom.cpp \
    ../../src/processing/blossoms/install/apt/apt_latest_bossom.cpp \
    ../../src/processing/blossoms/install/apt/apt_methods.cpp \
    ../../src/processing/blossoms/install/apt/apt_present_bossom.cpp \
    ../../src/processing/blossoms/install/apt/apt_update_blossom.cpp \
    ../../src/processing/blossoms/install/apt/apt_upgrade_blossom.cpp \
    ../../src/processing/blossoms/special/assert_blossom.cpp \
    ../../src/processing/blossoms/special/cmd_blossom.cpp \
    ../../src/processing/blossoms/special/exit_blossom.cpp \
    ../../src/processing/blossoms/special/print_blossom.cpp \
    ../../src/processing/blossoms/ssh/ssh_cmd_blossom.cpp \
    ../../src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.cpp \
    ../../src/processing/blossoms/ssh/ssh_scp_blossom.cpp \
    ../../src/processing/blossoms/blossom.cpp \
    ../../src/processing/blossoms/blossom_getter.cpp \
    ../../src/processing/common/item_methods.cpp \
    ../../src/processing/common/value_item_functions.cpp \
    ../../src/processing/sakura_thread.cpp \
    ../../src/processing/subtree_queue.cpp \
    ../../src/processing/thread_pool.cpp \
    ../../src/sakura_root.cpp \
    ../../src/processing/blossoms/files/common_files/path_chmod_blossom.cpp \
    ../../src/processing/blossoms/files/common_files/path_chown_blossom.cpp \
    ../../src/processing/blossoms/files/common_files/path_copy_blossom.cpp \
    ../../src/processing/blossoms/files/common_files/path_delete_blossom.cpp \
    ../../src/processing/blossoms/files/common_files/path_rename_blossom.cpp \
    ../../src/processing/blossoms/special/item_update_blossom.cpp \
    ../../src/processing/blossoms/files/template_files/template_create_file_blossom.cpp \
    ../../src/processing/blossoms/files/template_files/template_create_string_blossom.cpp \
    ../../src/processing/blossoms/files/template_files/template_methods.cpp \
    ../../src/processing/validator.cpp \
    processing/blossoms/files/common_files/path_chmod_blossom_test.cpp \
    processing/blossoms/files/common_files/path_chown_blossom_test.cpp \
    processing/blossoms/files/common_files/path_copy_blossom_test.cpp \
    processing/blossoms/files/common_files/path_delete_blossom_test.cpp \
    processing/blossoms/files/common_files/path_rename_blossom_test.cpp \
    processing/blossoms/files/template_files/template_create_blossom_test.cpp \
    processing/blossoms/files/text_files/text_append_blossom_test.cpp \
    processing/blossoms/files/text_files/text_read_blossom_test.cpp \
    processing/blossoms/files/text_files/text_replace_blossom_test.cpp \
    processing/blossoms/files/text_files/text_write_blossom_test.cpp \
    processing/blossoms/special/cmd_blossom_test.cpp \
    processing/common/functions_test.cpp \
    processing/common/item_methods_test.cpp \
    main.cpp


SAKURA_PROVISIONING_SUBTREE = ../../src/predefined_subtrees/sakura_provisioning_subtree.tree

OTHER_FILES +=  \
    $$SAKURA_PROVISIONING_SUBTREE

sakura_provisioning_subtree.input = SAKURA_PROVISIONING_SUBTREE
sakura_provisioning_subtree.output = ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.commands = xxd -i ${QMAKE_FILE_IN} | sed 's/____________SakuraTree_src_predefined_subtrees_//g' > ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.variable_out = HEADERS
sakura_provisioning_subtree.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += sakura_provisioning_subtree

