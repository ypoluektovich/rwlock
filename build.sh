#!/bin/bash

set -e

if [[ ! -e lib ]]; then
    mkdir lib
elif [[ ! -d lib ]]; then
    echo "Error: lib is not a directory!" 1>&2
    exit 1
fi
cd lib

LIBNAME=rwlock
LIBVER_MAJOR=2
LIBVER_MINOR=0
LIBVER_REV=0

SONAME="lib${LIBNAME}.so.${LIBVER_MAJOR}"
g++ -std=c++11 -fpic -Wall -g -c -I ../include ../src/*.cpp
g++ -g -shared "-Wl,-soname,${SONAME}" "-Wl,-z,defs" -o "${SONAME}.${LIBVER_MINOR}.${LIBVER_REV}" *.o
