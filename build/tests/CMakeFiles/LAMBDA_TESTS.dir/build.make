# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/idra/Sync/EPQ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/idra/Sync/EPQ/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/LAMBDA_TESTS.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/LAMBDA_TESTS.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/LAMBDA_TESTS.dir/flags.make

tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o: tests/CMakeFiles/LAMBDA_TESTS.dir/flags.make
tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o: ../tests/test_assembler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/idra/Sync/EPQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o -c /home/idra/Sync/EPQ/tests/test_assembler.cpp

tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.i"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/idra/Sync/EPQ/tests/test_assembler.cpp > CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.i

tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.s"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/idra/Sync/EPQ/tests/test_assembler.cpp -o CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.s

tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o: tests/CMakeFiles/LAMBDA_TESTS.dir/flags.make
tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o: ../tests/test_util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/idra/Sync/EPQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o -c /home/idra/Sync/EPQ/tests/test_util.cpp

tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.i"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/idra/Sync/EPQ/tests/test_util.cpp > CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.i

tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.s"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/idra/Sync/EPQ/tests/test_util.cpp -o CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.s

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o: tests/CMakeFiles/LAMBDA_TESTS.dir/flags.make
tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o: ../src/assembler/assembler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/idra/Sync/EPQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o -c /home/idra/Sync/EPQ/src/assembler/assembler.cpp

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.i"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/idra/Sync/EPQ/src/assembler/assembler.cpp > CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.i

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.s"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/idra/Sync/EPQ/src/assembler/assembler.cpp -o CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.s

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o: tests/CMakeFiles/LAMBDA_TESTS.dir/flags.make
tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o: ../src/util/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/idra/Sync/EPQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o -c /home/idra/Sync/EPQ/src/util/util.cpp

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.i"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/idra/Sync/EPQ/src/util/util.cpp > CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.i

tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.s"
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/idra/Sync/EPQ/src/util/util.cpp -o CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.s

# Object files for target LAMBDA_TESTS
LAMBDA_TESTS_OBJECTS = \
"CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o" \
"CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o" \
"CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o" \
"CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o"

# External object files for target LAMBDA_TESTS
LAMBDA_TESTS_EXTERNAL_OBJECTS =

tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/test_assembler.cpp.o
tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/test_util.cpp.o
tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/assembler/assembler.cpp.o
tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/__/src/util/util.cpp.o
tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/build.make
tests/LAMBDA_TESTS: /home/idra/vcpkg/installed/x64-linux/debug/lib/manual-link/libCatch2Maind.a
tests/LAMBDA_TESTS: /home/idra/vcpkg/installed/x64-linux/debug/lib/libCatch2d.a
tests/LAMBDA_TESTS: tests/CMakeFiles/LAMBDA_TESTS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/idra/Sync/EPQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable LAMBDA_TESTS"
	cd /home/idra/Sync/EPQ/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LAMBDA_TESTS.dir/link.txt --verbose=$(VERBOSE)
	cd /home/idra/Sync/EPQ/build/tests && /usr/bin/cmake -E cmake_echo_color --cyan ---\ TESTS\ BUILT\ ---

# Rule to build all files generated by this target.
tests/CMakeFiles/LAMBDA_TESTS.dir/build: tests/LAMBDA_TESTS

.PHONY : tests/CMakeFiles/LAMBDA_TESTS.dir/build

tests/CMakeFiles/LAMBDA_TESTS.dir/clean:
	cd /home/idra/Sync/EPQ/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/LAMBDA_TESTS.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/LAMBDA_TESTS.dir/clean

tests/CMakeFiles/LAMBDA_TESTS.dir/depend:
	cd /home/idra/Sync/EPQ/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/idra/Sync/EPQ /home/idra/Sync/EPQ/tests /home/idra/Sync/EPQ/build /home/idra/Sync/EPQ/build/tests /home/idra/Sync/EPQ/build/tests/CMakeFiles/LAMBDA_TESTS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/LAMBDA_TESTS.dir/depend
