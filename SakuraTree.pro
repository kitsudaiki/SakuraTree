QT -= qt core gui

TARGET = SakuraTree
CONFIG += console
CONFIG += c++14

# LIBS += -L../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
# LIBS += -L../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
# LIBS += -L../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
# INCLUDEPATH += ../libKitsunemimiSakuraNetwork/include

# LIBS += -L../libKitsunemimiProjectCommon/src -lKitsunemimiProjectCommon
# LIBS += -L../libKitsunemimiProjectCommon/src/debug -lKitsunemimiProjectCommon
# LIBS += -L../libKitsunemimiProjectCommon/src/release -lKitsunemimiProjectCommon
I# NCLUDEPATH += ../libKitsunemimiProjectCommon/include

LIBS += -L../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../libKitsunemimiCommon/include

# LIBS += -L../libKitsunemimiNetwork/src -lKitsunemimiNetwork
# LIBS += -L../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
# LIBS += -L../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
# INCLUDEPATH += ../libKitsunemimiNetwork/include

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

LIBS += -L../libKitsunemimiIni/src -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/debug -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/release -lKitsunemimiIni
INCLUDEPATH += ../libKitsunemimiIni/include

LIBS += -L../libKitsunemimiSakuraParser/src -lKitsunemimiSakuraParser
LIBS += -L../libKitsunemimiSakuraParser/src/debug -lKitsunemimiSakuraParser
LIBS += -L../libKitsunemimiSakuraParser/src/release -lKitsunemimiSakuraParser
INCLUDEPATH += ../libKitsunemimiSakuraParser/include

LIBS +=  -lboost_filesystem -lboost_system -lboost_program_options  -lssl -lcrypt

INCLUDEPATH += $$PWD \
               src

HEADERS += \
    src/branch_builder/provision_branch_builder.h \
    src/common/defines.h \
    src/common/enums.h \
    src/common/includes.h \
    src/common/typedefs.h \
    src/converter/converter.h \
    src/items/sakura_items.h \
    src/items/value_items.h \
    src/processing/blossoms/files/common_files/file_chmod_blossom.h \
    src/processing/blossoms/files/common_files/file_chown_blossom.h \
    src/processing/blossoms/files/common_files/file_copy_blossom.h \
    src/processing/blossoms/files/common_files/file_delete_blossom.h \
    src/processing/blossoms/files/common_files/file_rename_blossom.h \
    src/processing/blossoms/files/ini_files/ini_delete_entry_blossom.h \
    src/processing/blossoms/files/ini_files/ini_read_entry_blossom.h \
    src/processing/blossoms/files/ini_files/ini_set_entry_blossom.h \
    src/processing/blossoms/files/template_files/template_create_blossom.h \
    src/processing/blossoms/files/text_files/text_append_blossom.h \
    src/processing/blossoms/files/text_files/text_read_blossom.h \
    src/processing/blossoms/files/text_files/text_replace_blossom.h \
    src/processing/blossoms/files/text_files/text_write_blossom.h \
    src/processing/blossoms/install/apt/apt_absent_blossom.h \
    src/processing/blossoms/install/apt/apt_latest_blossom.h \
    src/processing/blossoms/install/apt/apt_methods.h \
    src/processing/blossoms/install/apt/apt_present_blossom.h \
    src/processing/blossoms/install/apt/apt_update_blossom.h \
    src/processing/blossoms/install/apt/apt_upgrade_blossom.h \
    src/processing/blossoms/special/assert_blossom.h \
    src/processing/blossoms/special/cmd_blossom.h \
    src/processing/blossoms/special/exit_blossom.h \
    src/processing/blossoms/special/print_blossom.h \
    src/processing/blossoms/special/sakura_copy_subtree_blossom.h \
    src/processing/blossoms/ssh/ssh_cmd_blossom.h \
    src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.h \
    src/processing/blossoms/ssh/ssh_scp_blossom.h \
    src/processing/blossoms/blossom.h \
    src/processing/blossoms/blossom_getter.h \
    src/processing/common/item_methods.h \
    src/processing/sakura_thread.h \
    src/processing/sakura_tree_callbacks.h \
    src/processing/subtree_queue.h \
    src/processing/thread_pool.h \
    src/common.h \
    src/sakura_root.h \
    tests/processing/blossoms/files/common_files/file_chmod_blossom_test.h \
    tests/processing/blossoms/files/common_files/file_chown_blossom_test.h \
    tests/processing/blossoms/files/common_files/file_copy_blossom_test.h \
    tests/processing/blossoms/files/common_files/file_delete_blossom_test.h \
    tests/processing/blossoms/files/common_files/file_rename_blossom_test.h \
    tests/processing/blossoms/files/template_files/template_create_blossom_test.h \
    tests/processing/blossoms/files/text_files/text_append_blossom_test.h \
    tests/processing/blossoms/files/text_files/text_read_blossom_test.h \
    tests/processing/blossoms/files/text_files/text_replace_blossom_test.h \
    tests/processing/blossoms/files/text_files/text_write_blossom_test.h \
    tests/processing/blossoms/special/cmd_blossom_test.h \
    tests/processing/common/functions_test.h \
    tests/processing/common/item_methods_test.h \
    tests/run_unit_tests.h \
    src/processing/common/value_item_functions.h \
    src/converter/common_converter_methods.h \
    src/items/value_item_map.h

