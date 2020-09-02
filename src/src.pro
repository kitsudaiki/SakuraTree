QT -= qt core gui

TARGET = SakuraTree
CONFIG += console
CONFIG += c++14

LIBS += -L../../libKitsunemimiArgs/src -lKitsunemimiArgs
LIBS += -L../../libKitsunemimiArgs/src/debug -lKitsunemimiArgs
LIBS += -L../../libKitsunemimiArgs/src/release -lKitsunemimiArgs
INCLUDEPATH += ../../libKitsunemimiArgs/include

LIBS += -L../../libKitsunemimiConfig/src -lKitsunemimiConfig
LIBS += -L../../libKitsunemimiConfig/src/debug -lKitsunemimiConfig
LIBS += -L../../libKitsunemimiConfig/src/release -lKitsunemimiConfig
INCLUDEPATH += ../../libKitsunemimiConfig/include

LIBS += -L../../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../../libKitsunemimiSakuraNetwork/include

LIBS += -L../../libKitsunemimiProjectNetwork/src -lKitsunemimiProjectNetwork
LIBS += -L../../libKitsunemimiProjectNetwork/src/debug -lKitsunemimiProjectNetwork
LIBS += -L../../libKitsunemimiProjectNetwork/src/release -lKitsunemimiProjectNetwork
INCLUDEPATH += ../../libKitsunemimiProjectNetwork/include

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

LIBS += -L../../libKitsunemimiSakuraLang/src -lKitsunemimiSakuraLang
LIBS += -L../../libKitsunemimiSakuraLang/src/debug -lKitsunemimiSakuraLang
LIBS += -L../../libKitsunemimiSakuraLang/src/release -lKitsunemimiSakuraLang
INCLUDEPATH += ../../libKitsunemimiSakuraLang/include

LIBS +=  -lboost_filesystem -lboost_system -lssl -lcrypto

INCLUDEPATH += $$PWD \
               src

HEADERS += \
    common/defines.h \
    common/enums.h \
    common/includes.h \
    common/typedefs.h \
    items/sakura_items.h \
    items/value_items.h \
    processing/blossoms/files/ini_files/ini_delete_entry_blossom.h \
    processing/blossoms/files/ini_files/ini_read_entry_blossom.h \
    processing/blossoms/files/ini_files/ini_set_entry_blossom.h \
    processing/blossoms/files/text_files/text_append_blossom.h \
    processing/blossoms/files/text_files/text_read_blossom.h \
    processing/blossoms/files/text_files/text_replace_blossom.h \
    processing/blossoms/files/text_files/text_write_blossom.h \
    processing/blossoms/install/apt/apt_absent_blossom.h \
    processing/blossoms/install/apt/apt_latest_blossom.h \
    processing/blossoms/install/apt/apt_methods.h \
    processing/blossoms/install/apt/apt_present_blossom.h \
    processing/blossoms/install/apt/apt_update_blossom.h \
    processing/blossoms/install/apt/apt_upgrade_blossom.h \
    processing/blossoms/special/assert_blossom.h \
    processing/blossoms/special/cmd_blossom.h \
    processing/blossoms/special/exit_blossom.h \
    processing/blossoms/special/print_blossom.h \
    processing/blossoms/ssh/ssh_cmd_blossom.h \
    processing/blossoms/ssh/ssh_cmd_create_file_blossom.h \
    processing/blossoms/ssh/ssh_scp_blossom.h \
    processing/blossoms/blossom.h \
    processing/blossoms/blossom_getter.h \
    processing/common/item_methods.h \
    processing/sakura_thread.h \
    processing/sakura_tree_callbacks.h \
    processing/subtree_queue.h \
    processing/thread_pool.h \
    common.h \
    sakura_root.h \
    processing/common/value_item_functions.h \
    processing/blossoms/files/common_files/path_chmod_blossom.h \
    processing/blossoms/files/common_files/path_chown_blossom.h \
    processing/blossoms/files/common_files/path_copy_blossom.h \
    processing/blossoms/files/common_files/path_delete_blossom.h \
    processing/blossoms/files/common_files/path_rename_blossom.h \
    processing/blossoms/special/item_update_blossom.h \
    args.h \
    config.h \
    processing/blossoms/files/template_files/template_create_file_blossom.h \
    processing/blossoms/files/template_files/template_create_string_blossom.h \
    processing/blossoms/files/template_files/template_methods.h \
    processing/validator.h \

SOURCES += \
    processing/blossoms/files/ini_files/ini_delete_entry_blossom.cpp \
    processing/blossoms/files/ini_files/ini_read_entry_blossom.cpp \
    processing/blossoms/files/ini_files/ini_set_entry_blossom.cpp \
    processing/blossoms/files/text_files/text_append_blossom.cpp \
    processing/blossoms/files/text_files/text_read_blossom.cpp \
    processing/blossoms/files/text_files/text_replace_blossom.cpp \
    processing/blossoms/files/text_files/text_write_blossom.cpp \
    processing/blossoms/install/apt/apt_absent_blossom.cpp \
    processing/blossoms/install/apt/apt_latest_bossom.cpp \
    processing/blossoms/install/apt/apt_methods.cpp \
    processing/blossoms/install/apt/apt_present_bossom.cpp \
    processing/blossoms/install/apt/apt_update_blossom.cpp \
    processing/blossoms/install/apt/apt_upgrade_blossom.cpp \
    processing/blossoms/special/assert_blossom.cpp \
    processing/blossoms/special/cmd_blossom.cpp \
    processing/blossoms/special/exit_blossom.cpp \
    processing/blossoms/special/print_blossom.cpp \
    processing/blossoms/ssh/ssh_cmd_blossom.cpp \
    processing/blossoms/ssh/ssh_cmd_create_file_blossom.cpp \
    processing/blossoms/ssh/ssh_scp_blossom.cpp \
    processing/blossoms/blossom.cpp \
    processing/blossoms/blossom_getter.cpp \
    processing/common/item_methods.cpp \
    processing/common/value_item_functions.cpp \
    processing/sakura_thread.cpp \
    processing/subtree_queue.cpp \
    processing/thread_pool.cpp \
    sakura_root.cpp \
    processing/blossoms/files/common_files/path_chmod_blossom.cpp \
    processing/blossoms/files/common_files/path_chown_blossom.cpp \
    processing/blossoms/files/common_files/path_copy_blossom.cpp \
    processing/blossoms/files/common_files/path_delete_blossom.cpp \
    processing/blossoms/files/common_files/path_rename_blossom.cpp \
    processing/blossoms/special/item_update_blossom.cpp \
    processing/blossoms/files/template_files/template_create_file_blossom.cpp \
    processing/blossoms/files/template_files/template_create_string_blossom.cpp \
    processing/blossoms/files/template_files/template_methods.cpp \
    processing/validator.cpp \
    main.cpp \
    processing/subtree_calls.cpp

SAKURA_PROVISIONING_SUBTREE = predefined_subtrees/sakura_provisioning_subtree.tree

OTHER_FILES +=  \
    $$SAKURA_PROVISIONING_SUBTREE

sakura_provisioning_subtree.input = SAKURA_PROVISIONING_SUBTREE
sakura_provisioning_subtree.output = ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.commands = xxd -i ${QMAKE_FILE_IN} | sed 's/_________SakuraTree_src_predefined_subtrees_//g' > ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.variable_out = HEADERS
sakura_provisioning_subtree.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += sakura_provisioning_subtree



