# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CMake\bin\cmake.exe

# The command to remove a file.
RM = D:\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\OI\Computer-project\puzzle-loop-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\OI\Computer-project\puzzle-loop-2\build

# Include any dependencies generated for this target.
include CMakeFiles/interface.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/interface.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/interface.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/interface.dir/flags.make

CMakeFiles/interface.dir/src/interface.cpp.obj: CMakeFiles/interface.dir/flags.make
CMakeFiles/interface.dir/src/interface.cpp.obj: D:/OI/Computer-project/puzzle-loop-2/src/interface.cpp
CMakeFiles/interface.dir/src/interface.cpp.obj: CMakeFiles/interface.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OI\Computer-project\puzzle-loop-2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/interface.dir/src/interface.cpp.obj"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/interface.dir/src/interface.cpp.obj -MF CMakeFiles\interface.dir\src\interface.cpp.obj.d -o CMakeFiles\interface.dir\src\interface.cpp.obj -c D:\OI\Computer-project\puzzle-loop-2\src\interface.cpp

CMakeFiles/interface.dir/src/interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/interface.dir/src/interface.cpp.i"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OI\Computer-project\puzzle-loop-2\src\interface.cpp > CMakeFiles\interface.dir\src\interface.cpp.i

CMakeFiles/interface.dir/src/interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/interface.dir/src/interface.cpp.s"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OI\Computer-project\puzzle-loop-2\src\interface.cpp -o CMakeFiles\interface.dir\src\interface.cpp.s

# Object files for target interface
interface_OBJECTS = \
"CMakeFiles/interface.dir/src/interface.cpp.obj"

# External object files for target interface
interface_EXTERNAL_OBJECTS =

libinterface.a: CMakeFiles/interface.dir/src/interface.cpp.obj
libinterface.a: CMakeFiles/interface.dir/build.make
libinterface.a: CMakeFiles/interface.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\OI\Computer-project\puzzle-loop-2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libinterface.a"
	$(CMAKE_COMMAND) -P CMakeFiles\interface.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\interface.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/interface.dir/build: libinterface.a
.PHONY : CMakeFiles/interface.dir/build

CMakeFiles/interface.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\interface.dir\cmake_clean.cmake
.PHONY : CMakeFiles/interface.dir/clean

CMakeFiles/interface.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\OI\Computer-project\puzzle-loop-2 D:\OI\Computer-project\puzzle-loop-2 D:\OI\Computer-project\puzzle-loop-2\build D:\OI\Computer-project\puzzle-loop-2\build D:\OI\Computer-project\puzzle-loop-2\build\CMakeFiles\interface.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/interface.dir/depend

