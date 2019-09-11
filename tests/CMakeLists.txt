message(STATUS "Generating tests")

# Create and link the test executables
add_executable(lqueryparser.test_main src/test_main.cpp)
target_link_libraries(lqueryparser.test_main lqueryparser)

# Test definitions
macro(create_test target)
    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro(create_test)

create_test(lqueryparser.test_main)

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
            lqueryparser.test_main
    )
endif()
