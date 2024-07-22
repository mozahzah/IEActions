# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)
project(IEActions VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("IEActions\n")

if(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(GLOB IMPL_CPP_FILES "./Implementation/*.cpp")
file(GLOB SOURCE_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${IMPL_CPP_FILES})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ".")
file(GLOB HEADER_FILES "./*.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${HEADER_FILES})

if(TARGET IECore)
  message("Target IECore is already defined.")
else()
  add_subdirectory(ThirdParty/IECore)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC IECore)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${COREAUDIO_LIBRARY})
endif()

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(InstallRequiredSystemLibraries)

get_target_property(DEPENDENCIES ${PROJECT_NAME} LINK_LIBRARIES)
foreach(DEPENDENCY ${DEPENDENCIES})
  if(TARGET ${DEPENDENCY})
    message("Installing dependency: ${DEPENDENCY}")
    install(TARGETS ${DEPENDENCY}
        EXPORT "${PROJECT_NAME}Targets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()
endforeach()

message("------------------------------------------------------------\n")