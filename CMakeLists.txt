cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(_RWLOCK_SOURCES src/rwlock.cpp src/rwlockable.cpp src/rwlockable_counters.cpp)
set(_RWLOCK_COMPILE_FLAGS "-Wall -Wextra -Wpedantic -std=c++11")
set(_RWLOCK_LINK_FLAGS "-Wl,-z,defs")

add_library(rwlock        SHARED EXCLUDE_FROM_ALL ${_RWLOCK_SOURCES})
add_library(rwlock_static STATIC EXCLUDE_FROM_ALL ${_RWLOCK_SOURCES})

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
    add_custom_target(rwlock-tests)
    foreach(_test_name IN ITEMS is_lock_free)
        set(_test_target_name "rwlock_test_${_test_name}")
        cxxtest_add_test(${_test_target_name} "test-${_test_name}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/${_test_name}.hpp")
        target_link_libraries(${_test_target_name} rwlock)
        set_target_properties(
                ${_test_target_name} PROPERTIES
                COMPILE_FLAGS "-std=c++11"
                EXCLUDE_FROM_ALL ON
        )
        add_dependencies(rwlock-tests ${_test_target_name})
    endforeach()
else()
    message(WARNING, "CxxTest was not found on the system. Tests are disabled!")
endif()
