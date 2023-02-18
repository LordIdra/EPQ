# Enable code coverage
ADD_COMPILE_OPTIONS(-fprofile-arcs -ftest-coverage --coverage)

# Enable clang tidy checks
#FIND_PROGRAM(CCACHE ccache)
#SET(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion,-readability-magic-numbers,-bugprone-narrowing-conversions,-modernize-pass-by-value,-cppcoreguidelines-avoid-magic-numbers,-performance-inefficient-string-concatenation)

# Find dependencies
INCLUDE("~/vcpkg/scripts/buildsystems/vcpkg.cmake")
FIND_PACKAGE(Catch2 3 REQUIRED)

# Specify executable
LIST(TRANSFORM SOURCE_FILES PREPEND ../src/)
ADD_EXECUTABLE(LAMBDA_TESTS ${TEST_FILES} ${SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(LAMBDA_TESTS PRIVATE "../src")

# Link libraries
TARGET_LINK_LIBRARIES(LAMBDA_TESTS PRIVATE Catch2::Catch2WithMain --coverage)
TARGET_LINK_LIBRARIES(LAMBDA_TESTS PRIVATE LAMBDA_LIBRARY)

# Custom commands
ADD_CUSTOM_COMMAND(TARGET LAMBDA_TESTS POST_BUILD COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "--- TESTS BUILT ---")

# 1) Compile
# 2) run tests
# 3) lcov -q --directory . --capture -o lvcov.info