cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(_RWLOCK_SOURCES src/rwlock.cpp src/rwlockable.cpp src/rwlockable_counters.cpp)
set(_RWLOCK_COMPILE_FLAGS "-Wall -std=c++11")
set(_RWLOCK_LINK_FLAGS "-Wl,-z,defs")

add_library(rwlock SHARED ${_RWLOCK_SOURCES})
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

target_include_directories(rwlock PUBLIC "include")
target_include_directories(rwlock_static PUBLIC "include")

find_package(CxxTest)
if (CXXTEST_FOUND)
    enable_testing()
    cxxtest_add_test(rwlock_test_isLockFree test-runner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/is_lock_free.hpp)
    target_link_libraries(rwlock_test_isLockFree rwlock)
    set_target_properties(rwlock_test_isLockFree PROPERTIES COMPILE_FLAGS "-std=c++11")
else()
    message(WARNING, "CxxTest was not found on the system. Tests are disabled!")
endif()
