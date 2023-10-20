#!/usr/bin/bash

set -xe

APP=$1

EXT=""
if [[ $OSTYPE == msys* || $OSTYPE == mingw* || $OSTYPE == cygwin* ]]; then
    EXT=".exe"
    LIBRARIES="-lShlwapi -lwinmm -lws2_32"
else
    EXT=""
    LIBRARIES="-lm -lasound -lnsl -lresolv"
fi

mkdir -p out
clang "./tests/$APP.c"\
    -Werror \
    -o "./out/$APP$EXT" \
    $LIBRARIES

cd out
./$APP$EXT
