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
TEST_ARGS["args"]="-something2=1 -a -c cool"

for APP in "$@"; do
    clang "../tests/$APP.c" \
        -Werror \
        -o "./$APP$EXT" \
        $LIBRARIES
    if [[ -n "${TEST_ARGS[$APP]}" ]]; then
        ./"$APP$EXT" ${TEST_ARGS[$APP]}
    else
        ./"$APP$EXT"
    fi
done
