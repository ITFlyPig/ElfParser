# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yuelin/CLionProjects/ElfParser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuelin/CLionProjects/ElfParser/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ElfParser.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ElfParser.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ElfParser.dir/flags.make

CMakeFiles/ElfParser.dir/main.cpp.o: CMakeFiles/ElfParser.dir/flags.make
CMakeFiles/ElfParser.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ElfParser.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ElfParser.dir/main.cpp.o -c /Users/yuelin/CLionProjects/ElfParser/main.cpp

CMakeFiles/ElfParser.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ElfParser.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuelin/CLionProjects/ElfParser/main.cpp > CMakeFiles/ElfParser.dir/main.cpp.i

CMakeFiles/ElfParser.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ElfParser.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuelin/CLionProjects/ElfParser/main.cpp -o CMakeFiles/ElfParser.dir/main.cpp.s

CMakeFiles/ElfParser.dir/ElfInfo.cpp.o: CMakeFiles/ElfParser.dir/flags.make
CMakeFiles/ElfParser.dir/ElfInfo.cpp.o: ../ElfInfo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ElfParser.dir/ElfInfo.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ElfParser.dir/ElfInfo.cpp.o -c /Users/yuelin/CLionProjects/ElfParser/ElfInfo.cpp

CMakeFiles/ElfParser.dir/ElfInfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ElfParser.dir/ElfInfo.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuelin/CLionProjects/ElfParser/ElfInfo.cpp > CMakeFiles/ElfParser.dir/ElfInfo.cpp.i

CMakeFiles/ElfParser.dir/ElfInfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ElfParser.dir/ElfInfo.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuelin/CLionProjects/ElfParser/ElfInfo.cpp -o CMakeFiles/ElfParser.dir/ElfInfo.cpp.s

CMakeFiles/ElfParser.dir/SectionHeader.cpp.o: CMakeFiles/ElfParser.dir/flags.make
CMakeFiles/ElfParser.dir/SectionHeader.cpp.o: ../SectionHeader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ElfParser.dir/SectionHeader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ElfParser.dir/SectionHeader.cpp.o -c /Users/yuelin/CLionProjects/ElfParser/SectionHeader.cpp

CMakeFiles/ElfParser.dir/SectionHeader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ElfParser.dir/SectionHeader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuelin/CLionProjects/ElfParser/SectionHeader.cpp > CMakeFiles/ElfParser.dir/SectionHeader.cpp.i

CMakeFiles/ElfParser.dir/SectionHeader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ElfParser.dir/SectionHeader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuelin/CLionProjects/ElfParser/SectionHeader.cpp -o CMakeFiles/ElfParser.dir/SectionHeader.cpp.s

CMakeFiles/ElfParser.dir/ElfHeader.cpp.o: CMakeFiles/ElfParser.dir/flags.make
CMakeFiles/ElfParser.dir/ElfHeader.cpp.o: ../ElfHeader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ElfParser.dir/ElfHeader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ElfParser.dir/ElfHeader.cpp.o -c /Users/yuelin/CLionProjects/ElfParser/ElfHeader.cpp

CMakeFiles/ElfParser.dir/ElfHeader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ElfParser.dir/ElfHeader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuelin/CLionProjects/ElfParser/ElfHeader.cpp > CMakeFiles/ElfParser.dir/ElfHeader.cpp.i

CMakeFiles/ElfParser.dir/ElfHeader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ElfParser.dir/ElfHeader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuelin/CLionProjects/ElfParser/ElfHeader.cpp -o CMakeFiles/ElfParser.dir/ElfHeader.cpp.s

CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o: CMakeFiles/ElfParser.dir/flags.make
CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o: ../ProgramHeader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o -c /Users/yuelin/CLionProjects/ElfParser/ProgramHeader.cpp

CMakeFiles/ElfParser.dir/ProgramHeader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ElfParser.dir/ProgramHeader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuelin/CLionProjects/ElfParser/ProgramHeader.cpp > CMakeFiles/ElfParser.dir/ProgramHeader.cpp.i

CMakeFiles/ElfParser.dir/ProgramHeader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ElfParser.dir/ProgramHeader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuelin/CLionProjects/ElfParser/ProgramHeader.cpp -o CMakeFiles/ElfParser.dir/ProgramHeader.cpp.s

# Object files for target ElfParser
ElfParser_OBJECTS = \
"CMakeFiles/ElfParser.dir/main.cpp.o" \
"CMakeFiles/ElfParser.dir/ElfInfo.cpp.o" \
"CMakeFiles/ElfParser.dir/SectionHeader.cpp.o" \
"CMakeFiles/ElfParser.dir/ElfHeader.cpp.o" \
"CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o"

# External object files for target ElfParser
ElfParser_EXTERNAL_OBJECTS =

ElfParser: CMakeFiles/ElfParser.dir/main.cpp.o
ElfParser: CMakeFiles/ElfParser.dir/ElfInfo.cpp.o
ElfParser: CMakeFiles/ElfParser.dir/SectionHeader.cpp.o
ElfParser: CMakeFiles/ElfParser.dir/ElfHeader.cpp.o
ElfParser: CMakeFiles/ElfParser.dir/ProgramHeader.cpp.o
ElfParser: CMakeFiles/ElfParser.dir/build.make
ElfParser: CMakeFiles/ElfParser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ElfParser"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ElfParser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ElfParser.dir/build: ElfParser

.PHONY : CMakeFiles/ElfParser.dir/build

CMakeFiles/ElfParser.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ElfParser.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ElfParser.dir/clean

CMakeFiles/ElfParser.dir/depend:
	cd /Users/yuelin/CLionProjects/ElfParser/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuelin/CLionProjects/ElfParser /Users/yuelin/CLionProjects/ElfParser /Users/yuelin/CLionProjects/ElfParser/cmake-build-debug /Users/yuelin/CLionProjects/ElfParser/cmake-build-debug /Users/yuelin/CLionProjects/ElfParser/cmake-build-debug/CMakeFiles/ElfParser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ElfParser.dir/depend