SOURCES += \
    src/branch_builder/provision_branch_builder.cpp \
    src/converter/converter.cpp \
    src/items/sakura_items.cpp \
    src/processing/blossoms/files/common_files/file_chmod_blossom.cpp \
    src/processing/blossoms/files/common_files/file_chown_blossom.cpp \
    src/processing/blossoms/files/common_files/file_copy_blossom.cpp \
    src/processing/blossoms/files/common_files/file_delete_blossom.cpp \
    src/processing/blossoms/files/common_files/file_rename_blossom.cpp \
    src/processing/blossoms/files/ini_files/ini_delete_entry_blossom.cpp \
    src/processing/blossoms/files/ini_files/ini_read_entry_blossom.cpp \
    src/processing/blossoms/files/ini_files/ini_set_entry_blossom.cpp \
    src/processing/blossoms/files/template_files/template_create_blossom.cpp \
    src/processing/blossoms/files/text_files/text_append_blossom.cpp \
    src/processing/blossoms/files/text_files/text_read_blossom.cpp \
    src/processing/blossoms/files/text_files/text_replace_blossom.cpp \
    src/processing/blossoms/files/text_files/text_write_blossom.cpp \
    src/processing/blossoms/install/apt/apt_absent_blossom.cpp \
    src/processing/blossoms/install/apt/apt_latest_bossom.cpp \
    src/processing/blossoms/install/apt/apt_methods.cpp \
    src/processing/blossoms/install/apt/apt_present_bossom.cpp \
    src/processing/blossoms/install/apt/apt_update_blossom.cpp \
    src/processing/blossoms/install/apt/apt_upgrade_blossom.cpp \
    src/processing/blossoms/special/assert_blossom.cpp \
    src/processing/blossoms/special/cmd_blossom.cpp \
    src/processing/blossoms/special/exit_blossom.cpp \
    src/processing/blossoms/special/print_blossom.cpp \
    src/processing/blossoms/special/sakura_copy_subtree_blossom.cpp \
    src/processing/blossoms/ssh/ssh_cmd_blossom.cpp \
    src/processing/blossoms/ssh/ssh_cmd_create_file_blossom.cpp \
    src/processing/blossoms/ssh/ssh_scp_blossom.cpp \
    src/processing/blossoms/blossom.cpp \
    src/processing/blossoms/blossom_getter.cpp \
    src/processing/common/item_methods.cpp \
    src/processing/common/value_item_functions.cpp \
    src/processing/sakura_thread.cpp \
    src/processing/subtree_queue.cpp \
    src/processing/thread_pool.cpp \
    src/main.cpp \
    src/sakura_root.cpp \
    tests/processing/blossoms/files/common_files/file_chmod_blossom_test.cpp \
    tests/processing/blossoms/files/common_files/file_chown_blossom_test.cpp \
    tests/processing/blossoms/files/common_files/file_copy_blossom_test.cpp \
    tests/processing/blossoms/files/common_files/file_delete_blossom_test.cpp \
    tests/processing/blossoms/files/common_files/file_rename_blossom_test.cpp \
    tests/processing/blossoms/files/template_files/template_create_blossom_test.cpp \
    tests/processing/blossoms/files/text_files/text_append_blossom_test.cpp \
    tests/processing/blossoms/files/text_files/text_read_blossom_test.cpp \
    tests/processing/blossoms/files/text_files/text_replace_blossom_test.cpp \
    tests/processing/blossoms/files/text_files/text_write_blossom_test.cpp \
    tests/processing/blossoms/special/cmd_blossom_test.cpp \
    tests/processing/common/functions_test.cpp \
    tests/processing/common/item_methods_test.cpp \
    tests/run_unit_tests.cpp \
    src/converter/common_converter_methods.cpp \
    src/items/value_item_map.cpp




