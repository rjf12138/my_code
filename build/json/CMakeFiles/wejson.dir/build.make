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
include json/CMakeFiles/wejson.dir/depend.make

# Include the progress variables for this target.
include json/CMakeFiles/wejson.dir/progress.make

# Include the compile flags for this target's objects.
include json/CMakeFiles/wejson.dir/flags.make

json/CMakeFiles/wejson.dir/wejson.cc.o: json/CMakeFiles/wejson.dir/flags.make
json/CMakeFiles/wejson.dir/wejson.cc.o: ../json/wejson.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/my_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object json/CMakeFiles/wejson.dir/wejson.cc.o"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wejson.dir/wejson.cc.o -c /home/ruanjian/workspace/project/my_code/json/wejson.cc

json/CMakeFiles/wejson.dir/wejson.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wejson.dir/wejson.cc.i"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/my_code/json/wejson.cc > CMakeFiles/wejson.dir/wejson.cc.i

json/CMakeFiles/wejson.dir/wejson.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wejson.dir/wejson.cc.s"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/my_code/json/wejson.cc -o CMakeFiles/wejson.dir/wejson.cc.s

json/CMakeFiles/wejson.dir/json_type.cc.o: json/CMakeFiles/wejson.dir/flags.make
json/CMakeFiles/wejson.dir/json_type.cc.o: ../json/json_type.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/my_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object json/CMakeFiles/wejson.dir/json_type.cc.o"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wejson.dir/json_type.cc.o -c /home/ruanjian/workspace/project/my_code/json/json_type.cc

json/CMakeFiles/wejson.dir/json_type.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wejson.dir/json_type.cc.i"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/my_code/json/json_type.cc > CMakeFiles/wejson.dir/json_type.cc.i

json/CMakeFiles/wejson.dir/json_type.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wejson.dir/json_type.cc.s"
	cd /home/ruanjian/workspace/project/my_code/build/json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/my_code/json/json_type.cc -o CMakeFiles/wejson.dir/json_type.cc.s

# Object files for target wejson
wejson_OBJECTS = \
"CMakeFiles/wejson.dir/wejson.cc.o" \
"CMakeFiles/wejson.dir/json_type.cc.o"

# External object files for target wejson
wejson_EXTERNAL_OBJECTS =

../local/lib/libwejson.a: json/CMakeFiles/wejson.dir/wejson.cc.o
../local/lib/libwejson.a: json/CMakeFiles/wejson.dir/json_type.cc.o
../local/lib/libwejson.a: json/CMakeFiles/wejson.dir/build.make
../local/lib/libwejson.a: json/CMakeFiles/wejson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruanjian/workspace/project/my_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../local/lib/libwejson.a"
	cd /home/ruanjian/workspace/project/my_code/build/json && $(CMAKE_COMMAND) -P CMakeFiles/wejson.dir/cmake_clean_target.cmake
	cd /home/ruanjian/workspace/project/my_code/build/json && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wejson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
json/CMakeFiles/wejson.dir/build: ../local/lib/libwejson.a

.PHONY : json/CMakeFiles/wejson.dir/build

json/CMakeFiles/wejson.dir/clean:
	cd /home/ruanjian/workspace/project/my_code/build/json && $(CMAKE_COMMAND) -P CMakeFiles/wejson.dir/cmake_clean.cmake
.PHONY : json/CMakeFiles/wejson.dir/clean

json/CMakeFiles/wejson.dir/depend:
	cd /home/ruanjian/workspace/project/my_code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruanjian/workspace/project/my_code /home/ruanjian/workspace/project/my_code/json /home/ruanjian/workspace/project/my_code/build /home/ruanjian/workspace/project/my_code/build/json /home/ruanjian/workspace/project/my_code/build/json/CMakeFiles/wejson.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json/CMakeFiles/wejson.dir/depend
