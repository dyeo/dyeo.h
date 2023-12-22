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
cp -r res/ out/res/

cd out

declare -A BUILD_ARGS
declare -A TEST_ARGS
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

COMPILE_ARGS=()
RUNTIME_ARGS=()
SWITCH=false
for arg in "$@"; do
    if $SWITCH; then
        RUNTIME_ARGS+=("$arg")
    elif [[ "$arg" == "--" ]]; then
        SWITCH=true
    else
        COMPILE_ARGS+=("$arg")
    fi
done

if [[ "${COMPILE_ARGS[0]}" == "ALL" ]]; then
    APPS=()
    for file in ../*.h; do
        [ -f "$file" ] && APPS+=("$(basename "$file" .h)")
    done
else
    APPS=("${COMPILE_ARGS[@]}")
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
    echo + $cc
    eval $cc
    if [[ -n "${TEST_ARGS[$APP]}" ]]; then
        exe="./"$APP$EXT" ${TEST_ARGS[$APP]} ${RUNTIME_ARGS[*]}"
    else
        exe="./"$APP$EXT" ${RUNTIME_ARGS[*]}"
    fi
    echo + $exe
    eval $exe
done
