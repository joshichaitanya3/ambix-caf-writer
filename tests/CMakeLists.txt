# Create test executable
add_executable(endian_tests
    test_endianness.cc
    # test_caf_writer.cpp
    # Add more test files as needed
)

# Link against Google Test and your code
target_link_libraries(endian_tests
    gtest_main
    gtest
)

# Include your source headers for testing
target_include_directories(endian_tests PRIVATE 
    ../src
    /usr/local/include  # For AudioFile
)

# Discover tests automatically
include(GoogleTest)
gtest_discover_tests(endian_tests)
