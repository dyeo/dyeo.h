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

for APP in "$@"; do
    clang "../tests/$APP.c" \
        -Werror \
        -o "./$APP$EXT" \
        $LIBRARIES
    ./$APP$EXT
done
