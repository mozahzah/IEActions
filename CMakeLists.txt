# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)

if(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
endif()

file(GLOB_RECURSE CPP_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${CPP_FILES} ${MM_FILES})
add_library(IEActions STATIC ${SOURCE_FILES})
target_include_directories(IEActions PUBLIC "./")
target_link_libraries(IEActions PUBLIC IECore)

if(APPLE)
    target_link_libraries(IEActions PUBLIC ${COREAUDIO_LIBRARY})
endif()

set(IEACTIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "")
message("IEACTIONS_DIR is cached as ${IEACTIONS_DIR}")