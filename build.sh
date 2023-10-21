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

for APP in "$@"; do
clang "./tests/$APP.c"\
    -Werror \
    -o "./out/$APP$EXT" \
    $LIBRARIES
done

cd out

for APP in "$@"; do
./$APP$EXT
done
