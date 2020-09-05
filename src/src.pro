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
    blossoms/files/common_files/path_chmod_blossom.h \
    blossoms/files/common_files/path_chown_blossom.h \
    blossoms/files/common_files/path_copy_blossom.h \
    blossoms/files/common_files/path_delete_blossom.h \
    blossoms/files/common_files/path_rename_blossom.h \
    blossoms/files/ini_files/ini_delete_entry_blossom.h \
    blossoms/files/ini_files/ini_read_entry_blossom.h \
    blossoms/files/ini_files/ini_set_entry_blossom.h \
    blossoms/files/template_files/template_create_file_blossom.h \
    blossoms/files/template_files/template_create_string_blossom.h \
    blossoms/files/template_files/template_methods.h \
    blossoms/files/text_files/text_append_blossom.h \
    blossoms/files/text_files/text_read_blossom.h \
    blossoms/files/text_files/text_replace_blossom.h \
    blossoms/files/text_files/text_write_blossom.h \
    blossoms/install/apt/apt_absent_blossom.h \
    blossoms/install/apt/apt_latest_blossom.h \
    blossoms/install/apt/apt_methods.h \
    blossoms/install/apt/apt_present_blossom.h \
    blossoms/install/apt/apt_update_blossom.h \
    blossoms/install/apt/apt_upgrade_blossom.h \
    blossoms/special/assert_blossom.h \
    blossoms/special/cmd_blossom.h \
    blossoms/special/exit_blossom.h \
    blossoms/special/item_update_blossom.h \
    blossoms/special/print_blossom.h \
    blossoms/ssh/ssh_cmd_blossom.h \
    blossoms/ssh/ssh_cmd_create_file_blossom.h \
    blossoms/ssh/ssh_scp_blossom.h \
    blossoms/blossom_getter.h \
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
    sakura_tree_callbacks.h

SOURCES += \
    blossoms/files/common_files/path_chmod_blossom.cpp \
    blossoms/files/common_files/path_chown_blossom.cpp \
    blossoms/files/common_files/path_copy_blossom.cpp \
    blossoms/files/common_files/path_delete_blossom.cpp \
    blossoms/files/common_files/path_rename_blossom.cpp \
    blossoms/files/ini_files/ini_delete_entry_blossom.cpp \
    blossoms/files/ini_files/ini_read_entry_blossom.cpp \
    blossoms/files/ini_files/ini_set_entry_blossom.cpp \
    blossoms/files/template_files/template_create_file_blossom.cpp \
    blossoms/files/template_files/template_create_string_blossom.cpp \
    blossoms/files/template_files/template_methods.cpp \
    blossoms/files/text_files/text_append_blossom.cpp \
    blossoms/files/text_files/text_read_blossom.cpp \
    blossoms/files/text_files/text_replace_blossom.cpp \
    blossoms/files/text_files/text_write_blossom.cpp \
    blossoms/install/apt/apt_absent_blossom.cpp \
    blossoms/install/apt/apt_latest_bossom.cpp \
    blossoms/install/apt/apt_methods.cpp \
    blossoms/install/apt/apt_present_bossom.cpp \
    blossoms/install/apt/apt_update_blossom.cpp \
    blossoms/install/apt/apt_upgrade_blossom.cpp \
    blossoms/special/assert_blossom.cpp \
    blossoms/special/cmd_blossom.cpp \
    blossoms/special/exit_blossom.cpp \
    blossoms/special/item_update_blossom.cpp \
    blossoms/special/print_blossom.cpp \
    blossoms/ssh/ssh_cmd_blossom.cpp \
    blossoms/ssh/ssh_cmd_create_file_blossom.cpp \
    blossoms/ssh/ssh_scp_blossom.cpp \
    networking/host_handler.cpp \
    networking/sakura_network.cpp \
    main.cpp \
    sakura_root.cpp


SAKURA_PROVISIONING_SUBTREE = predefined_subtrees/sakura_provisioning_subtree.tree

OTHER_FILES +=  \
    $$SAKURA_PROVISIONING_SUBTREE

sakura_provisioning_subtree.input = SAKURA_PROVISIONING_SUBTREE
sakura_provisioning_subtree.output = ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.commands = xxd -i ${QMAKE_FILE_IN} | sed 's/_________SakuraTree_src_predefined_subtrees_//g' > ${QMAKE_FILE_BASE}.h
sakura_provisioning_subtree.variable_out = HEADERS
sakura_provisioning_subtree.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += sakura_provisioning_subtree


