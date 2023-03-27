# Export copy command
# https://stackoverflow.com/questions/697560/how-to-copy-directory-from-source-tree-to-binary-tree slightly modified
macro(resource_dirs dirs)
    foreach(dir ${dirs})
        # Replace / at the end of the path (copy dir content VS copy dir)
        string(REGEX REPLACE "/+$" "" dirclean "${dir}")
        message(STATUS "Copying resource ${dirclean}")
        file(COPY ${dirclean} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()
endmacro()


# Set default options/values
cmake_minimum_required(VERSION 3.13)
project(EGBoy VERSION 1.0.0 LANGUAGES CXX)
add_executable(EGBoy)
target_compile_features(EGBoy PRIVATE cxx_std_20)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Set all options
option(GDBSTUB_INCLUDED "Compile with GDB Stub, OFF by default" OFF)

# Release if no build type specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Define compile flags according to the build type
if (CMAKE_BUILD_TYPE STREQUAL Release)
  target_compile_options(EGBoy PRIVATE -Wall -O3 -s)
else()
  # When compiling as debug
  target_compile_options(EGBoy PRIVATE -Wall -Wextra -Werror -g -fsanitize=address -fsanitize=bounds-strict)
  target_link_options(EGBoy PRIVATE -g -fsanitize=address -fsanitize=bounds-strict)
  target_compile_definitions(EGBoy PRIVATE -DDBG)
endif()


# More compile flags according to the options
if (GDBSTUB_INCLUDED)
  add_definitions(-DGDBSTUB_DBG)
endif()

# Add source files and include dirs
target_include_directories(EGBoy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_sources(EGBoy PRIVATE 
            ${SOURCE_FOLDER}/main.cpp
            ${SOURCE_FOLDER}/Emulator/Emulator.cpp
            ${SOURCE_FOLDER}/Emulator/CPU/CPU.cpp
			${SOURCE_FOLDER}/Emulator/CPU/Instructions/InstrCommon.cpp
			${SOURCE_FOLDER}/Emulator/CPU/Instructions/InstrTable.cpp
            ${SOURCE_FOLDER}/Emulator/Joypad/Joypad.cpp
			${SOURCE_FOLDER}/Emulator/Timer/Timer.cpp
            ${SOURCE_FOLDER}/Emulator/LCD/LCD.cpp
            ${SOURCE_FOLDER}/Emulator/Memory/MMU.cpp
            ${SOURCE_FOLDER}/Emulator/Memory/Cartridge/Cartridge.cpp)

# Find SDL
find_package(SDL2 REQUIRED)

#il faut inclure de dossier ${SDL2_INCLUDE_DIR} où il y a les .h de SDL
include_directories(${SDL2_INCLUDE_DIRS})

# Link against SDL
target_link_libraries(EGBoy ${SDL2_LIBRARIES})

# Copy res dir
file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  resource_dirs("${CMAKE_CURRENT_SOURCE_DIR}/res/")
endif()
