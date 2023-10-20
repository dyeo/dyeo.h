#!/usr/bin/bash

set -xe

APP=$1

LIBRARIES=""
if [[ $OSTYPE == msys* || $OSTYPE == mingw* || $OSTYPE == cygwin* ]]; then
    LIBRARIES="-lShlwapi -lwinmm -lws2_32"
else
    LIBRARIES="-lm -lasound -lnsl -lresolv"
fi

mkdir -p out
clang "./tests/$APP.c"\
    -Werror \
    -o "./out/$APP" \
    $LIBRARIES

cd out
./$APP
