# Enable code coverage
ADD_COMPILE_OPTIONS(-fprofile-arcs -ftest-coverage --coverage)

# Enable clang tidy checks
FIND_PROGRAM(CCACHE ccache)
SET(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion,-readability-magic-numbers,-bugprone-narrowing-conversions,-modernize-pass-by-value,-cppcoreguidelines-avoid-magic-numbers)

# Export compile commands
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
SET(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})

# Specify executable
ADD_EXECUTABLE(LAMBDA ${SOURCE_FILES} ${MAIN_FILE})
TARGET_INCLUDE_DIRECTORIES(LAMBDA PRIVATE .)

# Link libraries
TARGET_LINK_LIBRARIES(LAMBDA PRIVATE LAMBDA_LIBRARY)

# Custom commands
ADD_CUSTOM_COMMAND(TARGET LAMBDA POST_BUILD COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "--- MAIN  BUILT ---")