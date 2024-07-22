# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)

message("\n------------------------------------------------------------")
message("IEActions\n")

if(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
endif()

file(GLOB IMPL_CPP_FILES "./Implementation/*.cpp")
file(GLOB SOURCE_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${IMPL_CPP_FILES})
add_library(IEActions STATIC ${SOURCE_FILES})
target_include_directories(IEActions PUBLIC "./")
target_link_libraries(IEActions PUBLIC IECore)

if(APPLE)
    target_link_libraries(IEActions PUBLIC ${COREAUDIO_LIBRARY})
endif()

set(IEACTIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "")
message("IEACTIONS_DIR is cached as ${IEACTIONS_DIR}")

message("------------------------------------------------------------\n")