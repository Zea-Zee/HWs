# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.2.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.2.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\HWs\OOP\Phillipov homeworks"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\HWs\OOP\Phillipov homeworks\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Phillipov_homeworks.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Phillipov_homeworks.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Phillipov_homeworks.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Phillipov_homeworks.dir/flags.make

CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj: CMakeFiles/Phillipov_homeworks.dir/flags.make
CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj: C:/HWs/OOP/Phillipov\ homeworks/12th.cpp
CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj: CMakeFiles/Phillipov_homeworks.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\HWs\OOP\Phillipov homeworks\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj -MF CMakeFiles\Phillipov_homeworks.dir\12th.cpp.obj.d -o CMakeFiles\Phillipov_homeworks.dir\12th.cpp.obj -c "C:\HWs\OOP\Phillipov homeworks\12th.cpp"

CMakeFiles/Phillipov_homeworks.dir/12th.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Phillipov_homeworks.dir/12th.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\HWs\OOP\Phillipov homeworks\12th.cpp" > CMakeFiles\Phillipov_homeworks.dir\12th.cpp.i

CMakeFiles/Phillipov_homeworks.dir/12th.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Phillipov_homeworks.dir/12th.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\HWs\OOP\Phillipov homeworks\12th.cpp" -o CMakeFiles\Phillipov_homeworks.dir\12th.cpp.s

# Object files for target Phillipov_homeworks
Phillipov_homeworks_OBJECTS = \
"CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj"

# External object files for target Phillipov_homeworks
Phillipov_homeworks_EXTERNAL_OBJECTS =

Phillipov_homeworks.exe: CMakeFiles/Phillipov_homeworks.dir/12th.cpp.obj
Phillipov_homeworks.exe: CMakeFiles/Phillipov_homeworks.dir/build.make
Phillipov_homeworks.exe: CMakeFiles/Phillipov_homeworks.dir/linkLibs.rsp
Phillipov_homeworks.exe: CMakeFiles/Phillipov_homeworks.dir/objects1.rsp
Phillipov_homeworks.exe: CMakeFiles/Phillipov_homeworks.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\HWs\OOP\Phillipov homeworks\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Phillipov_homeworks.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Phillipov_homeworks.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Phillipov_homeworks.dir/build: Phillipov_homeworks.exe
.PHONY : CMakeFiles/Phillipov_homeworks.dir/build

CMakeFiles/Phillipov_homeworks.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Phillipov_homeworks.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Phillipov_homeworks.dir/clean

CMakeFiles/Phillipov_homeworks.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\HWs\OOP\Phillipov homeworks" "C:\HWs\OOP\Phillipov homeworks" "C:\HWs\OOP\Phillipov homeworks\cmake-build-debug" "C:\HWs\OOP\Phillipov homeworks\cmake-build-debug" "C:\HWs\OOP\Phillipov homeworks\cmake-build-debug\CMakeFiles\Phillipov_homeworks.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Phillipov_homeworks.dir/depend

