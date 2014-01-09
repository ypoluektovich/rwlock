#!/bin/bash

set -e

source config.sh

echo "About to build $LIBNAME in $(pwd)" 1>&2

if [[ ! -e lib ]]; then
    mkdir lib
elif [[ ! -d lib ]]; then
    echo "Error: lib is not a directory!" 1>&2
    exit 1
fi

cd lib

echo "  - compiling..." 1>&2
rm -f *.o
g++ -std=c++11 -fpic -Wall -g -c -I ../include ../src/*.cpp

echo "  - linking..." 1>&2
rm -f ${SOLINKERNAME}*
g++ -g -shared "-Wl,-soname,${SONAME}" "-Wl,-z,defs" -o "${SOREALNAME}" *.o

echo "Built successsfully, setting up links..." 1>&2
ln -s "$SOREALNAME" "$SONAME"
ln -s "$SONAME" "$SOLINKERNAME"
echo "Done." 1>&2
