#!/usr/bin/bash

set -e

if [[ $OSTYPE == msys* || $OSTYPE == mingw* || $OSTYPE == cygwin* ]]; then
    OS=win32
    EXT=".exe"
    LIBRARIES=""
else
    OS=posix
    EXT=""
    LIBRARIES=""
fi

mkdir -p out
mkdir -p res

cd out
cp -r ../res res

declare -A BUILD_ARGS
declare -A TEST_ARGS
TEST_ARGS["args"]="-a 1 -a 2 -a 3 -a 4 -a 5"
BUILD_ARGS["gm"]="-lm"
BUILD_ARGS["wav"]="-lm"
if [[ $OS == win32 ]]; then
    BUILD_ARGS["wav"]="-lwinmm"
    BUILD_ARGS["udp"]="-lws2_32"
else
    BUILD_ARGS["mathe"]="-lm"
    BUILD_ARGS["wav"]="-lasound"
    BUILD_ARGS["udp"]="-lnsl -lresolv"
fi

if [[ "$1" == "ALL" ]]; then
    APPS=()
    for file in ../*.h; do
        [ -f "$file" ] && APPS+=("$(basename "$file" .h)")
    done
else
    APPS=("$@")
fi

for APP in "${APPS[@]}"; do
    if [[ -n "${BUILD_ARGS[$APP]}" ]]; then
        cc="clang "../tests/$APP.c" \
        -O3 -Werror \
        -o "./$APP$EXT" \
        $LIBRARIES ${BUILD_ARGS[$APP]}"
    else
        cc="clang "../tests/$APP.c" \
        -O3 -Werror \
        -o "./$APP$EXT" \
        $LIBRARIES"
    fi
    echo $cc
    eval $cc
    if [[ -n "${TEST_ARGS[$APP]}" ]]; then
        exe="./"$APP$EXT" ${TEST_ARGS[$APP]}"
    else
        exe="./"$APP$EXT""
    fi
    echo $exe
    eval $exe
done
