# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jgd/workplace/cpp-http-survey/cpp/seastar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgd/workplace/cpp-http-survey/cpp/seastar/build

# Include any dependencies generated for this target.
include CMakeFiles/regular_seastar_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/regular_seastar_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/regular_seastar_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/regular_seastar_client.dir/flags.make

CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o: CMakeFiles/regular_seastar_client.dir/flags.make
CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o: /home/jgd/workplace/cpp-http-survey/cpp/seastar/regular_seastar_client.cpp
CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o: CMakeFiles/regular_seastar_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jgd/workplace/cpp-http-survey/cpp/seastar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o -MF CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o.d -o CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o -c /home/jgd/workplace/cpp-http-survey/cpp/seastar/regular_seastar_client.cpp

CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgd/workplace/cpp-http-survey/cpp/seastar/regular_seastar_client.cpp > CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.i

CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgd/workplace/cpp-http-survey/cpp/seastar/regular_seastar_client.cpp -o CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.s

# Object files for target regular_seastar_client
regular_seastar_client_OBJECTS = \
"CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o"

# External object files for target regular_seastar_client
regular_seastar_client_EXTERNAL_OBJECTS =

/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: CMakeFiles/regular_seastar_client.dir/regular_seastar_client.cpp.o
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: CMakeFiles/regular_seastar_client.dir/build.make
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/local/lib/libseastar.a
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libcares.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libfmt.so.9.1.0
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/liblz4.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libgnutls.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libsctp.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libprotobuf.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libyaml-cpp.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: /usr/lib/x86_64-linux-gnu/libhwloc.so
/home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client: CMakeFiles/regular_seastar_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jgd/workplace/cpp-http-survey/cpp/seastar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/regular_seastar_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/regular_seastar_client.dir/build: /home/jgd/workplace/cpp-http-survey/executable/regular_seastar_client
.PHONY : CMakeFiles/regular_seastar_client.dir/build

CMakeFiles/regular_seastar_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/regular_seastar_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/regular_seastar_client.dir/clean

CMakeFiles/regular_seastar_client.dir/depend:
	cd /home/jgd/workplace/cpp-http-survey/cpp/seastar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgd/workplace/cpp-http-survey/cpp/seastar /home/jgd/workplace/cpp-http-survey/cpp/seastar /home/jgd/workplace/cpp-http-survey/cpp/seastar/build /home/jgd/workplace/cpp-http-survey/cpp/seastar/build /home/jgd/workplace/cpp-http-survey/cpp/seastar/build/CMakeFiles/regular_seastar_client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/regular_seastar_client.dir/depend

