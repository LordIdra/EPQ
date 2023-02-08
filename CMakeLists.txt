CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT("LAMBDA") # Largely Arbitrary Monstrously Bland Acronym

# --- FILES --- #
# Main files
SET(MAIN_FILE
    "compile.cpp"
)

# Source files
SET(SOURCE_FILES
    "util/util.cpp" 
    "util/errors.cpp" 
    "readfile/readfile.cpp"
    "scanner/scanner.cpp"
    "parser/parser.cpp"
    "parser/sets/setUtil.cpp"
    "parser/sets/first.cpp"
    "parser/sets/follow.cpp"
    "parser/parser.cpp"
    "parser/table.cpp"
    "semanticAnalyser/scopes/scope.cpp"
    "semanticAnalyser/scopes/scopeManager.cpp"
    "semanticAnalyser/scopes/scopeTraverser.cpp"
    "semanticAnalyser/semanticErrors.cpp"
    "semanticAnalyser/checks.cpp"
    "semanticAnalyser/semanticAnalyser.cpp"
    "assembler/assembler.cpp"
    "generator/assembly.cpp"
    "generator/generator.cpp"
    "generator/memory.cpp"
    "generator/registers.cpp"
    "simulator/simulator.cpp"
)

# Test files
SET(TEST_FILES
    "test_util.cpp"
    "test_readfile.cpp"
    "test_scanner.cpp"
    "test_parser.cpp"
    "test_semanticAnalyser.cpp"
    "test_grammar.cpp"
    "test_assembler.cpp"
    "test_integration.cpp"
    "treesIdentical.cpp"
)

#set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Create library
ADD_LIBRARY(LAMBDA_LIBRARY INTERFACE)
TARGET_COMPILE_OPTIONS(LAMBDA_LIBRARY INTERFACE -O0 -g  --coverage) # -fstandalone-debug
TARGET_LINK_LIBRARIES(LAMBDA_LIBRARY INTERFACE --coverage)

# Subdirectories
#ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)