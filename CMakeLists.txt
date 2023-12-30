cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(osapi)

set(APP_SOURCES "")

if(CONFIG_OSAPI_WITH_CUNIT)
    target_include_directories(app PRIVATE "cunit/Headers")

    file(GLOB_RECURSE CUNIT_SOURCES "cunit/Sources/*.c")
    list(APPEND APP_SOURCES ${CUNIT_SOURCES})

    file(GLOB_RECURSE CUNIT_TESTS "src/cunit_tests/*.cpp")
    list(APPEND APP_SOURCES ${CUNIT_TESTS})

    execute_process(COMMAND git apply cunit_patch.patch
                    RESULT_VARIABLE GIT_APPLY_RESULT
                    OUTPUT_QUIET
                    ERROR_QUIET)
    if(NOT "${GIT_APPLY_RESULT}" EQUAL "0")
        message(WARNING "Failed to apply the git patch.")
    endif()
else()
    list(APPEND APP_SOURCES "src/main.cpp")
endif()

list(APPEND APP_SOURCES "src/osapi/zephyr/osapi_zephyr.cpp")
target_include_directories(app PRIVATE "src/osapi")

target_sources(app PRIVATE ${APP_SOURCES})
