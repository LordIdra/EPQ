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
    "util/util.cpp"
)

# Test files
SET(TEST_FILES
    "test_assembler.cpp"
    "test_util.cpp"
)

# Create library
ADD_LIBRARY(LAMBDA_LIBRARY INTERFACE)
TARGET_COMPILE_OPTIONS(LAMBDA_LIBRARY INTERFACE -O0 -g --coverage)
TARGET_LINK_LIBRARIES(LAMBDA_LIBRARY INTERFACE --coverage)

# Subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)