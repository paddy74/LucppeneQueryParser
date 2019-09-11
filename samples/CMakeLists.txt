message(STATUS "Generating samples")

add_executable(lqueryparser.example src/example.cpp)
target_link_libraries(lqueryparser.example lqueryparser)

message(STATUS "Generating samples - done")
