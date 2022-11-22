CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT("LAMBDA") # Largely Arbitrary Monstrously Bland Acronym

# --- FILES --- #
# Main files
SET(MAIN_FILE
    "compile.cpp"
)

# Source files
SET(SOURCE_FILES
    "assembler/assembler.cpp"
    "readfile/readfile.cpp"
    "scanner/scanner.cpp"
    "util/util.cpp"
)

# Test files
SET(TEST_FILES
    "test_assembler.cpp"
    "test_readfile.cpp"
    "test_scanner.cpp"
    "test_util.cpp"
)

#set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Create library
ADD_LIBRARY(LAMBDA_LIBRARY INTERFACE)
TARGET_COMPILE_OPTIONS(LAMBDA_LIBRARY INTERFACE -O0 -g --coverage)
TARGET_LINK_LIBRARIES(LAMBDA_LIBRARY INTERFACE --coverage)

# Subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)