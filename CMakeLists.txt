cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(_RWLOCK_SOURCES src/rwlock.cpp src/rwlockable.cpp src/rwlockable_counters.cpp)
set(_RWLOCK_COMPILE_FLAGS "-Wall -std=c++11")
set(_RWLOCK_LINK_FLAGS "-Wl,-z,defs")

add_library(rwlock SHARED EXCLUDE_FROM_ALL ${_RWLOCK_SOURCES})
add_library(rwlock_static EXCLUDE_FROM_ALL STATIC ${_RWLOCK_SOURCES})

set_target_properties(
        rwlock rwlock_static
        PROPERTIES
        COMPILE_FLAGS "${_RWLOCK_COMPILE_FLAGS}"
        LINK_FLAGS "${_RWLOCK_LINK_FLAGS}"
)

set_target_properties(
        rwlock
        PROPERTIES
        VERSION "2.1.0"
        SOVERSION "2"
)

include_directories("include")
