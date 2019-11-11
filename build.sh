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

function build_kitsune_lib_repo () {
    REPO_NAME=$1

    # create build directory for repo and go into this directory
    REPO_DIR="$BUILD_DIR/$REPO_NAME"
    mkdir -p $REPO_DIR
    cd $REPO_DIR

    # build repo library with qmake
    /usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/$REPO_NAME/$REPO_NAME.pro" -spec linux-g++ "CONFIG += optimize_full staticlib"
    /usr/bin/make -j4

    # copy build-result and include-files into the result-directory
    cp  $REPO_DIR/src/$REPO_NAME.a $RESULT_DIR/
    cp -r $PARENT_DIR/$REPO_NAME/include $RESULT_DIR/
}

function get_required_kitsune_lib_repo () {
    REPO_NAME=$1
    TAG_OR_BRANCH=$2

    # clone repo
    git clone  git@gitlab.com:tobiasanker/$REPO_NAME.git "$PARENT_DIR/$REPO_NAME"
    cd "$PARENT_DIR/$REPO_NAME"
    git checkout $TAG_OR_BRANCH

    build_kitsune_lib_repo $REPO_NAME
}

#-----------------------------------------------------------------------------------------------------------------

get_required_kitsune_lib_repo "libKitsunemimiCommon" "master"

get_required_kitsune_lib_repo "libKitsunemimiPersistence" "master"

get_required_kitsune_lib_repo "libKitsunemimiNetwork" "master"

get_required_kitsune_lib_repo "libKitsunemimiJson" "master"

get_required_kitsune_lib_repo "libKitsunemimiJinja2" "master"

get_required_kitsune_lib_repo "libKitsunemimiSakuraParser" "master"

get_required_kitsune_lib_repo "libKitsunemimiProjectCommon" "master"

get_required_kitsune_lib_repo "libKitsunemimiSakuraNetwork" "master"

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

