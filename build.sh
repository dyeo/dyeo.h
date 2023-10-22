#!/usr/bin/bash

set -xe

if [[ $OSTYPE == msys* || $OSTYPE == mingw* || $OSTYPE == cygwin* ]]; then
    EXT=".exe"
    LIBRARIES="-lShlwapi -lwinmm -lws2_32"
else
    EXT=""
    LIBRARIES="-lm -lasound -lnsl -lresolv"
fi

mkdir -p out
mkdir -p res

cd out
cp -r ../res res/

declare -A TEST_ARGS
TEST_ARGS["args"]="-something2=1 -a -c sine.wav"

set +xe
if [[ "$1" == "ALL" ]]; then
    APPS=()
    for file in ../*.h; do
        [ -f "$file" ] && APPS+=("$(basename "$file" .h)")
    done
else
    APPS=("$@")
fi

set +x
for APP in "${APPS[@]}"; do
    cc="clang "../tests/$APP.c" \
        -Werror \
        -o "./$APP$EXT" \
        $LIBRARIES"
    echo + $cc
    eval $cc
    if [[ -n "${TEST_ARGS[$APP]}" ]]; then
        exe="./"$APP$EXT" ${TEST_ARGS[$APP]}"
    else
        exe="./"$APP$EXT""
    fi
    echo + $exe
    eval $exe
done
