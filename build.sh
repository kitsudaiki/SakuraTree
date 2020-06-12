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
    NUMBER_OF_THREADS=$2

    # create build directory for repo and go into this directory
    REPO_DIR="$BUILD_DIR/$REPO_NAME"
    mkdir -p $REPO_DIR
    cd $REPO_DIR

    # build repo library with qmake
    /usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/$REPO_NAME/$REPO_NAME.pro" -spec linux-g++ "CONFIG += optimize_full staticlib"
    /usr/bin/make -j$NUMBER_OF_THREADS

    # copy build-result and include-files into the result-directory
    cp -d $REPO_DIR/src/$REPO_NAME.so.* $RESULT_DIR/
    cp -r $PARENT_DIR/$REPO_NAME/include $RESULT_DIR/
    ls -l $RESULT_DIR/include/
    ls -l $RESULT_DIR
}

function get_required_kitsune_lib_repo () {
    REPO_NAME=$1
    TAG_OR_BRANCH=$2
    NUMBER_OF_THREADS=$3

    # clone repo
    git clone https://github.com/tobiasanker/$REPO_NAME.git "$PARENT_DIR/$REPO_NAME"
    cd "$PARENT_DIR/$REPO_NAME"
    git checkout $TAG_OR_BRANCH

    build_kitsune_lib_repo $REPO_NAME $NUMBER_OF_THREADS
}

#-----------------------------------------------------------------------------------------------------------------

get_required_kitsune_lib_repo "libKitsunemimiCommon" "master" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiPersistence" "master" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiArgs" "master" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiNetwork" "v0.6.2" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiJson" "0.10.2" 1
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiIni" "0.4.3" 1
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiConfig" "master" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiJinja2" "master" 1
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiSakuraParser" "update-multinode-setup" 1
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiProjectNetwork" "master" 4
echo ""
echo "###########################################################################################################"
echo ""
get_required_kitsune_lib_repo "libKitsunemimiSakuraNetwork" "master" 4
echo ""
echo "###########################################################################################################"
echo ""

#-----------------------------------------------------------------------------------------------------------------

# create build directory for SakuraTree and go into this directory
LIB_KITSUNE_SAKURA_TREE_DIR="$BUILD_DIR/SakuraTree"
mkdir -p $LIB_KITSUNE_SAKURA_TREE_DIR
cd $LIB_KITSUNE_SAKURA_TREE_DIR

# build SakuraTree library with qmake
if [ $1 = "test" ]; then
    /usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/SakuraTree/SakuraTree.pro" -spec linux-g++ "CONFIG += optimize_full" "CONFIG+=run_tests"
    /usr/bin/make -j4
    # copy build-result and include-files into the result-directory
    cp "$LIB_KITSUNE_SAKURA_TREE_DIR/SakuraTree_Test" "$RESULT_DIR/"
else
    /usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/SakuraTree/SakuraTree.pro" -spec linux-g++ "CONFIG += optimize_full"
    /usr/bin/make -j4
    # copy build-result and include-files into the result-directory
    cp "$LIB_KITSUNE_SAKURA_TREE_DIR/SakuraTree" "$RESULT_DIR/"
fi

#-----------------------------------------------------------------------------------------------------------------

