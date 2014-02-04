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
    $ cmake .. && make rwlock_static


## How to test

* Unit tests are written using CxxTest framework. Make sure it's present on your system, or CMake will complain.
* Build rwlock as a shared library.
* Run `make rwlock-tests` to generate and build tests.
* Run `make test` to actually run tests.
