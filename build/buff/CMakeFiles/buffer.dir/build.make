# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ruanjian/workspace/project/my_code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ruanjian/workspace/project/my_code/build

# Include any dependencies generated for this target.
include buff/CMakeFiles/buffer.dir/depend.make

# Include the progress variables for this target.
include buff/CMakeFiles/buffer.dir/progress.make

# Include the compile flags for this target's objects.
include buff/CMakeFiles/buffer.dir/flags.make

buff/CMakeFiles/buffer.dir/byte_buffer.cc.o: buff/CMakeFiles/buffer.dir/flags.make
buff/CMakeFiles/buffer.dir/byte_buffer.cc.o: ../buff/byte_buffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/my_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object buff/CMakeFiles/buffer.dir/byte_buffer.cc.o"
	cd /home/ruanjian/workspace/project/my_code/build/buff && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/buffer.dir/byte_buffer.cc.o -c /home/ruanjian/workspace/project/my_code/buff/byte_buffer.cc

buff/CMakeFiles/buffer.dir/byte_buffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buffer.dir/byte_buffer.cc.i"
	cd /home/ruanjian/workspace/project/my_code/build/buff && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/my_code/buff/byte_buffer.cc > CMakeFiles/buffer.dir/byte_buffer.cc.i

buff/CMakeFiles/buffer.dir/byte_buffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buffer.dir/byte_buffer.cc.s"
	cd /home/ruanjian/workspace/project/my_code/build/buff && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/my_code/buff/byte_buffer.cc -o CMakeFiles/buffer.dir/byte_buffer.cc.s

# Object files for target buffer
buffer_OBJECTS = \
"CMakeFiles/buffer.dir/byte_buffer.cc.o"

# External object files for target buffer
buffer_EXTERNAL_OBJECTS =

buff/lib/libbuffer.so: buff/CMakeFiles/buffer.dir/byte_buffer.cc.o
buff/lib/libbuffer.so: buff/CMakeFiles/buffer.dir/build.make
buff/lib/libbuffer.so: buff/CMakeFiles/buffer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruanjian/workspace/project/my_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library lib/libbuffer.so"
	cd /home/ruanjian/workspace/project/my_code/build/buff && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/buffer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
buff/CMakeFiles/buffer.dir/build: buff/lib/libbuffer.so

.PHONY : buff/CMakeFiles/buffer.dir/build

buff/CMakeFiles/buffer.dir/clean:
	cd /home/ruanjian/workspace/project/my_code/build/buff && $(CMAKE_COMMAND) -P CMakeFiles/buffer.dir/cmake_clean.cmake
.PHONY : buff/CMakeFiles/buffer.dir/clean

buff/CMakeFiles/buffer.dir/depend:
	cd /home/ruanjian/workspace/project/my_code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruanjian/workspace/project/my_code /home/ruanjian/workspace/project/my_code/buff /home/ruanjian/workspace/project/my_code/build /home/ruanjian/workspace/project/my_code/build/buff /home/ruanjian/workspace/project/my_code/build/buff/CMakeFiles/buffer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : buff/CMakeFiles/buffer.dir/depend
