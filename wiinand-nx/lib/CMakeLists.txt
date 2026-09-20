# wiinand: the Wii's NAND formats, for Switch and for the host.
#
# Standalone: it depends on nothing but the standard library, so any project can
# take it. On the host it also builds its tests, which check the library against
# files written by the runtime itself.
cmake_minimum_required(VERSION 3.16)
project(wiinand LANGUAGES CXX)

add_library(wiinand STATIC
    source/sysconf.cpp
    source/miidb.cpp)
target_include_directories(wiinand PUBLIC include)
target_compile_features(wiinand PUBLIC cxx_std_17)

add_library(wiinand::wiinand ALIAS wiinand)

# Tests run on the machine doing the building, against reference files.
if (PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    enable_testing()
    add_executable(wiinand_sysconf_test tests/sysconf_test.cpp)
    target_link_libraries(wiinand_sysconf_test PRIVATE wiinand)
    add_test(NAME sysconf
             COMMAND wiinand_sysconf_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/SYSCONF)

    add_executable(wiinand_miidb_test tests/miidb_test.cpp)
    target_link_libraries(wiinand_miidb_test PRIVATE wiinand)
    add_test(NAME miidb
             COMMAND wiinand_miidb_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/RFL_DB.dat)
endif ()
