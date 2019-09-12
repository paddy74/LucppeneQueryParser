message(STATUS "Generating tests")

# Create and link the test executables
add_executable(lqueryparser.test_boolperator src/test_boolperator.cpp)
target_link_libraries(lqueryparser.test_boolperator lqueryparser)

add_executable(lqueryparser.test_boolperatorPair src/test_boolperatorPair.cpp)
target_link_libraries(lqueryparser.test_boolperatorPair lqueryparser)

add_executable(lqueryparser.test_lqueryparser src/test_lqueryparser.cpp)
target_link_libraries(lqueryparser.test_lqueryparser lqueryparser)

# Test definitions
macro(create_test target)
    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro(create_test)

create_test(lqueryparser.test_boolperator)
create_test(lqueryparser.test_boolperatorPair)
create_test(lqueryparser.test_lqueryparser)

message(STATUS "Generating tests - done")

# -----------------------------------------------------------------------------
# Code Coverage report
# -----------------------------------------------------------------------------
# cmake .. -DENABLE_COVERAGE=ON
# cmake --build .
# make <target_name>
if (ENABLE_COVERAGE)
    include(CodeCoverage)
    set(COVERAGE_LCOV_EXCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../libs/*")

    APPEND_COVERAGE_COMPILER_FLAGS()

    include(ProcessorCount)
    ProcessorCount(N_CORES)

    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME mytemplate.coverage
        EXECUTABLE cd tests/ && ctest -j ${N_CORES}
        DEPENDENCIES
            lqueryparser.test_boolperator
            lqueryparser.test_boolperatorPair
            lqueryparser.test_lqueryparser
    )
endif()
