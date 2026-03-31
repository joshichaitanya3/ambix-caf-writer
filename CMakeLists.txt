cmake_minimum_required(VERSION 3.23)

project(ambix-caf-writer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-fsanitize=address,undefined)
add_compile_options(-fno-omit-frame-pointer)
add_link_options(-fsanitize=address,undefined)

add_executable(monoToAmbix)

include(FetchContent)
FetchContent_Declare(CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG        v2.6.2
)
FetchContent_MakeAvailable(CLI11)

target_link_libraries(monoToAmbix PRIVATE CLI11::CLI11)

# Find the AudioFile header
find_path(AUDIOFILE_INCLUDE_DIR 
    NAMES AudioFile/AudioFile.h
    PATHS /usr/local/include /usr/include
    DOC "AudioFile include directory"
)

if(NOT AUDIOFILE_INCLUDE_DIR)
    message(FATAL_ERROR "AudioFile header not found!")
endif()

# Link the AudioFile include directory
target_include_directories(monoToAmbix PRIVATE ${AUDIOFILE_INCLUDE_DIR})

# Testing setup
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    # Fetch Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0  # Use latest stable version
    )

    # For Windows: Prevent overriding parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Enable testing
    enable_testing()

    # Add test subdirectory
    add_subdirectory(tests)
endif()

add_subdirectory(src)
