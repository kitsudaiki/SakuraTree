#!/bin/bash

# get current directory-path and the path of the parent-directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PARENT_DIR="$(dirname "$DIR")"

# create build-directory
BUILD_DIR="$PARENT_DIR/build"
mkdir -p $BUILD_DIR

# create directory for the final result
RESULT_DIR="$PARENT_DIR/result"
mkdir -p $RESULT_DIR

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsuneCommon
git clone  git@gitlab.com:tobiasanker/libKitsuneCommon.git "$PARENT_DIR/libKitsuneCommon"
cd "$PARENT_DIR/libKitsuneCommon"
git checkout 0.5.3

# create build directory for libKitsuneCommon and go into this directory
LIB_KITSUNE_COMMON_DIR="$BUILD_DIR/libKitsuneCommon"
mkdir -p $LIB_KITSUNE_COMMON_DIR
cd $LIB_KITSUNE_COMMON_DIR

# build libKitsuneCommon library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneCommon/libKitsuneCommon.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_COMMON_DIR/src/libKitsuneCommon.so.0.5.3" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneCommon/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsuneJson
git clone  git@gitlab.com:tobiasanker/libKitsuneJson.git "$PARENT_DIR/libKitsuneJson"
cd "$PARENT_DIR/libKitsuneJson"
git checkout 0.5.0

# create build directory for libKitsuneJson and go into this directory
LIB_KITSUNE_JSON_DIR="$BUILD_DIR/libKitsuneJson"
mkdir -p $LIB_KITSUNE_JSON_DIR
cd $LIB_KITSUNE_JSON_DIR

# build libKitsuneJson library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneJson/libKitsuneJson.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_JSON_DIR/src/libKitsuneJson.so.0.5.0" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneJson/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsuneJinja2
git clone  git@gitlab.com:tobiasanker/libKitsuneJinja2.git "$PARENT_DIR/libKitsuneJinja2"
cd "$PARENT_DIR/libKitsuneJinja2"
git checkout 0.3.0

# create build directory for libKitsuneJinja2 and go into this directory
LIB_KITSUNE_JINJA2_DIR="$BUILD_DIR/libKitsuneJinja2"
mkdir -p $LIB_KITSUNE_JINJA2_DIR
cd $LIB_KITSUNE_JINJA2_DIR

# build libKitsuneJinja2 library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneJinja2/libKitsuneJinja2.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_JINJA2_DIR/src/libKitsuneJinja2.so.0.3.0" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneJinja2/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsuneSakuraParser
git clone  git@gitlab.com:tobiasanker/libKitsuneSakuraParser.git "$PARENT_DIR/libKitsuneSakuraParser"
cd "$PARENT_DIR/libKitsuneSakuraParser"
git checkout 17bb072a250352b83f50138f9dad24b220d695d5

# create build directory for libKitsuneSakuraParser and go into this directory
LIB_KITSUNE_SAKURA_PAESER_DIR="$BUILD_DIR/libKitsuneSakuraParser"
mkdir -p $LIB_KITSUNE_SAKURA_PAESER_DIR
cd $LIB_KITSUNE_SAKURA_PAESER_DIR

# build libKitsuneSakuraParser library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneSakuraParser/libKitsuneSakuraParser.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_SAKURA_PAESER_DIR/src/libKitsuneSakuraParser.so.0.1.0" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneSakuraParser/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# create build directory for SakuraTree and go into this directory
LIB_KITSUNE_SAKURA_TREE_DIR="$BUILD_DIR/SakuraTree"
mkdir -p $LIB_KITSUNE_SAKURA_TREE_DIR
cd $LIB_KITSUNE_SAKURA_TREE_DIR

# build SakuraTree library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/SakuraTree/SakuraTree.pro" -spec linux-g++ "CONFIG += optimize_full" "QMAKE_CXXFLAGS += -DRUN_UNIT_TEST"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_SAKURA_TREE_DIR/SakuraTree" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# recreate symlinks
cd "$RESULT_DIR/"
ln -s libKitsuneCommon.so.0.5.3 libKitsuneCommon.so.0.5
ln -s libKitsuneCommon.so.0.5.3 libKitsuneCommon.so.0
ln -s libKitsuneCommon.so.0.5.3 libKitsuneCommon.so

ln -s libKitsuneJson.so.0.5.0 libKitsuneJson.so.0.5
ln -s libKitsuneJson.so.0.5.0 libKitsuneJson.so.0
ln -s libKitsuneJson.so.0.5.0 libKitsuneJson.so

ln -s libKitsuneJinja2.so.0.3.0 libKitsuneJinja2.so.0.3
ln -s libKitsuneJinja2.so.0.3.0 libKitsuneJinja2.so.0
ln -s libKitsuneJinja2.so.0.3.0 libKitsuneJinja2.so

ln -s libKitsuneSakuraParser.so.0.1.0 libKitsuneSakuraParser.so.0.1
ln -s libKitsuneSakuraParser.so.0.1.0 libKitsuneSakuraParser.so.0
ln -s libKitsuneSakuraParser.so.0.1.0 libKitsuneSakuraParser.so
