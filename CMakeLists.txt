cmake_minimum_required(VERSION 2.8.3)

IF( APPLE )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )
  SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++" )
  SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++" )
  SET( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++" )
ENDIF( APPLE )

project(hikari)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O")

#
# MSVC-specific settings
#
# Remove security warnings with Visual C++
#
if( COMPILER_MSVC )
    add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
endif()

add_subdirectory( engine )
