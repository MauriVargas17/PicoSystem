# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/maurivargas17/Desktop/PicoProject/hello_pico

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/maurivargas17/Desktop/PicoProject/hello_pico/build

# Utility rule file for picosystem_screen_pio_h.

# Include any custom commands dependencies for this target.
include CMakeFiles/picosystem_screen_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/picosystem_screen_pio_h.dir/progress.make

CMakeFiles/picosystem_screen_pio_h: screen.pio.h

screen.pio.h: /Users/maurivargas17/Desktop/PicoProject/picosystem/libraries/screen.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/maurivargas17/Desktop/PicoProject/hello_pico/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating screen.pio.h"
	pioasm/pioasm -o c-sdk /Users/maurivargas17/Desktop/PicoProject/picosystem/libraries/screen.pio /Users/maurivargas17/Desktop/PicoProject/hello_pico/build/screen.pio.h

picosystem_screen_pio_h: CMakeFiles/picosystem_screen_pio_h
picosystem_screen_pio_h: screen.pio.h
picosystem_screen_pio_h: CMakeFiles/picosystem_screen_pio_h.dir/build.make
.PHONY : picosystem_screen_pio_h

# Rule to build all files generated by this target.
CMakeFiles/picosystem_screen_pio_h.dir/build: picosystem_screen_pio_h
.PHONY : CMakeFiles/picosystem_screen_pio_h.dir/build

CMakeFiles/picosystem_screen_pio_h.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/picosystem_screen_pio_h.dir/cmake_clean.cmake
.PHONY : CMakeFiles/picosystem_screen_pio_h.dir/clean

CMakeFiles/picosystem_screen_pio_h.dir/depend:
	cd /Users/maurivargas17/Desktop/PicoProject/hello_pico/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/maurivargas17/Desktop/PicoProject/hello_pico /Users/maurivargas17/Desktop/PicoProject/hello_pico /Users/maurivargas17/Desktop/PicoProject/hello_pico/build /Users/maurivargas17/Desktop/PicoProject/hello_pico/build /Users/maurivargas17/Desktop/PicoProject/hello_pico/build/CMakeFiles/picosystem_screen_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/picosystem_screen_pio_h.dir/depend

