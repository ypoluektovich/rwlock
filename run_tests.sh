#!/bin/bash

set -e

./build.sh

echo "Commencing testing..." 1>&2

source config.sh

cd test
rm -f runner*

echo "  - generating test runner..." 1>&2
cxxtestgen --error-printer -o runner.cpp *.hpp

echo "  - compiling test runner..." 1>&2
g++ -std=c++11 -I../include -L../lib -lrwlock -o runner runner.cpp

echo "  - running tests..." 1>&2
LD_LIBRARY_PATH=../lib ./runner
