# rwlock

A lock-free read-write lock implementation in C++.


## Directory structure

* `include` contains the public header
* `src` contains the sources
* `test` contains the test sources


## How to build

This library is built using CMake.

### Shared library

    $ mkdir lib && cd lib
    $ cmake .. && make rwlock

### Static library

    $ mkdir lib && cd lib
    $ cmake .. && make rwlock-static


## How to test

Build as a shared library. Then run `make rwlock-tests`, then `make test`.
